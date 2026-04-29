
#include <iostream>
#include <string>
#include "../include/CurrentAccount.h"
using namespace std;

//daily limit logic implemented in debit function of current account. 
//if amount exceeds daily limit, transaction is denied and user is informed about remaining limit for the day.
//if amount is within limit, it is debited from balance and added to daily withdrawn amount.

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
    return true;
}
double CurrentAccount::getDailyWithdrawn() { 
    return dailyWithdrawn; 
}
double CurrentAccount::getRemainingLimit() { 
    return dailyLimit - dailyWithdrawn;
}
string CurrentAccount::getAccountType() { 
    return "Current Account";   // ans of virtual function implemented in current account class
}