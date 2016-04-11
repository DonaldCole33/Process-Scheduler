/*
 * Scheduler.h
 *
 *  Created on: Mar 26, 2016
 *      Author: doncole
 */

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

#include <queue>
#include <list>
#include <fstream>
#include <string>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <cstring>

class Process{
public:
	long arrivalTime;
	unsigned long ID;
	long totalProcessedTime;
	long totalCpuTimeNeeded;
	long responseTime;
	long turnaroundTime;
	long finishedTime;
	long totalWaitingTime;
	std::queue<int>* ioList;
	long numberofIOEvents;
	//std::list<int>* ioList;
	long priority;
	void CalculateTurnAroundTime() {
		turnaroundTime = finishedTime - arrivalTime;
	}
};

namespace std {

class Scheduler : public std::list<Process> {
public:
	Scheduler(const char* fileName);
	Scheduler();
	virtual ~Scheduler();
	double GetTotalTurnAroundTime();
	double GetTotalAverageWaitingTime();

private:
	ifstream _processDataFile;
	void ParseProcessFile(void);
	void ParseLine(string line, Process* process);
	Process* GetNewProcessStruct(void);
};

} /* namespace std */

#endif /* SCHEDULER_H_ */
