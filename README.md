# CPU Scheduler

## Overview

A CPU scheduler is a critical component of an operating system that manages process execution with the goals of:

- Maximizing CPU usage
- Enhancing throughput
- Reducing waiting time
- Lowering turnaround time
- Minimizing response time
- Ensuring fairness
- Prioritizing tasks effectively

This project implements four standard scheduling algorithms and introduces one innovative algorithm.

## Scheduling Algorithms

### 1. First Come First Serve (FCFS)

#### Description

The FCFS (First-Come, First-Served) algorithm schedules processes in the order they arrive.

#### Features

- **Non-preemptive**: Processes run to completion once started.
- **Queue Structure**: Processes are managed using a FIFO (First-In, First-Out) queue.
- **Performance**: May lead to the "convoy effect," increasing average waiting times.
- **Context Switches**: Minimal, as processes are not interrupted.
- **Use Case**: Suitable for batch systems with known execution times and where processes can wait.

#### Example
For three processes with burst times:

- P1: 24 ms
- P2: 3 ms
- P3: 3 ms

IArriving in order P1, P2, P3, the Gantt chart will be:
Average waiting time = (0 + 24 + 27) / 3 = 17 ms

### 2. Shortest Job First (SJF)

#### Description
The SJF (Shortest Job First) algorithm selects the process with the shortest execution time next.

#### Features
- **Types**:
  - **Non-preemptive**: The process runs to completion without interruption.
  - **Preemptive (Shortest Remaining Time First, SRTF)**: A new process with a shorter burst time can preempt the current one.
- **Optimality**: Minimizes average waiting time.
- **Queue Structure**: Managed based on burst times.
- **Performance**: Can cause "starvation" for longer processes.
- **Context Switches**: More frequent in preemptive SJF.
- **Use Case**: Suitable where burst times can be estimated accurately.

#### Example

For four processes with burst times:

- P1: 6 ms
- P2: 8 ms
- P3: 7 ms
- P4: 3 ms

Arriving in order P1, P2, P3, P4, the Gantt chart for non-preemptive SJF will be:
Average waiting time = (0 + 3 + 9 + 16) / 4 = 7 ms

### 3. Round Robin (RR)

#### Description
The Round Robin (RR) algorithm assigns a fixed time slice or quantum to each process, rotating through them.

#### Features

- **Preemptive**: Processes are interrupted when their time slice expires.
- **Time Quantum**: Critical; too short increases context switching, too long behaves like FCFS.
- **Queue Structure**: Managed in a circular queue.
- **Fairness**: Each process gets an equal share of CPU time.
- **Performance**: Good response time but higher average waiting time.
- **Context Switches**: Frequent if the time quantum is too short.
- **Use Case**: Suitable for time-sharing systems where responsiveness is critical.

#### Example

For three processes with burst times and a time quantum of 4 ms:

- P1: 6 ms
- P2: 8 ms
- P3: 7 ms

The Gantt chart will be:
Average waiting time:

- P1: (0 + 8) - 4 = 4 ms
- P2: (4 + 8 + 4) - 8 = 8 ms
- P3: (8 + 7) - 12 = 3 ms
Total average waiting time = (4 + 8 + 3) / 3 = 5 ms

### 4. Innovative Algorithm

#### Description

This innovative algorithm aims to significantly reduce waiting and turnaround times compared to standard algorithms, including Round Robin.

#### Approach

- Executes the shortest job first.
- Uses a dynamic "Smart time quantum" based on the median burst time of all processes.

#### Steps

1. **Arrange**: Processes in increasing order of burst time.
2. **Calculate**: Median burst time; set as the Smart time quantum.
3. **Execute**: Allocate CPU based on the Smart time quantum until completion.

#### Benefits

- **Reduced Waiting Time**: Prioritizes shorter jobs and dynamically adjusts the time quantum.
- **Improved Turnaround Time**: Efficient handling of processes.
- **Eliminates Discrepancies**: Dynamic time quantum addresses issues with static time quanta.

#### Example

Consider four processes with burst times:

- P1: 6 ms
- P2: 8 ms
- P3: 7 ms
- P4: 3 ms

**Stage 1**: Arrange in order: P4, P1, P3, P2  
**Stage 2**: Calculate median = (6 + 7) / 2 = 6.5 (rounded to 6 ms)  
**Stage 3**: Allocate CPU as per the algorithm:

This innovative CPU scheduling algorithm ensures efficient process management, resulting in better overall performance metrics.

## Running the Scheduler

TThe code is written in C++ and can be executed through the terminal.

## Resources

- [YouTube Playlist on CPU Scheduling](https://www.youtube.com/playlist?list=PLBlnK6fEyqRitWSE_AyyySWfhRgyA-rHk)
- [Documentation](https://ravipatel1309.github.io/CPUScheduler/docs.html)
- [Research on Improved Round Robin Scheduling Algorithm](https://www.researchgate.net/publication/49619229_An_Improved_Round_Robin_Schedduling_Algorithm_for_CPU_Scheduling)
