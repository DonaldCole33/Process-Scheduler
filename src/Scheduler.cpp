/*
 * Scheduler.cpp
 *
 *  Created on: Mar 26, 2016
 *      Author: doncole
 */

#include "Scheduler.h"

namespace std {

Scheduler::Scheduler(char* fileName) {
		//initialize Data Structures
		this->_processDataFile.open(fileName);
		if (!_processDataFile.is_open()){
			cout << "Error Opening File: " << fileName << endl;
			exit(0);
		}
		ParseProcessFile();		//Parses Process file and adds process to queue
	}

Scheduler::Scheduler(){}

Scheduler::~Scheduler() {
	// TODO Auto-generated destructor stub
}

void Scheduler::ParseProcessFile(void){
	string line;		//to get each line from the file

	while(getline(this->_processDataFile, line, ';')){
		Process* temp = GetNewProcessStruct();
		this->ParseLine(line, temp);		//Get the info for the Process
		this->push_back(*temp);
	}
}

void Scheduler::ParseLine(string line, Process* process){
	int found;

	if (strcmp(&line[0], "P") == 0){
		cout << "Error Parsing Process File" << endl;
	}

	found = line.find('P');
	process->ID = atoi(&line.at(found+1));
	found = line.find(',', found+1);
	process->arrivalTime = atoi(&line.at(found+1));
	found = line.find(',', found+1);
	process->totalCpuTimeNeeded = atoi(&line.at(found+1));
	found = line.find('(', found+1);
	found++;
	while ( strcmp(&line.at(found), ")") != 0 ){			//We have an IO Time
		process->ioList->push((atoi(&line.at(found++))));
		process->numberofIOEvents++;
		found = line.find(',', found);
		if ( found == -1 ){									//No more IO
			found = line.find(')');
			break;
		}
		found++;
	}

	process->priority = atoi(&line.at(found+1));
}

Process* Scheduler::GetNewProcessStruct(){
	Process* tempProcess = new Process();
	tempProcess->ioList = new queue<int>();
	tempProcess->numberofIOEvents = 0;
	tempProcess->totalCpuTimeNeeded = 0;
	tempProcess->finishedTime = 0;
	tempProcess->totalProcessedTime = 0;
	tempProcess->totalWaitingTime = 0;
	tempProcess->turnaroundTime = 0;
	return tempProcess;
}

int Scheduler::GetTotalTurnAroundTime(){
	int totalTurnAroundTime = 0;

	for (std::list<Process>::iterator it = this->begin(); it != this->end(); it++){
		totalTurnAroundTime += it->turnaroundTime;
	}

	return totalTurnAroundTime;
}

int Scheduler::GetTotalAverageWaitingTime(){
	int totalWaitingTime = 0;

	for (std::list<Process>::iterator it = this->begin(); it != this->end(); it++){
		totalWaitingTime += it->totalWaitingTime;
	}

	return totalWaitingTime / this->size();
}

} /* namespace std */
