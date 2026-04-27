#include <iostream>
#include "ATM.h"
#include "Account.h"
#include<iostream>


#include "../include/ATM.h"

//  ../  jumps back a folder.  /include is foler including .h files. 

using namespace std;

void display() {
	cout << "hafsa";
}
int main() {

	//create ATM with initial cash (file handling for cash in ATM in phase 2)

	//load accounts function
	//load previous transactions

	//sample accounts added (hard coded )for testing purposes in phase 1

	//start session ( atm.startSession(); )

	//after session ends, cursor comes back here
	display();


	// Note: You will need to declare the functions from Account.cpp 
	// or put them in Account.h so main can see them.
	void openAccount();
	void depositMoney();

		while (true) {
			std::cout << "\n1. Open Account\n2. Deposit\n3. Exit\n";
			int ch = getInt("Choice: ");
			if (ch == 1) openAccount();
			else if (ch == 2) depositMoney();
			else if (ch == 3) break;
		}

	return 0;
}