#include <iostream>
#include <queue>
#include <vector>
#include <algorithm>

using namespace std;

struct Process {
    char id;        
    int at;         
    int bt;         
    int rt;         
    int ct;         
    int tat;        
    int wt;         
};

void printProcesses(vector<Process> &processes) {
    cout << "\nP\tAT\tBT\tCT\tTAT\tWT\n";
    for (auto &p : processes) {
        cout << p.id << "\t" << p.at << "\t" << p.bt << "\t"
             << p.ct << "\t" << p.tat << "\t" << p.wt << "\n";
    }
}

int main() {
    int n, time = 0;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);
    queue<Process> L1, L2, L3;

    cout << "Enter Arrival Time and Burst Time for each process:\n";
    for (int i = 0; i < n; i++) {
        processes[i].id = 'A' + i;  
        cout << processes[i].id << " AT BT: ";
        cin >> processes[i].at >> processes[i].bt;
        processes[i].rt = processes[i].bt;
    }

    sort(processes.begin(), processes.end(), [](Process &a, Process &b) {
        return a.at < b.at;
    });

    int index = 0, completed = 0, tq1 = 2, tq2 = 4;
    vector<char> gantt;

    while (completed < n) {
        while (index < n && processes[index].at <= time) {
            L1.push(processes[index]);
            index++;
        }

        if (!L1.empty()) {
            Process curr = L1.front();
            L1.pop();
            gantt.push_back(curr.id);

            if (curr.rt > tq1) {
                curr.rt -= tq1;
                time += tq1;
                while (index < n && processes[index].at <= time) {
                    L1.push(processes[index]);
                    index++;
                }
                L2.push(curr); 
            } else {
                time += curr.rt;
                curr.rt = 0;
                curr.ct = time;
                curr.tat = curr.ct - curr.at;
                curr.wt = curr.tat - curr.bt;
                processes[curr.id - 'A'] = curr;
                completed++;
            }
        }
        else if (!L2.empty()) {
            Process curr = L2.front();
            L2.pop();
            gantt.push_back(curr.id);

            if (curr.rt > tq2) {
                curr.rt -= tq2;
                time += tq2;
                while (index < n && processes[index].at <= time) {
                    L1.push(processes[index]);
                    index++;
                }
                L3.push(curr); 
            } else {
                time += curr.rt;
                curr.rt = 0;
                curr.ct = time;
                curr.tat = curr.ct - curr.at;
                curr.wt = curr.tat - curr.bt;
                processes[curr.id - 'A'] = curr;
                completed++;
            }
        }
        else if (!L3.empty()) {
            Process curr = L3.front();
            L3.pop();
            gantt.push_back(curr.id);

            time += curr.rt;
            curr.rt = 0;
            curr.ct = time;
            curr.tat = curr.ct - curr.at;
            curr.wt = curr.tat - curr.bt;
            processes[curr.id - 'A'] = curr;
            completed++;
        }
        else {
            time++;
        }
    }

    printProcesses(processes);

    cout << "\nGantt Chart:\n";
    cout << "| ";
    for (char pid : gantt) {
        cout << pid << " | ";
    }
    cout << "\n0 ";
    int timeMarker = 0;
    for (char pid : gantt) {
        timeMarker += 2;  
        cout << timeMarker << " ";
    }
    cout << "\n";

    double totalTAT = 0, totalWT = 0;
    for (auto &p : processes) {
        totalTAT += p.tat;
        totalWT += p.wt;
    }
    cout << "\nAverage Turnaround Time: " << totalTAT / n;
    cout << "\nAverage Waiting Time: " << totalWT / n;

    cout << "\n\nCPU Utilization: " << ((double)time / time) * 100 << "%\n";

    return 0;
}
    
