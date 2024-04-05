#include <iostream>
#include <list>
#include <vector>
#include <algorithm>
#include <fstream>
#include <iomanip>
#include <iterator>
#include <unordered_map>

using namespace std;

/**
 * Reads page numbers from a file and returns them as a vector.
 *
 * @param inputFile The name of the input file.
 * @return A vector containing the page numbers read from the file.
 */
vector<int> readPageNumbers(const string& inputFile) {
    ifstream fin(inputFile);
    vector<int> pageNumbers;
    int pageNumber;

    while (fin >> pageNumber) {
        pageNumbers.push_back(pageNumber);
    }
    return pageNumbers;
}

/**
 * Calculates the page fault rate given the number of page faults and total references.
 *
 * @param pageFaults The number of page faults.
 * @param totalReferences The total number of page references.
 * @return The page fault rate as a percentage.
 */
double calculatePageFaultRate(int pageFaults, int totalReferences) {
    return (static_cast<double>(pageFaults) / totalReferences) * 100;
}

/**
 * Implements the FIFO (First-In-First-Out) page replacement algorithm.
 *
 * @param pageNumbers A vector containing the sequence of page references.
 * @param frameSize The number of frames in the memory.
 * @return The number of page faults occurred during execution.
 */
int fifo(const vector<int>& pageNumbers, int frameSize) {
    list<int> frames;
    int pageFaults = 0;

    for (vector<int>::size_type i = 0; i < pageNumbers.size(); ++i) {
        auto it = find(frames.begin(), frames.end(), pageNumbers[i]);

        if (it == frames.end()) {
            if (frames.size() == static_cast<size_t>(frameSize)) {
                frames.pop_back();
            }
            frames.push_front(pageNumbers[i]);
            pageFaults++;
        }
    }
    return pageFaults;
}

/**
 * Implements the LRU (Least Recently Used) page replacement algorithm.
 *
 * @param pageNumbers A vector containing the sequence of page references.
 * @param frameSize The number of frames in the memory.
 * @return The number of page faults occurred during execution.
 */
int lru(const vector<int>& pageNumbers, int frameSize) {
    list<int> frames;
    unordered_map<int, list<int>::iterator> pageMap;
    int pageFaults = 0;

    for (vector<int>::size_type i = 0; i < pageNumbers.size(); ++i) {
        auto it = pageMap.find(pageNumbers[i]);

        if (it == pageMap.end()) {
            if (frames.size() == static_cast<size_t>(frameSize)) {
                pageMap.erase(frames.back());
                frames.pop_back();
            }
            frames.push_front(pageNumbers[i]);
            pageMap[pageNumbers[i]] = frames.begin();
            pageFaults++;
        } else {
            frames.erase(it->second);
            frames.push_front(pageNumbers[i]);
            pageMap[pageNumbers[i]] = frames.begin();
        }
    }
    return pageFaults;
}

/**
 * Implements the MRU (Most Recently Used) page replacement algorithm.
 *
 * @param pageNumbers A vector containing the sequence of page references.
 * @param frameSize The number of frames in the memory.
 * @return The number of page faults occurred during execution.
 */
int mru(const vector<int>& pageNumbers, int frameSize) {
    list<int> frames;
    unordered_map<int, list<int>::iterator> pageMap;
    int pageFaults = 0;

    for (vector<int>::size_type i = 0; i < pageNumbers.size(); ++i) {
        auto it = pageMap.find(pageNumbers[i]);

        if (it == pageMap.end()) {
            if (frames.size() == static_cast<size_t>(frameSize)) {
                pageMap.erase(frames.front());
                frames.pop_front();
            }
            frames.push_back(pageNumbers[i]);
            pageMap[pageNumbers[i]] = --frames.end();
            pageFaults++;
        } else {
            frames.erase(it->second);
            frames.push_back(pageNumbers[i]);
            pageMap[pageNumbers[i]] = --frames.end();
        }
    }
    return pageFaults;
}

/**
 * Implements the Optimal page replacement algorithm.
 *
 * @param pageNumbers A vector containing the sequence of page references.
 * @param frameSize The number of frames in the memory.
 * @return The number of page faults occurred during execution.
 */
int optimal(const vector<int>& pageNumbers, int frameSize) {
    list<int> frames;
    unordered_map<int, int> nextPage;
    int pageFaults = 0;

    for (vector<int>::size_type i = 0; i < pageNumbers.size(); ++i) {
        auto it = find(frames.begin(), frames.end(), pageNumbers[i]);

        if (it == frames.end()) {
            if (frames.size() == static_cast<size_t>(frameSize)) {
                int farthest = -1, idx = -1;
                for (auto iter = frames.begin(); iter != frames.end(); ++iter) {
                    if (nextPage[*iter] == -1) {
                        idx = distance(frames.begin(), iter);
                        break;
                    } else {
                        if (nextPage[*iter] > farthest) {
                            farthest = nextPage[*iter];
                            idx = distance(frames.begin(), iter);
                        }
                    }
                }
                auto it = frames.begin();
                advance(it, idx);
                frames.erase(it);
            }
            frames.push_front(pageNumbers[i]);
            pageFaults++;
        }

        nextPage[pageNumbers[i]] = i;
    }
    return pageFaults;
}

/**
 * Runs experiments with different parameters for page replacement algorithms.
 *
 * @param inputFile The name of the input file containing page references.
 * @param pageSize The page size.
 * @param numFrames The number of frames in the memory.
 */
void runExperiments(const string& inputFile, int pageSize, int numFrames) {
    vector<int> pageNumbers = readPageNumbers(inputFile);
    int numPages = pageNumbers.size();
    cout << "Page Size: " << pageSize << ", Number of Frames: " << numFrames << endl;
    cout << "--------------------------------------------" << endl;
    cout << "Algorithm\tPage Fault Percentage" << endl;
    cout << "--------------------------------------------" << endl;

    double pageFaultPercentage;

    // FIFO
    int fifoFaults = fifo(pageNumbers, numFrames);
    pageFaultPercentage = calculatePageFaultRate(fifoFaults, numPages);
    cout << "FIFO\t\t" << fixed << setprecision(2) << pageFaultPercentage << "%" << endl;

    // LRU
    int lruFaults = lru(pageNumbers, numFrames);
    pageFaultPercentage = calculatePageFaultRate(lruFaults, numPages);
    cout << "LRU\t\t" << fixed << setprecision(2) << pageFaultPercentage << "%" << endl;

    // MRU
    int mruFaults = mru(pageNumbers, numFrames);
    pageFaultPercentage = calculatePageFaultRate(mruFaults, numPages);
    cout << "MRU\t\t" << fixed << setprecision(2) << pageFaultPercentage << "%" << endl;

    // Optimal
    int optimalFaults = optimal(pageNumbers, numFrames);
    pageFaultPercentage = calculatePageFaultRate(optimalFaults, numPages);
    cout << "Optimal\t\t" << fixed << setprecision(2) << pageFaultPercentage << "%" << endl;

    cout << endl;
}

/**
 * The main function that runs experiments with different parameters.
 *
 * @return 0 on success.
 */
int main() {
    runExperiments("SampleInput.txt", 512, 4);
    runExperiments("SampleInput.txt", 512, 12);
    runExperiments("SampleInput.txt", 2048, 4);
    runExperiments("SampleInput.txt", 2048, 12);

    return 0;
}
