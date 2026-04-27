#include "Account.h"
#include "ATM.h"
#include <iostream>
using namespace std;

void openAccount() {
    cout << "\n-- Open New Account --\n";
    Account a;
    cout << "Enter name: ";
    getline(cin, a.name);

    a.balance = getAmount("Enter opening balance: Rs. ");

    while (true) {
        cout << "Set 4 digit PIN: ";
        string p;
        getline(cin, p);
        if (!checkPin(p)) cout << "PIN must be 4 digits.\n";
        else { a.pin = p; break; }
    }

    a.accNum = accCounter++;
    allAccounts.push_back(a);
    cout << "\nAccount created! Acc No: " << a.accNum << endl;
}

void depositMoney() {
    int num = getInt("Enter account number: ");
    int idx = searchAcc(num);
    if (idx == -1) {
        cout << "Account not found.\n";
        return;
    }
    double amt = getAmount("Enter deposit amount: Rs. ");
    allAccounts[idx].balance += amt;
    cout << "Success. New balance: Rs. " << allAccounts[idx].balance << endl;
}