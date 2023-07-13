#include "UI.h"
#include<iostream>	
#include<fstream>	
#include"Hospital.h"
#include"Event.h"
#include"Arrival.h"
#include"Leave.h"
#include"Stair.h"
//using namespace std;
UI::UI(Hospital* hptr) {
	Hptr = hptr;
				 }

void UI::LoadFile() {
	int numfloors;
	ifstream infile;
	infile.open(InputFileName);// we will add a code to read different file names
	if (infile.is_open()) {
		infile >> numfloors;
		Hptr->CreateFloors(numfloors);
		int var1, var2, var3;
		infile >> var1 >> var2 >> var3;//speedV, speedC, speedP
		Hptr->getVisitorElevator()->setSpeed(var1);
		Hptr->getCargoElevator()->setSpeed(var2);
		Hptr->getPatientElevator()->setSpeed(var3);
		infile >> var1 >> var2 >> var3; //capV, capC, capP
		Hptr->getVisitorElevator()->setCapacity(var1);
		Hptr->getCargoElevator()->setCapacity(var2);
		Hptr->getPatientElevator()->setCapacity(var3);
		infile >> var1 >> var2;	 //S and autoS
		Hptr->setSandautoS(var1, var2);
		int num;
		infile >> num;
		Hptr->readNumEvents = num;
		char event, type;
		int var4, var5;
		for (int i = 0; i<num && infile >> event; i++) {
			Event* newEvent;
			if (event == 'A') {
				infile >> type;
				if (type == 'P') {
					bool contagious;
					//infile >> var1 >> var2 >> var3 >> var4 >> var5;//>> contagious;
					
					//contagious input
					infile >> var1 >> var2 >> var3 >> var4 >> var5>> contagious;
					if (var3 == var4)continue;
					newEvent = new Arrival(Hptr, type, var1, var2, var3, var4, var5);// , contagious);

					//contagious input
					newEvent = new Arrival(Hptr, type, var1, var2, var3, var4, var5, contagious);

					Hptr->CreateEvent(newEvent);
				}
				else {
					infile >> var1 >> var2 >> var3 >> var4;
					if (var3 == var4)continue;
					newEvent = new Arrival(Hptr, type, var1, var2, var3, var4);
					Hptr->CreateEvent(newEvent);
				}
			}
			else if (event == 'L') {
				infile >> var1 >> var2;
				newEvent = new Leave(Hptr, var1, var2);
				Hptr->CreateEvent(newEvent);
			}
			else if (event == 'S') {
				infile >> var1 >> var2;
				newEvent = new Stair(Hptr, var1, var2);
				Hptr->CreateEvent(newEvent);
			}
		}


	}
}

//UI::UI(Hospital* Hptr, ifstream& in) {
//	LoadFile(in, Hptr);
//}

//>> contagious;
//					if (var3 == var4)continue;
//					newEvent = new Arrival(HPtr, type, var1, var2, var3, var4, var5);// , contagious);

//void UI::LoadFile(ifstream&infile,Hospital*HPtr) {
//	int numfloors;
//	//infile.open();
//	if (infile.is_open()) {
//		infile >> numfloors;
//		HPtr->CreateFloors(numfloors);
//		//cout << numfloors << endl;
//		int var1, var2, var3;
//		infile >> var1 >> var2 >> var3;//speedV, speedC, speedP
//		HPtr->getVisitorElevator()->setSpeed(var1);
//		HPtr->getCargoElevator()->setSpeed(var2);
//		HPtr->getPatientElevator()->setSpeed(var3);
//		infile >> var1 >> var2 >> var3; //capV, capC, capP
//
//		HPtr->getVisitorElevator()->setCapacity(var1);
//		HPtr->getCargoElevator()->setCapacity(var2);
//		HPtr->getPatientElevator()->setCapacity(var3);
//		infile >> var1 >> var2;	 //S and autoS
//		HPtr->setSandautoS(var1,var2);
//		int num;
//		infile >> num;
//		HPtr->readNumEvents = num;
//		
//		char event, type;
//		int var4, var5;
//		for (int i = 0; i<num && infile >> event; i++) {
//			Event* newEvent;
//			if (event=='A') {
//				infile >> type;
//				if (type == 'P') {
//					bool contagious;
//					infile >> var1 >> var2 >> var3 >> var4 >> var5;//>> contagious;
//					if (var3 == var4)continue;
//					newEvent = new Arrival(HPtr, type, var1, var2, var3, var4, var5);// , contagious);
//					
//					HPtr->CreateEvent(newEvent);
////cout << *newEvent << endl;
//					//HPtr->currNumEvents++;
//
//					//cout << event << " " << type << " " << var1 << " " << var2 << " " << var3 << " " << var4 << " " <<var5<<endl;
//				}
//				else {
//					infile >> var1 >> var2 >> var3 >> var4;	
//					if (var3 == var4)continue;
//					newEvent = new Arrival(HPtr,type, var1, var2, var3, var4);
//					//HPtr->getEvents().enqueue(&*newEvent);
//					HPtr->CreateEvent(newEvent);
//
//					//cout << *newEvent << endl;
//
//					//HPtr->currNumEvents++;
//					//cout << event << " " << type << " " << var1 << " " << var2 << " " << var3 << " " << var4 << " "<<endl;
//
//				}
//			}
//			else if(event=='L') {
//				infile >> var1 >> var2;
//				newEvent = new Leave(HPtr, var1,var2);
//				//HPtr->getEvents().enqueue(&*newEvent);
//				//cout << *newEvent << endl;
//				HPtr->CreateEvent(newEvent);
//
//				//HPtr->currNumEvents++;
//
//				//cout << event<<" " << var1 << " " << var2 << endl;
//
//			}
//			else if (event == 'S') {
//
//				infile >> var1 >> var2;
//				newEvent = new Stair(HPtr, var1,var2);
//				//HPtr->getEvents().enqueue(&*newEvent);
//				HPtr->CreateEvent(newEvent);
//				//cout << *newEvent << endl;
//				//HPtr->currNumEvents++;
//				//cout << event<<" " << var1 << " " << var2 << endl;
//			}
//		}
//
//
//	}
//	//HPtr->getEvents().Traverse();
//  }


void UI:: PrintInteractiveMode(Hospital* HosPtr) {
	cout << "Current timestep: "<<"" << endl;

}

void UI::printMyStats(Hospital*pH,bool O) {
	cout << "TT	 " << "ID	" << "AT	" << "WT	" << "ST	\n";
	Node<Pickable*>* temp = pH->getCompletedList()->getTop();
	while (temp) {
		cout << temp->getData()->getTT() << "	"<< temp->getData()->getPickableId() <<"	" << temp->getData()->getAT() << "	" << temp->getData()->getWT() << "	" << temp->getData()->getST() << endl;
		
		pH->avgSerV += temp->getData()->getST();
		pH->avgWait += temp->getData()->getWT();
		temp = temp->getNext();
	}
	int c = pH->getCompletedList()->getCount();
	if(c)cout << pH->avgWait/c << endl;
	if(c)cout << pH->avgSerV/c << endl;
	//cout << pH->avgSerV << endl;
	pH->avgSerV = 0;
	pH->avgWait = 0;

}

//------------------------------------------------
string UI::readInputFileName() {
	cout << "Enter Test File Name: " << endl;
	cin >> InputFileName;
	return InputFileName + ".txt";// .txt directory is appended automatically
}
string UI::readOutputFileName() {
	cin >> OutputFileName;
	return OutputFileName + ".txt";// .txt directory is appended automatically
}

int UI::getMode()
{
	return Mode;
}

void UI::SelectMode() {
	cout << "Select Program Mode: (1 for Interactive mode, 2 for Step-by-step mode, 3 for Silent mode): ";
	cin >> Mode;
	while (Mode != 1 && Mode != 2 && Mode != 3) {
		cout << "Invalid Input! Re-enter Program Mode: (1 for ineractive mode, 2 for Step-by-step mode, 3 for Silent mode): ";
		cin >> Mode;
	}
	if (Mode == 1) {
		cout << "INTERACTIVE MODE" << endl;
		this->RunInteractiveMode();
	}
	else if (Mode == 2) {
		cout << "STEP-BY-STEP MODE" << endl;
		this->RunStepbyStepMode();
	}
	else if (Mode == 3) {
		cout << "SILENT MODE" << endl;
		this->RunSilentMode();
	}
}

void UI::RunInteractiveMode() {
	string InputString = "";
	cout << "Press Enter to Start." << endl;
	getline(std::cin, InputString);
	while (InputString.length() == 0 && Hptr->shouldsimulationEnd() == 0) {
		Hptr->SimpleSimulator();
		this->OutputWindow();
		cout << "Press ENTER to proceed with next time." << std::endl;
		getline(std::cin, InputString);
	}
	Hptr->WriteOutputFile();
}

void UI::RunStepbyStepMode() {
	string InputString = "";
	cout << "Press Enter to Start." << endl;
	getline(std::cin, InputString);
	while (!Hptr->shouldsimulationEnd())
	{
		Hptr->SimpleSimulator2();
		this->OutputWindow();
		Sleep(1000);
	}
	cout << "End of step-by-step simulation!" << endl;
	Hptr->WriteOutputFile();
}


void UI::RunSilentMode() {
	cout << "Simulation Starts..." << endl;
	while (!Hptr->shouldsimulationEnd())
	{
		Hptr->SimpleSimulator3();
	}
	Hptr->WriteOutputFile();
	cout << "Simulation ends, Output file is created." << endl;
}


void UI::OutputWindow() {
	Hptr->PrintFloors();
	cout << "***********\n";
	cout << Hptr->totalWaiting() << " total waiting pass/cargos (" << Hptr->leaving.getCount() << " visitors left)\n";
	cout << Hptr->countlist(Hptr->getEvents(), 'C') + Hptr->countlist(Hptr->getEvents(), 'P') + Hptr->countlist(Hptr->getEvents(), 'V') << " total in-service pass/cargos (" << Hptr->getStairs()->getCount() << " visitors by stairs)\n";   //countlist(this->getStairs(),'S')
	cout << Hptr->getCompletedList()->getCount() << " total completed pass/cargos\n";																												   //should be "visitors by stairs" in phase 2
	//Hptr->getPatientElevator()->PrintElevatorInfo();
	//Hptr->getVisitorElevator()->PrintElevatorInfo();
	//Hptr->getCargoElevator()->PrintElevatorInfo();
	return;
}
//---------------------------------------------------------------