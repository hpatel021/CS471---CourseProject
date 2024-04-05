#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <cstdlib>
#include <ctime>
#include <chrono>
#include <atomic>

using namespace std;

/**
 * @brief Structure for Sales Record
 */
struct Record {
    int dd, mm, storeID, registerNumber; // Renamed from 'register' to 'registerNumber'
    float saleAmount;
};

// Global Variables
const int b = 10; // buffer size
atomic<int> recordsGenerated(0); // number of records generated
mutex m; // mutex for synchronization
atomic<bool> endFlag(false); // flag to indicate end of record generation

// Shared Variables
vector<Record> buffer; // buffer to store records
int storeWideTotalSales[10] = {0}; // store wise total sales
int monthWiseTotalSales[12] = {0}; // month wise total sales
atomic<int> aggregateSales(0); // aggregate sales
chrono::time_point<chrono::system_clock> startTime; // start time
chrono::time_point<chrono::system_clock> endTime; // end time

// Function Prototypes
void producer(int storeID);
void consumer(int consumerID);

/**
 * @brief Main function to coordinate producers and consumers
 * 
 * @param argc Number of command-line arguments
 * @param argv Array of command-line arguments
 * @return int Exit status
 */
int main(int argc, char *argv[]) {
    int p, c; // number of producers and consumers
    vector<thread> producers; // vector to store producers
    vector<thread> consumers; // vector to store consumers

    // Taking inputs
    cout << "Enter the number of producers: ";
    cin >> p;
    cout << "Enter the number of consumers: ";
    cin >> c;

    // Initializing Shared Variables
    srand(time(nullptr));

    // Creating Producers
    for (int i = 0; i < p; i++)
        producers.push_back(thread(producer, i + 1));

    // Creating Consumers
    for (int i = 0; i < c; i++)
        consumers.push_back(thread(consumer, i + 1));

    // Start time
    startTime = chrono::system_clock::now();

    // Joining Producers
    for (int i = 0; i < p; i++)
        producers[i].join();

    // Set end flag
    endFlag = true;

    // Joining Consumers
    for (int i = 0; i < c; i++)
        consumers[i].join();

    // End time
    endTime = chrono::system_clock::now();

    // Calculating total time
    chrono::duration<double> elapsedTime = endTime - startTime;

    // Printing Overall Statistics
    cout << "\nOverall Statistics:\n";
    cout << "Total Time: " << elapsedTime.count() << " seconds\n";
    cout << "Aggregate Sales: $" << aggregateSales << endl;
    for (int i = 0; i < 10; i++)
        cout << "Store " << i+1 << " Total Sales: $" << storeWideTotalSales[i] << endl;
    for (int i = 0; i < 12; i++)
        cout << "Month " << i+1 << " Total Sales: $" << monthWiseTotalSales[i] << endl;

    return 0;
}

/**
 * @brief Producer Function
 * 
 * @param storeID Store ID of the producer
 */
void producer(int storeID) {
    while (recordsGenerated < 500) {
        // Generating random sales information
        Record record;
        record.dd = (rand() % 30) + 1;
        record.mm = (rand() % 12) + 1;
        record.storeID = storeID;
        record.registerNumber = (rand() % 6) + 1; // Renamed from 'register' to 'registerNumber'
        record.saleAmount = (rand() % 950) + 50;

        // Acquiring mutex
        m.lock();

        // Adding record to buffer
        buffer.push_back(record);
        recordsGenerated++;

        // Releasing mutex
        m.unlock();

        // Sleeping for 5-40 milliseconds
        int sleepTime = (rand() % 36) + 5;
        this_thread::sleep_for(chrono::milliseconds(sleepTime));
    }
}

/**
 * @brief Consumer Function
 * 
 * @param consumerID ID of the consumer
 */
void consumer(int consumerID) {
    int storeWideTotal = 0;
    int monthWiseTotal[12] = {0};
    while (!endFlag || !buffer.empty()) {
        // Acquiring mutex
        m.lock();

        if (!buffer.empty()) {
            // Taking record from buffer
            Record record = buffer.back();
            buffer.pop_back();

            // Calculating local totals
            storeWideTotal += record.saleAmount;
            monthWiseTotal[record.mm - 1] += record.saleAmount;

            // Releasing mutex
            m.unlock();
        }
        else {
            // Releasing mutex
            m.unlock();
            this_thread::sleep_for(chrono::milliseconds(5));
        }
    }

    // Acquiring mutex
    m.lock();

    // Adding local totals to global totals
    storeWideTotalSales[consumerID - 1] = storeWideTotal;
    for (int i = 0; i < 12; i++)
        monthWiseTotalSales[i] += monthWiseTotal[i];
    aggregateSales += storeWideTotal;

    // Releasing mutex
    m.unlock();

    // Printing local statistics
    cout << "\nConsumer " << consumerID << " Statistics:\n";
    cout << "Store " << consumerID << " Total Sales: $" << storeWideTotal << endl;
    for (int i = 0; i < 12; i++)
        cout << "Month " << i+1 << " Total Sales: $" << monthWiseTotal[i] << endl;
}
