#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <iomanip>

using namespace std;

/**
 * FIFO Page Replacement Algorithm
 * 
 * @param virtualAddresses Vector containing virtual addresses referenced by the process
 * @param numFrames Number of frames allocated to the process
 * @return Number of page faults that occurred during the simulation
 */
int fifo(const vector<int>& virtualAddresses, int numFrames) {
    unordered_map<int, bool> frames;
    int pageFaults = 0;

    vector<int> fifoQueue;
    for (int address : virtualAddresses) {
        if (frames.find(address) == frames.end()) {
            pageFaults++;
            if (fifoQueue.size() == numFrames) {
                int oldest = fifoQueue.front();
                fifoQueue.erase(fifoQueue.begin());
                frames.erase(oldest);
            }
            fifoQueue.push_back(address);
            frames[address] = true;
        }
    }

    return pageFaults;
}

/**
 * LRU Page Replacement Algorithm
 * 
 * @param virtualAddresses Vector containing virtual addresses referenced by the process
 * @param numFrames Number of frames allocated to the process
 * @return Number of page faults that occurred during the simulation
 */
int lru(const vector<int>& virtualAddresses, int numFrames) {
    unordered_map<int, int> frames;
    int pageFaults = 0;
    int counter = 0;

    for (int address : virtualAddresses) {
        if (frames.find(address) == frames.end()) {
            pageFaults++;
            if (frames.size() == numFrames) {
                int lru = min_element(frames.begin(), frames.end(),
                    [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; })->first;
                frames.erase(lru);
            }
            frames[address] = counter++;
        } else {
            frames[address] = counter++;
        }
    }

    return pageFaults;
}

/**
 * MRU Page Replacement Algorithm
 * 
 * @param virtualAddresses Vector containing virtual addresses referenced by the process
 * @param numFrames Number of frames allocated to the process
 * @return Number of page faults that occurred during the simulation
 */
int mru(const vector<int>& virtualAddresses, int numFrames) {
    unordered_map<int, int> frames;
    int pageFaults = 0;
    int counter = 0;

    for (int address : virtualAddresses) {
        if (frames.find(address) == frames.end()) {
            pageFaults++;
            if (frames.size() == numFrames) {
                int mru = max_element(frames.begin(), frames.end(),
                    [](const pair<int, int>& a, const pair<int, int>& b) { return a.second < b.second; })->first;
                frames.erase(mru);
            }
            frames[address] = counter++;
        } else {
            frames[address] = counter++;
        }
    }

    return pageFaults;
}

/**
 * Optimal Page Replacement Algorithm
 * 
 * @param virtualAddresses Vector containing virtual addresses referenced by the process
 * @param numFrames Number of frames allocated to the process
 * @return Number of page faults that occurred during the simulation
 */
int optimal(const vector<int>& virtualAddresses, int numFrames) {
    unordered_map<int, int> frames;
    int pageFaults = 0;
    vector<int> futureIndexes(virtualAddresses.size(), 0);

    for (int i = 0; i < virtualAddresses.size(); ++i) {
        futureIndexes[i] = -1;
        for (int j = i + 1; j < virtualAddresses.size(); ++j) {
            if (virtualAddresses[i] == virtualAddresses[j]) {
                futureIndexes[i] = j;
                break;
            }
        }
    }

    for (int i = 0; i < virtualAddresses.size(); ++i) {
        if (frames.find(virtualAddresses[i]) == frames.end()) {
            pageFaults++;
            if (frames.size() == numFrames) {
                int farthest = -1, replace;
                for (const auto& frame : frames) {
                    if (futureIndexes[i] == -1) {
                        replace = frame.first;
                        break;
                    }
                    if (futureIndexes[frame.first] == -1) {
                        replace = frame.first;
                        break;
                    }
                    if (futureIndexes[frame.first] > farthest) {
                        farthest = futureIndexes[frame.first];
                        replace = frame.first;
                    }
                }
                frames.erase(replace);
            }
            frames[virtualAddresses[i]] = 1;
        }
    }

    return pageFaults;
}

/**
 * Function to simulate page replacement algorithm
 * 
 * @param virtualAddresses Vector containing virtual addresses referenced by the process
 * @param pageSize Size of each page in words
 * @param numFrames Number of frames allocated to the process
 * @param algorithm Name of the page replacement algorithm to use
 * @return Number of page faults that occurred during the simulation
 */
int simulate(const vector<int>& virtualAddresses, int pageSize, int numFrames, string algorithm) {
    // For simplicity, assume each page is of size 'pageSize'
    // Convert virtual addresses to page numbers
    vector<int> pageNumbers;
    for (int address : virtualAddresses) {
        pageNumbers.push_back(address / pageSize);
    }

    // Call the appropriate page replacement algorithm based on the input
    if (algorithm == "FIFO") {
        return fifo(pageNumbers, numFrames);
    } else if (algorithm == "LRU") {
        return lru(pageNumbers, numFrames);
    } else if (algorithm == "MRU") {
        return mru(pageNumbers, numFrames);
    } else if (algorithm == "Optimal") {
        return optimal(pageNumbers, numFrames);
    } else {
        cerr << "Unknown algorithm: " << algorithm << endl;
        return -1;
    }
}

/**
 * Main function to run simulations for different combinations of parameters
 * and output the results.
 */
int main() {
    // Define parameters for runs
    vector<pair<int, int>> parameters = {{512, 4}, {512, 12}, {2048, 4}, {2048, 12}};

    // Read virtual addresses from the file
    ifstream infile("SampleInput.txt");
    vector<int> virtualAddresses;
    int address;
    while (infile >> address) {
        virtualAddresses.push_back(address);
    }
    infile.close();

    // Run simulations for each combination of parameters
    vector<string> algorithms = {"FIFO", "LRU", "MRU", "Optimal"};
    for (const auto& param : parameters) {
        int pageSize = param.first;
        int numFrames = param.second;
        cout << "\n--------------------------------------------------\n";
        cout << "           Virtual Memory Management          \n";
        cout << "--------------------------------------------------\n";
        cout << "  Page Size: " << pageSize << " Words, Number of Frames: " << numFrames << "\n";
        cout << "--------------------------------------------------\n";
        cout << left << setw(20) << "Algorithm" << "|  Page Fault Percentage\n";
        cout << "--------------------|-----------------------------\n";

        // Simulate each page replacement algorithm
        for (const auto& algorithm : algorithms) {
            int pageFaults = simulate(virtualAddresses, pageSize, numFrames, algorithm);

            // Calculate page fault percentage
            int totalReferences = virtualAddresses.size();
            double pageFaultPercentage = (double)pageFaults / totalReferences * 100;

            // Output statistics
            cout << left << setw(20) << algorithm << "|  " << fixed << setprecision(2) << pageFaultPercentage << "%" << endl;
        }
        cout << endl;
    }

    return 0;
}
