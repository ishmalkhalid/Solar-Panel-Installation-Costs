/*-------------------------------------------------*/
/* Name: Ishmal Khalid, Net ID: ik1299 */
/* Date: November 20, 2019. */
/* Program: Assignment.cpp */
/* Description: This program calculates the cost of installing a flat-panel photovoltaic power system for residential home use in Abu Dhabi, UAE.*/
/*-------------------------------------------------*/

#include <iostream>
#include <cmath>

using namespace std;

//Declaring and initializing global variables
#define InsolCsize 24 //column dimsnsions
#define InsolRsize 12  //row dimensions
#define Invefficiency 0.90 //power inverter efficiency
#define Solefficiency 0.18 //solar panel efficiency
#define SizeInsol 12 //size of seasonal variation in insolation array
double SeasVarInsol[SizeInsol] = { 4.89, 5.52, 6.21, 6.88, 7.20, 6.76, 6.33, 6.39, 6.35, 6.04, 5.13, 4.68 }; //seasonal variation in insolation array

//Function Prototypes
double dailyInsolation(double* insolationptr, int columnSize);
double fillInInsolation(double insolation[][InsolCsize], int rowSize);
void getInput(double&, double&, double&, double&);
double MaxInsolation(double insolation[][InsolCsize], int rowSize);

int main(){

	//Declare and initialize variables
	double insolDayMin, insolHourMax, *insolHourMaxPtr = &insolHourMax;
	double area = 0, cost = 0, batteryCapacity = 4.8;
	int minPowInv;
	double DER = 0, solpan = 0, powivt = 0, batt = 0;
	double insolation[InsolRsize][InsolCsize];

	//Initialize all elements of matrix to 0
	for (int j = 0; j < 12; j++){
		for (int i = 0; i < 24; i++){
			insolation[j][i] = 0;
		}
	}

	insolDayMin = fillInInsolation(insolation, InsolRsize); //Return and save smallest daily insolation value 
	*insolHourMaxPtr = MaxInsolation(insolation, InsolRsize); //Return and save maximum insolation value

	getInput(DER, solpan, powivt, batt); //Function to obtain input values from the user
	area = ((DER / (Solefficiency * insolDayMin)) / Invefficiency); //calculate area of solar panels
	minPowInv = ceil(((insolHourMax*area) / 5)*(Solefficiency / Invefficiency)); //calculate minimum number of power inverters
	cost = (area*solpan) + (minPowInv*powivt) + (ceil(DER / batteryCapacity)*batt); //calculate total cost of the solar power system

	cout << "The total cost of installing a flat-panel photovoltaic power system is: " << cost << endl;
	system("pause");
	return 0;
}

//Function to obtain input values from the user
void getInput(double &DER, double &solpan, double &powivt, double &batt){

	do{
		cout << "Enter the daily energy requirements/kWh:";
		cin >> DER;

		cout << "Enter the individual component costs for:" << endl;

		cout << "Solar panels/$: ";
		cin >> solpan;

		cout << "Power Inverters/$: ";
		cin >> powivt;

		cout << "Batteries/$: ";
		cin >> batt;

		//Validate user input
		if (DER < 0 || solpan < 0 || powivt < 0 || batt < 0)
			cout << "Invalid input. Please re-enter values." << endl;

	} while (DER < 0 || solpan < 0 || powivt < 0 || batt < 0); //Repeat if input is invalid
}
	
//Function to return smallest daily insolation
double fillInInsolation(double insolation[][InsolCsize], int rowSize){

	double MinDailyInsol[InsolRsize];
	double minVal;

	//Loop to store SeasVarInsol value in insolation matrix for all hours of each month 
	for (int i = 0; i < SizeInsol; i++){
		for (int j = 6; j < 18; j++){
			insolation[i][j] = SeasVarInsol[i];
		}
	}
	//Loop to store daily insolation for each month
	for (int i = 0; i < rowSize; i++){
		MinDailyInsol[i] = dailyInsolation(insolation[i], rowSize);
	}

	minVal = MinDailyInsol[0];

	//Loop to smallest daily insolation value
	for (int i = 1; i < rowSize; i++){
		if (MinDailyInsol[i] < minVal){
			minVal = MinDailyInsol[i];
		}
	}
	return minVal; //returns smallest daily insolation value
}

//Function to approximate insolation on an hourly basis for each month
double dailyInsolation(double* insolationRptr, int columnSize){

	double dailyinsol = 0;

	//Loop to calculate and store insolation for each hour of each month
	for (int j = 0; j < 12; j++){
		insolationRptr[6 + j] = 2 * insolationRptr[6 + j] * cos(0.2618*(j + 6 - 12));
		dailyinsol = dailyinsol + insolationRptr[6 + j];
	}
	return dailyinsol; //Returns daily insolation value for each month
}

//Function to extract largest insolation value from matrix
double MaxInsolation(double insolation[][InsolCsize], int rowSize){

	double insolHourMax = 0;

	//Loop to find largest insolation value in whole matrix
	for (int j = 0; j < 24; j++){
		for (int i = 0; i < 12; i++){
			if (insolation[i][j]> insolHourMax)
				insolHourMax = insolation[i][j];
		}
	}
	return insolHourMax; //return largest insolation value
}
