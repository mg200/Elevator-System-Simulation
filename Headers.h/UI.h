#pragma once
#include<iostream>
#include<fstream>
#include <windows.h>
#include <iomanip>
#include <stdlib.h>
//#include "Hospital.h"
using namespace std;

class Hospital;

class UI
{	   protected:
	Hospital* Hptr;
public:
	string InputFileName = "SampleInput.txt";
	string OutputFileName;
	int Mode;
	UI(Hospital* Hptr);
	//UI(Hospital* Hptr,ifstream&in);
	void LoadFile();
	//void LoadFile(ifstream&inputfile, Hospital*Hptr);
	void PrintInteractiveMode(Hospital* HosPtr);
	void printMyStats(Hospital*,bool output=true);//1 for file
	void RunInteractiveMode();
	void RunStepbyStepMode();
	void RunSilentMode();
	void OutputWindow();
	void SelectMode();
	int getMode();
	string readInputFileName();
	string readOutputFileName();
	~UI();
};

