#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>

using namespace std;

int burstTime[100], arrivalTime[100], waitingTime[100], turnaroundTime[100], processID[100];
int numProcesses;
float avgWaitingTime[5], avgTurnaroundTime[5], cpuIdleTime[5];
vector<int> startTimes, endTimes;
vector<string> stateDescriptions;

void inputProcesses() {
    int mode;
    cout << "Select mode (0 for offline, 1 for online): ";
    cin >> mode;

    if (mode == 0) {
        cout << "Enter number of processes: ";
        cin >> numProcesses;
        vector<pair<pair<int, int>, int>> processes(numProcesses);

        for (int i = 0; i < numProcesses; i++) {
            processID[i] = i + 1;
            cout << "Enter burst time for process " << i + 1 << ": ";
            cin >> burstTime[i];
            cout << "Enter arrival time for process " << i + 1 << ": ";
            cin >> arrivalTime[i];
            processes[i] = {{arrivalTime[i], burstTime[i]}, i + 1};
        }

        sort(processes.begin(), processes.end());

        for (int i = 0; i < numProcesses; i++) {
            processID[i] = processes[i].second;
            burstTime[i] = processes[i].first.second;
            arrivalTime[i] = processes[i].first.first;
        }

    } else {
        int totalTime;
        cout << "Enter the total arrival time: ";
        cin >> totalTime;

        for (int t = 0; t <= totalTime; t++) {
            int newProcesses;
            cout << "Number of processes arriving at time " << t << ": ";
            cin >> newProcesses;
            numProcesses += newProcesses;

            for (int j = 0; j < newProcesses; j++) {
                int pid;
                cout << "Enter process ID: ";
                cin >> pid;
                cout << "Enter burst time for process " << pid << ": ";
                cin >> burstTime[pid - 1];
                arrivalTime[pid - 1] = t;
            }
        }

        vector<pair<pair<int, int>, int>> processes(numProcesses);
        for (int i = 0; i < numProcesses; i++) {
            processes[i] = {{arrivalTime[i], burstTime[i]}, i + 1};
        }

        sort(processes.begin(), processes.end());

        for (int i = 0; i < numProcesses; i++) {
            processID[i] = processes[i].second;
            burstTime[i] = processes[i].first.second;
            arrivalTime[i] = processes[i].first.first;
        }
    }

    for (int i = 0; i < 5; i++) {
        avgWaitingTime[i] = 0.0;
        avgTurnaroundTime[i] = 0.0;
    }
}

void displayResults(int algorithmIndex) {
    cout << "Average Waiting Time: " << avgWaitingTime[algorithmIndex - 1] << "\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime[algorithmIndex - 1] << "\n";
    cout << "CPU Idle Time: " << cpuIdleTime[algorithmIndex - 1] << "\n";
}

void firstComeFirstServe() {
    startTimes.clear();
    endTimes.clear();
    stateDescriptions.clear();

    int currentTime = arrivalTime[0] + burstTime[0];
    cpuIdleTime[0] = arrivalTime[0];

    if (arrivalTime[0] > 0) {
        startTimes.push_back(0);
        endTimes.push_back(arrivalTime[0]);
        stateDescriptions.push_back("Idle");
    }

    startTimes.push_back(arrivalTime[0]);
    endTimes.push_back(currentTime);
    stateDescriptions.push_back("P[" + to_string(processID[0]) + "]");

    waitingTime[0] = 0;
    turnaroundTime[0] = burstTime[0];
    avgWaitingTime[0] = waitingTime[0];
    avgTurnaroundTime[0] = turnaroundTime[0];

    for (int i = 1; i < numProcesses; i++) {
        if (arrivalTime[i] <= currentTime) {
            startTimes.push_back(currentTime);
            waitingTime[i] = currentTime - arrivalTime[i];
            currentTime += burstTime[i];
            endTimes.push_back(currentTime);
            stateDescriptions.push_back("P[" + to_string(processID[i]) + "]");
            avgWaitingTime[0] += waitingTime[i];
            turnaroundTime[i] = waitingTime[i] + burstTime[i];
            avgTurnaroundTime[0] += turnaroundTime[i];
        } else {
            startTimes.push_back(currentTime);
            endTimes.push_back(arrivalTime[i]);
            stateDescriptions.push_back("Idle");
            cpuIdleTime[0] += arrivalTime[i] - currentTime;
            startTimes.push_back(arrivalTime[i]);
            currentTime = arrivalTime[i] + burstTime[i];
            endTimes.push_back(currentTime);
            stateDescriptions.push_back("P[" + to_string(processID[i]) + "]");
            waitingTime[i] = 0;
            avgWaitingTime[0] += waitingTime[i];
            turnaroundTime[i] = burstTime[i];
            avgTurnaroundTime[0] += turnaroundTime[i];
        }
    }

    avgTurnaroundTime[0] /= numProcesses;
    avgWaitingTime[0] /= numProcesses;

    cout << "Process ID\tArrival Time\tCompletion Time\tBurst Time\tWaiting Time\tTurnaround Time\n";
    for (int i = 0; i < numProcesses; i++) {
        cout << processID[i] << "\t\t" << arrivalTime[i] << "\t\t" << (arrivalTime[i] + turnaroundTime[i]) << "\t\t" << burstTime[i] << "\t\t" << waitingTime[i] << "\t\t" << turnaroundTime[i] << "\n";
    }

    cout << "Start Times: ";
    for (int t : startTimes) {
        cout << t << "\t";
    }
    cout << "\nEnd Times: ";
    for (int t : endTimes) {
        cout << t << "\t";
    }
    cout << "\nStates: ";
    for (const string& state : stateDescriptions) {
        cout << state << "\t";
    }
    cout << "\n\n";
}

int calculateMedian(int bt[]) {
    vector<int> temp;
    for (int i = 0; i < numProcesses; i++) {
        if (bt[i] > 0) {
            temp.push_back(bt[i]);
        }
    }
    sort(temp.begin(), temp.end());
    int size = temp.size();
    if (size % 2 == 0) {
        return (temp[size / 2 - 1] + temp[size / 2]) / 2;
    } else {
        return temp[size / 2];
    }
}

void innovativeScheduling() {
    startTimes.clear();
    endTimes.clear();
    stateDescriptions.clear();
    int count = 0, currentTime = 0, minBurstIndex = 0;
    int totalWaitTime = 0, totalTurnaroundTime = 0;
    vector<int> remainingBurstTime(numProcesses);
    for (int i = 0; i < numProcesses; i++) {
        remainingBurstTime[i] = burstTime[i];
    }

    while (count < numProcesses) {
        int minBurstTime = INT_MAX;
        int minArrivalTime = INT_MAX;
        bool found = false;

        for (int i = 0; i < numProcesses; i++) {
            if (arrivalTime[i] <= currentTime && remainingBurstTime[i] > 0) {
                found = true;
                if (remainingBurstTime[i] < minBurstTime) {
                    minBurstTime = remainingBurstTime[i];
                    minBurstIndex = i;
                }
            }
        }

        if (!found) {
            for (int i = 0; i < numProcesses; i++) {
                if (remainingBurstTime[i] > 0 && arrivalTime[i] < minArrivalTime) {
                    minArrivalTime = arrivalTime[i];
                }
            }
            startTimes.push_back(currentTime);
            stateDescriptions.push_back("Idle");
            cpuIdleTime[4] += minArrivalTime - currentTime;
            currentTime = minArrivalTime;
            endTimes.push_back(currentTime);
            continue;
        }

        int timeQuantum = calculateMedian(remainingBurstTime.data());

        if (remainingBurstTime[minBurstIndex] <= timeQuantum) {
            startTimes.push_back(currentTime);
            currentTime += remainingBurstTime[minBurstIndex];
            endTimes.push_back(currentTime);
            stateDescriptions.push_back("P[" + to_string(processID[minBurstIndex]) + "]");
            remainingBurstTime[minBurstIndex] = 0;
            count++;
            cout << "Process[" << processID[minBurstIndex] << "]\t\t" << arrivalTime[minBurstIndex] << "\t\t" << burstTime[minBurstIndex] << "\t\t" << currentTime - arrivalTime[minBurstIndex] << "\t\t" << currentTime - arrivalTime[minBurstIndex] - burstTime[minBurstIndex] << "\n";
            totalWaitTime += currentTime - arrivalTime[minBurstIndex] - burstTime[minBurstIndex];
            totalTurnaroundTime += currentTime - arrivalTime[minBurstIndex];
        } else {
            startTimes.push_back(currentTime);
            currentTime += timeQuantum;
            endTimes.push_back(currentTime);
            stateDescriptions.push_back("P[" + to_string(processID[minBurstIndex]) + "]");
            remainingBurstTime[minBurstIndex] -= timeQuantum;
        }
    }

    avgWaitingTime[4] = (float)totalWaitTime / numProcesses;
    avgTurnaroundTime[4] = (float)totalTurnaroundTime / numProcesses;
    cout << "Average Waiting Time: " << avgWaitingTime[4] << "\n";
    cout << "Average Turnaround Time: " << avgTurnaroundTime[4] << "\n";
    cout << "CPU Idle Time: " << cpuIdleTime[4] << "\n";
}

void printMenu() {
    cout << "Choose the scheduling algorithm to test:\n";
    cout << "1. First Come First Serve (FCFS)\n";
    cout << "2. Shortest Job First (SJF)\n";
    cout << "3. Round Robin (RR)\n";
    cout << "4. Priority Scheduling\n";
    cout << "5. Innovative Scheduling Algorithm\n";
    cout << "0. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    inputProcesses();

    while (true) {
        printMenu();
        int choice;
        cin >> choice;

        switch (choice) {
            case 1:
                firstComeFirstServe();
                displayResults(1);
                break;
            case 5:
                innovativeScheduling();
                displayResults(5);
                break;
            case 0:
                return 0;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }
    return 0;
}
