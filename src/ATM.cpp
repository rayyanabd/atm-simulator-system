#include <iostream>
#include <vector>
#include <string>
#include "Account.h"  
#include "ATM.h"      

using namespace std;

// Initialize the global variables
vector<Account> allAccounts;
int accCounter = 1001;
bool checkPin(string p) {
    if (p.length() != 4) return false;
    for (char c : p) {
        if (c < '0' || c > '9') return false;
    }
    return true;
}

int searchAcc(int num) {
    for (int i = 0; i < allAccounts.size(); i++) {
        if (allAccounts[i].accNum == num) return i;
    }
    return -1;
}

double getAmount(string msg) {
    double val;
    while (true) {
        cout << msg;
        cin >> val;
        if (cin.fail() || val <= 0) {
            cout << "Invalid input. Must be a positive number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else {
            cin.ignore(1000, '\n');
            return val;
        }
    }
}

int getInt(string msg) {
    int n;
    while (true) {
        cout << msg;
        cin >> n;
        if (cin.fail()) {
            cout << "Please enter a valid number.\n";
            cin.clear();
            cin.ignore(1000, '\n');
        }
        else {
            cin.ignore(1000, '\n');
            return n;
        }
    }
}