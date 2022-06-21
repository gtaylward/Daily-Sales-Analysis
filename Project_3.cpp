/***********************************************************************
*                                                                      *
* @Author: Greg Aylward                                                *
* @Professor: Bill Chan                                                *
* Assignment: Project 3 Shopping cart                                  *
* Project_3.cpp file                                                   *
*                                                                      *
************************************************************************/

/***********************************************************************
* Attribution:                                                         *
*                                                                      *
* StackOverflow queried for solutions to several isues encountered     *
*   while integrating .cpp and .py code sets                           *
************************************************************************/

/***********************************************************************
* Required files:                                                      *
* This program calls code from file: PythonCode.py                     *
*                                                                      *
***********************************************************************/

#include <Python.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <Windows.h>
#include <cmath>
#include <string>

#pragma warning(disable : 4996)  //supresses strcpy error as function is depreciated

using namespace std;

int userInput;
string str;
string fileLine;
// Method to call a python function
void CallProcedure(string pName)
{
	char* procname = new char[pName.length() + 1];
	std::strcpy(procname, pName.c_str());

	Py_Initialize();
	PyObject* my_module = PyImport_ImportModule("PythonCode");
	PyErr_Print();
	PyObject* my_function = PyObject_GetAttrString(my_module, procname);
	PyObject* my_result = PyObject_CallObject(my_function, NULL);
	Py_Finalize();

	delete[] procname;
}
// Method overload a Python function with two string variables 
int callIntFunc(string proc, string param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	char* paramval = new char[param.length() + 1];
	std::strcpy(paramval, param.c_str());


	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(z)", paramval);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean
	delete[] procname;
	delete[] paramval;


	return _PyLong_AsInt(presult);
}
// Method overload a Python function with a string variable and int variable 
int callIntFunc(string proc, int param)
{
	char* procname = new char[proc.length() + 1];
	std::strcpy(procname, proc.c_str());

	PyObject* pName, * pModule, * pDict, * pFunc, * pValue = nullptr, * presult = nullptr;
	// Initialize the Python Interpreter
	Py_Initialize();
	// Build the name object
	pName = PyUnicode_FromString((char*)"PythonCode");
	// Load the module object
	pModule = PyImport_Import(pName);
	// pDict is a borrowed reference
	pDict = PyModule_GetDict(pModule);
	// pFunc is also a borrowed reference
	pFunc = PyDict_GetItemString(pDict, procname);
	if (PyCallable_Check(pFunc))
	{
		pValue = Py_BuildValue("(i)", param);
		PyErr_Print();
		presult = PyObject_CallObject(pFunc, pValue);
		PyErr_Print();
	}
	else
	{
		PyErr_Print();
	}
	//printf("Result is %d\n", _PyLong_AsInt(presult));
	Py_DECREF(pValue);
	// Clean up
	Py_DECREF(pModule);
	Py_DECREF(pName);
	// Finish the Python Interpreter
	Py_Finalize();

	// clean
	delete[] procname;

	return _PyLong_AsInt(presult);
}
// Main Menu provides a user interface and program control
void MainMenu()
{
	// Simple menu with 4 numeric options  
	cout << "----------------- User Menu -----------------------------" << endl;
	cout << "1: Produce a list of all items purchased with quantities" << endl;
	cout << "2: Display how many times a specific item was sold" << endl;
	cout << "3: Produce a histogram of sales data" << endl;
	cout << "4: Exit" << endl << endl;
	cout << "Enter your selection as a number: 1, 2, 3, or 4." << endl;
	cout << "---------------------------------------------------------" << endl;
	// Get user input
	// Calls appropriate function based on number chosen
	cin >> userInput;
	switch (userInput) {
		// Call Python function to generate list of items and quantities ordered each day
		case 1: {
			system("cls");  //Clears the screen
			cout << "\n\nPrinting items and quantities sold today: " << endl;
			CallProcedure("ItemCounter");
			cout << endl;
			break;
		}
		// Call Python function to generate amount sold for a specific item
		case 2: {
			system("cls");  //Clears the screen
			cout << "\n\nEnter item name: " << endl;
			cin >> str;  //get user input (specific item to analyse)
			system("cls");  //Clears the screen
			cout << "Printing the sales for " << str << ": " << endl;
			cout << str << ": " << callIntFunc("CountItemSales", str) << endl;
			cout << endl;
			break;
		}
		// Calls a Cpp function to create a text-based Histogram of items sold
		case 3: {
			system("cls");  //Clears the screen
			// create file with item and total sold and save to output file
			cout << "Writing items and quantities sold today to a file: " << endl;
			CallProcedure("CreateFreqDataFile");  // Python function creates Frequency data file
			//cout << endl;
			// Open the output file and read the data
			ifstream freqFile("grocery_sales.dat");
			string itemDescription;
			int itemCount;
			// Parse through the file line by line and print the histogram
			while (getline(freqFile, fileLine))
			{
				istringstream iss(fileLine);
				if (!(iss >> itemDescription >> itemCount)) { break; } // detect error and return to menu
				// Print the item name and "count" number of asterisks
				cout << itemDescription << " " << string(itemCount, '*') << endl;
			}
			break;
		}
		case 4: {
			system("cls");
			cout << "Exiting the program" << endl;
			cout << "Thank you" << endl;
			exit(EXIT_SUCCESS);
		
		}

	}

}


int main()
{
	// Loop menu. Program termination possible through Menu '4'
	while (1)
	{
		MainMenu();
	}

	return 0;
}
