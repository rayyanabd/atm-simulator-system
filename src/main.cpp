//#include<iostream>
//#include<string>
//#include "../include/Account.h"
//#include "../include/ATM.h"
//
////  ../  jumps back a folder.  /include is foler including .h files. 
//
//using namespace std;
//
//void display() {
//	cout << "hafsa";
//}
//int main() {
//
//	//create ATM with initial cash (file handling for cash in ATM in phase 2)
//
//	//load accounts function
//	//load previous transactions
//
//	//sample accounts added (hard coded )for testing purposes in phase 1
//
//	//start session ( atm.startSession(); )
//
//	//after session ends, cursor comes back here
//	//display();
//
//	////Account user;
//	//user.changePIN();
//	//user.miniStatement();
//
//	return 0;
//}

// //--withdraw and fast cash testing--
//#include <iostream>
//#include "../include/ATM.h"
//#include "../include/CurrentAccount.h"
//using namespace std;
//int main() {
//    CurrentAccount acc;
//    acc.setBalance(50000.0);  
//    ATM atm(&acc);
//    int choice;
//    do {
//        cout << "[1] Withdraw\n";
//        cout << "[2] Fast Cash\n";
//        cout << "[0] Exit\n";
//        cout << "Choice: ";
//        cin >> choice;
//        switch (choice) {
//        case 1: atm.withdraw(); 
//           break;
//        case 2: atm.fastCash();  
//           break;
//        }
//    } while (choice != 0);
//    return 0;
//}