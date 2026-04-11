#pragma once
#include <iostream>
#include <string>
#include "Account.h"
using namespace std;

class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;

public:
    CurrentAccount();
    bool debit(double amount);
    bool checkAndResetDailyLimit(double amount);
    double getDailyWithdrawn();
    double getRemainingLimit();
    string getAccountType();
};