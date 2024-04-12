# Producer-Consumer Problem Simulation Report

## Executive Summary

This report presents the findings from a simulation addressing the Producer-Consumer Problem, aiming to emulate the interaction between producers generating sales records and consumers computing sales statistics in a multi-threaded environment. The simulation encompassed variations in the number of producers and consumers, with careful consideration given to synchronization mechanisms and shared resource management.

## Experiment Details

- **Objective**: To analyze the efficiency and performance characteristics of a producer-consumer system handling sales data.
- **Experimental Parameters**:
  - Number of Producers (p): 2 and 10
  - Number of Consumers (c): 2 and 10
  - Total Sales Records Generated: 500
- **Shared Resources**: Shared buffer for sales records, count of records, and a special flag indicating completion.
- **Synchronization Mechanisms**: Semaphores utilized for mutual exclusion and coordination.

## Key Observations

1. **Overall Performance**
   - Total Time: 5.81432 seconds
   - Aggregate Sales: $256611

2. **Store-wise Total Sales**
   - Varied sales distribution across stores, reflecting distinct market dynamics.

3. **Month-wise Total Sales**
   - Monthly sales fluctuations observed, indicating potential seasonal trends.

4. **Consumer-wise Statistics**
   - Diverse sales analysis performed by individual consumers, highlighting unique perspectives on data interpretation.

## Shared Resource Management

- **Buffer Utilization**: Effective utilization of shared buffer for seamless data exchange between producers and consumers.
- **Count of Records**: Shared variable tracking the production progress, essential for termination signaling.
- **Special Flag**: Indicator for completion of sales record generation, facilitating orderly termination of the simulation.

## Recommendations for Optimization

- Fine-tuning of parameters such as sleep duration and buffer size to enhance system efficiency.
- Exploration of additional metrics (e.g., average sales per store, month) for deeper insights into sales dynamics.

## Conclusion

The simulation successfully illustrated the intricacies of managing shared data in a concurrent environment, offering valuable insights into the performance implications of varying producer-consumer configurations. Further refinements and optimizations hold the potential to elevate the system's effectiveness, ensuring streamlined processing of sales data in retail environments.




