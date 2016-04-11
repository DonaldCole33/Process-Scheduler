//============================================================================
// Name        : Processes-Program.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include "Scheduler.h"

using namespace std;

/*	Find out if the first integer is Smaller than the second
 * integer.  If true will return true, if the same will return true.
 * if second is bigger than first, will return false.
 */
bool CompareCPUTimeNeeded (const Process first, const Process second){

	if (first.totalCpuTimeNeeded <= second.totalCpuTimeNeeded){
		return true;
	}

	return false;
}

bool CompareArrivalTime (const Process first, const Process second){

	if (first.arrivalTime <= second.arrivalTime){
		return true;
	}

	return false;
}

void CheckArrivingProcesses(Scheduler *scheduler, queue<Process> *readyQueue, int currentTick) {

	while (!scheduler->empty() && scheduler->front().arrivalTime <= currentTick) {			//Add processes arrived to ready Queue
		readyQueue->push(scheduler->front());
		scheduler->pop_front();
	}
}

void CheckArrivingProcesses(Scheduler *scheduler, list<Process> *readyQueue, int currentTick) {

	while (!scheduler->empty() && scheduler->front().arrivalTime <= currentTick) {			//Add processes arrived to ready Queue
		readyQueue->push_back(scheduler->front());
		scheduler->pop_front();
	}
}

/*  First Come, First Serve Scheduler
* This function will take in a queue of processes, sorted by the arrival time
* needed and schedule each process by that time.  The processes will be put
* into a seperate ready queue, just as if they arrived in a real Operating System.
* The function will check each time slice for when a process has arrived.
* If nothing has arrived it will continue to schedule the processes as normal.
*/
void FCFS(Scheduler *scheduler) {
	queue<Process> *readyQueue = new queue<Process>();
	queue<Process> *finishedQueue = new queue<Process>();
	Scheduler *finishedScheduler = new Scheduler();
	int totalTicks = 0;
	int currentTicks = 0;
	int ioTime = 0;
	Process *runningProcess;

	while (!scheduler->empty() || !readyQueue->empty()) {			//Queues Not Empty

		CheckArrivingProcesses(scheduler, readyQueue, totalTicks);	//get arriving Processes

																	//Service the Ready Queue
		if (!readyQueue->empty()) {
			runningProcess = &readyQueue->front();			//get the RunningProcess
			readyQueue->pop();
			runningProcess->totalWaitingTime += totalTicks - runningProcess->arrivalTime;

//			if(runningProcess->ID == 290){
//				cout << "Running Process ID: " << runningProcess->ID;
//				cout << " Time Left: "<< runningProcess->totalCpuTimeNeeded - runningProcess->totalProcessedTime << endl;
//			}

			for (currentTicks = 0; runningProcess != nullptr; currentTicks++) {		//While Process is alive
				if (runningProcess->numberofIOEvents > 0 && ioTime == 0) {
					ioTime = runningProcess->ioList->front();
					runningProcess->ioList->pop();
					runningProcess->numberofIOEvents -= 1;
				}

				if (ioTime > currentTicks) {
					runningProcess->arrivalTime = (totalTicks + currentTicks + 5);		//Set new Arrival Time & add back to queue
					runningProcess->totalProcessedTime += currentTicks;
					readyQueue->push(*runningProcess);
					runningProcess = nullptr;
				}
				else if ((runningProcess->totalProcessedTime + currentTicks) == runningProcess->totalCpuTimeNeeded) {		//Time is Up for this Process
					runningProcess->totalProcessedTime = runningProcess->totalCpuTimeNeeded;
					runningProcess->finishedTime = currentTicks + totalTicks;
					runningProcess->CalculateTurnAroundTime();
					finishedScheduler->push_back(*runningProcess);
					finishedQueue->push(*runningProcess);
					cout << "Finished Queue -> " << runningProcess->ID << " total = " << finishedQueue->size() << endl;
					runningProcess = nullptr;
				}

				CheckArrivingProcesses(scheduler, readyQueue, currentTicks + totalTicks);	//get arriving Processes
			}
			ioTime = 0;			//Reset IO Time
			totalTicks += currentTicks;		//Update TotalTicks
		}
		else {
			totalTicks++;
		}
	}

	cout << "The Total Average Waiting Time for First Come First Served is..." << endl;
	cout << finishedScheduler->GetTotalAverageWaitingTime() << endl;
	cout << "The Total Turn Around Time for First Come First Served is..." << endl;
	cout << finishedScheduler->GetTotalTurnAroundTime() << endl;

}

/*  Shortest Job First Scheduler
* This function will take in a queue of processes, sorted by the total CPU time
* needed and schedule each process by that time.  The processes will be put
* into a seperate ready queue, just as if they arrived in a real Operating System.
* The function will check each time slice for when a process has arrived.
* If nothing has arrived it will continue to schedule the processes as normal.
*/
void SJFS(Scheduler *scheduler) {
	list<Process> *readyQueue = new list<Process>();
	Scheduler *finishedScheduler = new Scheduler();
	int totalTicks = 0;
	int currentTicks = 0;
	int ioTime = 0;
	Process *runningProcess;

	while (!scheduler->empty() || !readyQueue->empty()) {			//Queues Not Empty

		CheckArrivingProcesses(scheduler, readyQueue, totalTicks);	//get arriving Processes
		readyQueue->sort(CompareCPUTimeNeeded);						//Sort the Queue by shortest job

		if (!readyQueue->empty()) {							//Service the Ready Queue
			runningProcess = &readyQueue->front();			//get the RunningProcess
			readyQueue->pop_front();
			runningProcess->totalWaitingTime += totalTicks - runningProcess->arrivalTime;

//			if(runningProcess->ID == 290){
//				cout << "Running Process ID: " << runningProcess->ID;
//				cout << " Time Left: "<< runningProcess->totalCpuTimeNeeded - runningProcess->totalProcessedTime << endl;
//			}

			for (currentTicks = 0; runningProcess != nullptr; currentTicks++) {		//While Process is alive
				if (runningProcess->numberofIOEvents > 0 && ioTime == 0) {
					ioTime = runningProcess->ioList->front();
					runningProcess->ioList->pop();
					runningProcess->numberofIOEvents -= 1;
				}

				if (ioTime > currentTicks) {
					runningProcess->arrivalTime = (totalTicks + currentTicks + 5);		//Set new Arrival Time & add back to queue
					runningProcess->totalProcessedTime += currentTicks;
					readyQueue->push_back(*runningProcess);
					runningProcess = nullptr;
				}
				else if ((runningProcess->totalProcessedTime + currentTicks) == runningProcess->totalCpuTimeNeeded) {		//Time is Up for this Process
					runningProcess->totalProcessedTime = runningProcess->totalCpuTimeNeeded;
					runningProcess->finishedTime = currentTicks + totalTicks;
					runningProcess->CalculateTurnAroundTime();
					finishedScheduler->push_back(*runningProcess);
					cout << "Finished Queue -> " << runningProcess->ID << " total = " << finishedScheduler->size() << endl;
					runningProcess = nullptr;
				}

				CheckArrivingProcesses(scheduler, readyQueue, currentTicks + totalTicks);	//get arriving Processes
				readyQueue->sort(CompareCPUTimeNeeded);		//Sort the Queue by shortest Job first
			}
			ioTime = 0;			//Reset IO Time
			totalTicks += currentTicks;		//Update TotalTicks
		}
		else {
			totalTicks++;
		}
	}

	cout << "The Total Average Waiting Time for Shortest Job First is..." << endl;
	cout << finishedScheduler->GetTotalAverageWaitingTime() << endl;
	cout << "The Total Turn Around Time for Shortest Job First is..." << endl;
	cout << finishedScheduler->GetTotalTurnAroundTime() << endl;
}

/*  Round Robin Scheduler
* This function will take in a queue of processes, sorted by the arrival time
* needed and schedule each process by that time.  The processes will be put
* into a seperate ready queue, just as if they arrived in a real Operating System.
* The function will service each process from the ready queue after each time slice.
* The function will also check each time slice for when a process has arrived and
* keep the readyQueue sorted by arrival time.  If nothing has arrived it will
* continue to schedule the processes as normal.
*/
void RRS(Scheduler *scheduler, int timeSlice) {
	list<Process> *readyQueue = new list<Process>();
	Scheduler *finishedScheduler = new Scheduler();
	int totalTicks = 0;
	int currentTicks = 0;
	int ioTime = 0;
	Process *runningProcess;

	while (!scheduler->empty() || !readyQueue->empty()) {			//Queues Not Empty

		CheckArrivingProcesses(scheduler, readyQueue, totalTicks);	//get arriving Processes

		if (!readyQueue->empty()) {									//Service the Ready Queue
			runningProcess = &readyQueue->front();					//get the RunningProcess
			readyQueue->pop_front();
			runningProcess->totalWaitingTime += totalTicks - runningProcess->arrivalTime;

			for (currentTicks = 0; runningProcess != nullptr; currentTicks++) {		//While Process is alive
				if (runningProcess->numberofIOEvents > 0 && ioTime == 0) {
					if((runningProcess->ioList->front() - runningProcess->totalProcessedTime)
							< (timeSlice)){
						ioTime = runningProcess->ioList->front() - runningProcess->totalProcessedTime;
						runningProcess->ioList->pop();
						runningProcess->numberofIOEvents -= 1;
					}
				}

				if (ioTime == currentTicks) {
					runningProcess->arrivalTime = (totalTicks + currentTicks + 5);		//Set new Arrival Time & add back to queue
					runningProcess->totalProcessedTime += currentTicks;
					readyQueue->push_back(*runningProcess);
					runningProcess = nullptr;
				}
				else if ((runningProcess->totalProcessedTime + currentTicks) == runningProcess->totalCpuTimeNeeded) {		//Time is Up for this Process
					runningProcess->totalProcessedTime = runningProcess->totalCpuTimeNeeded;
					runningProcess->finishedTime = currentTicks + totalTicks;
					runningProcess->CalculateTurnAroundTime();
					finishedScheduler->push_back(*runningProcess);
					cout << "Finished Queue -> " << runningProcess->ID << " total = " << finishedScheduler->size() << endl;
					runningProcess = nullptr;
				}
				else if (currentTicks == timeSlice - 1){
					runningProcess->totalProcessedTime += timeSlice;
					readyQueue->push_back(*runningProcess);
					runningProcess = nullptr;
				}

				CheckArrivingProcesses(scheduler, readyQueue, currentTicks + totalTicks);	//get arriving Processes
				readyQueue->sort(CompareArrivalTime);
			}
			ioTime = 0;			//Reset IO Time
			totalTicks += currentTicks;		//Update TotalTicks
		}
		else {
			totalTicks++;
		}
	}

	cout << "The Total Average Waiting Time for Round Robin is..." << endl;
	cout << finishedScheduler->GetTotalAverageWaitingTime() << endl;
	cout << "The Total Turn Around Time for Round Robin is..." << endl;
	cout << finishedScheduler->GetTotalTurnAroundTime() << endl;
}


//This program takes no input from the user.
//It will take the data file provided and simulate
//process control in an operating system
int main() {
	//Init all Data Variables
	char* dataFile = "data.txt";
	int rrTimeSlice = 5;				//5 time units for Round Robin Scheduler

	Scheduler *fcfsScheduler = new Scheduler(dataFile);
	Scheduler *sjfScheduler = new Scheduler(dataFile);
	Scheduler *rrScheduler = new Scheduler(dataFile);

	FCFS(fcfsScheduler);
	SJFS(sjfScheduler);
	RRS(rrScheduler, rrTimeSlice);


	return 0;
}







