#include<iostream>
#include<string>
#include "../include/Account.h"
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

	Account user;
	user.changePIN();
	user.miniStatement();

	return 0;
}