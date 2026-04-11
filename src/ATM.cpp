#include <iostream>
#include <string>
#include "../include/ATM.h"
using namespace std;

//withdraw(); and fast cash();

//ATM::ATM(Account* acc)  //constructor
//{
//    currentAccount = acc;
//}

void ATM::withdraw() {
   /* if (currentAccount == nullptr) {
        cout << "No account linked!\n";
        return;
    }*/
    cout << "\n ======= WITHDRAW CASH =======\n";
    cout << "Balance: Rs. " << currentAccount->getBalance() << "\n";
    double amount;
    cout << "Enter amount: Rs. ";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount\n";
        return ;
    }
    if ((int)amount % 100 != 0) {
        cout << "error!! enter multiples of Rs.100 only\n";
        return;
    }
    if (currentAccount->debit(amount)) {
        cout << "Rs. " << amount << "withdrawn!\n";
        cout << "New Balance : Rs. " << currentAccount->getBalance() << "\n";
    }
}

void ATM::fastCash() {
    /*if (currentAccount == nullptr) {
        cout << "No account linked!\n";
        return;
    }*/
    double fixAmounts[] = { 500, 1000, 2000, 5000, 10000, 20000 };
    cout << "\n======= FAST CASH =======\n";
    cout << "Balance : Rs. " << currentAccount->getBalance() << "\n\n";
    for (int i = 0; i < 6; i++) {
        cout << (i + 1) << ". Rs. " << fixAmounts[i] << "\n";
    }
    cout << "7. Cancel\n";
    int choice;
    cout << "enter your choice: ";
    cin >> choice;
    if (choice == 7) {
        cout << "Cancelled!\n";
        return;
    }
    if (choice < 1 || choice > 6) {
        cout << "Invalid choice!\n";
        return;
    }
    double amount = fixAmounts[choice - 1];
    if (currentAccount->debit(amount)) {
        cout << "Rs. " << amount << " withdrawn!\n";
        cout << "New Balance : Rs. " << currentAccount->getBalance() << "\n";
    }
}