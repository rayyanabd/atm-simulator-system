#include "../include/CurrentAccount.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

CurrentAccount::CurrentAccount() {
    dailyLimit = 25000.0;
    dailyWithdrawn = 0.0;
}

bool CurrentAccount::checkAndResetDailyLimit(double amount) {
    double remaining = dailyLimit - dailyWithdrawn;
    if (amount > remaining) {
        cout << "ERROR!! EXCEEDS DAILY LIMIT\n";
        cout << "Remaining Today : Rs. " << remaining << "\n";
        return false;
    }
    return true;
}

bool CurrentAccount::debit(double amount) {
    if (amount > balance) {
        cout << "Insufficient balance!\n";
        return false;
    }
    if (!checkAndResetDailyLimit(amount))
        return false;
    balance -= amount;
    dailyWithdrawn += amount;

    // Phase 2 — save after every debit
    saveDailyLimit();
    return true;
}

double CurrentAccount::getDailyWithdrawn() { return dailyWithdrawn; }
double CurrentAccount::getRemainingLimit() { return dailyLimit - dailyWithdrawn; }
string CurrentAccount::getAccountType() { return "Current Account"; }

// Phase 2
void CurrentAccount::saveDailyLimit() {
    ofstream file("dailylimit.txt");
    if (!file.is_open()) { 
        cout << "Error saving!\n"; 
        return; 
    }
    file << accountNumber << "," << dailyWithdrawn << "\n";  
    file.close();
}

void CurrentAccount::loadDailyLimit() {
    ifstream file("dailylimit.txt");
    if (!file.is_open()) {
        dailyWithdrawn = 0.0;
        return; 
    }
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string num, amount;
        getline(ss, num, ',');
        getline(ss, amount, ',');
        if (num == accountNumber)
            dailyWithdrawn = stod(amount);
    }
    file.close();
}