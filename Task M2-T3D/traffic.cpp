#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <fstream>
#include <iostream>
#include <mutex>
#include <queue>
#include <sstream>
#include <string>
#include <thread>
#include <vector>

using namespace std;

struct TrafficRecord {
    string time;
    string light_id;
    int car_count;
};

class TrafficBuffer {
private:
    const int capacity;
    mutex mtx;
    condition_variable not_empty;
    condition_variable not_full;

public:
    queue<TrafficRecord> data_buffer;
    TrafficBuffer(int capacity) : capacity(capacity) {}

    void add(const TrafficRecord &record);
    TrafficRecord remove();
};

void trafficProducer(const string &filename, TrafficBuffer &buffer);
void trafficConsumer(TrafficBuffer &buffer);

void TrafficBuffer::add(const TrafficRecord &record) {
    unique_lock<mutex> lock(mtx);
    not_full.wait(lock, [this]() { return data_buffer.size() < capacity; });
    data_buffer.push(record);
    not_empty.notify_one();
}

TrafficRecord TrafficBuffer::remove() {
    unique_lock<mutex> lock(mtx);
    not_empty.wait(lock, [this]() { return !data_buffer.empty(); });
    TrafficRecord record = data_buffer.front();
    data_buffer.pop();
    not_full.notify_one();
    return record;
}

void trafficProducer(const string &filename, TrafficBuffer &buffer) {
    ifstream input_file(filename);
    if (!input_file.is_open()) {
        cerr << "Error opening file: " << filename << endl;
        return;
    }

    string line;
    while (getline(input_file, line)) {
        istringstream iss(line);
        string time, light_id;
        int car_count;
        if (!(iss >> time >> light_id >> car_count)) {
            cerr << "Error reading line: " << line << endl;
            continue;
        }
        TrafficRecord record{time, light_id, car_count};
        buffer.add(record);
    }

    input_file.close(); // Close the input file after reading
}

void trafficConsumer(TrafficBuffer &buffer) {
    vector<TrafficRecord> traffic_records;
    string current_hour = "";
    while (true) {
        TrafficRecord record = buffer.remove();
        string hour = record.time.substr(0, 2);

        if (hour >= "13")
            break;

        if (hour != current_hour && !traffic_records.empty()) {
            cout << "Time: " << current_hour << ":00"
                 << "\n\n";
            sort(traffic_records.begin(), traffic_records.end(),
                 [](const TrafficRecord &a, const TrafficRecord &b) {
                     return a.car_count < b.car_count;
                 });

            // Output the most congested traffic lights
            for (int i = traffic_records.size() - 1; i >= max(0, (int)traffic_records.size() - 3); --i) {
                cout << "Traffic Light Id: " << traffic_records[i].light_id << endl;
                cout << "Number of cars: " << traffic_records[i].car_count << endl;
                cout << "-------------------------------------" << endl;
            }
            cout << "\n";

            traffic_records.clear();
        }

        traffic_records.push_back(record);
        current_hour = hour;
    }
}

int main() {
    const int buffer_size = 90;

    // Declare variables to measure time
    auto start_time = chrono::high_resolution_clock::now();
    TrafficBuffer buffer(buffer_size);
    thread producer_thread(trafficProducer, "data.txt", ref(buffer));
    thread consumer_thread(trafficConsumer, ref(buffer));
    producer_thread.join();
    consumer_thread.join();
    auto end_time = chrono::high_resolution_clock::now();

    // Calculate the duration in nanoseconds
    auto duration_ns = chrono::duration_cast<chrono::nanoseconds>(end_time - start_time).count();

    // Output the total execution time in nanoseconds
    cout << "Total execution time: " << duration_ns << " nanoseconds" << endl;

    return 0;
}
