# Virtual Memory Management Simulation

## Overview

This program simulates virtual memory management techniques using various page replacement algorithms. It evaluates the performance of FIFO, LRU, MRU, and Optimal algorithms based on provided virtual addresses.

## Features

- Simulates FIFO, LRU, MRU, and Optimal page replacement algorithms.
- Evaluates page fault percentage for different combinations of page size and number of frames.
- Provides clear output statistics for each algorithm and parameter combination.

## Usage

### Prerequisites
- C++ compiler (supporting C++11)
- `make` utility

### Building the Program
1. Clone this repository.
2. Navigate to the project directory.
3. Run the following command to compile the code:
    ```bash
    make
    ```

### Running the Program
1. Ensure that the `SampleInput.txt` file containing virtual addresses is present in the project directory.
2. Run the compiled executable:
    ```bash
    ./sales_system SampleInput.txt
    ```

### Cleaning Up
To remove the compiled executable and object files, run:
```bash
make clean 
```

### Sample Input File
- You can download the sample input file [here](/home/cs_hpate021/Cs471/CS471---CourseProject/VMEMMAN/SampleInput.txt).

### Sample Output
```--------------------------------------------------
           Virtual Memory Management          
--------------------------------------------------
  Page Size: 512 Words, Number of Frames: 4
--------------------------------------------------
Algorithm           |  Page Fault Percentage
--------------------|-----------------------------
FIFO                |  80.37%
LRU                 |  80.00%
MRU                 |  93.10%
Optimal             |  90.00%


--------------------------------------------------
           Virtual Memory Management          
--------------------------------------------------
  Page Size: 512 Words, Number of Frames: 12
--------------------------------------------------
Algorithm           |  Page Fault Percentage
--------------------|-----------------------------
FIFO                |  42.97%
LRU                 |  42.07%
MRU                 |  88.97%
Optimal             |  81.30%


--------------------------------------------------
           Virtual Memory Management          
--------------------------------------------------
  Page Size: 2048 Words, Number of Frames: 4
--------------------------------------------------
Algorithm           |  Page Fault Percentage
--------------------|-----------------------------
FIFO                |  26.67%
LRU                 |  26.03%
MRU                 |  73.40%
Optimal             |  71.63%


--------------------------------------------------
           Virtual Memory Management          
--------------------------------------------------
  Page Size: 2048 Words, Number of Frames: 12
--------------------------------------------------
Algorithm           |  Page Fault Percentage
--------------------|-----------------------------
FIFO                |  1.83%
LRU                 |  1.83%
MRU                 |  60.40%
Optimal             |  41.10%

