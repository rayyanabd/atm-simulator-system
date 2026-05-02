

#pragma once
#include "Account.h"
#include <fstream>
#include <sstream>
using namespace std;

class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;

public:
    CurrentAccount();
    virtual bool debit(double amount) override;
    string getAccountType();
    void setDailyWithdrawn(double amount) { dailyWithdrawn = amount; }
    void saveDailyLimit();
    void loadDailyLimit();
    double getDailyWithdrawn();
    double getRemainingLimit();
    bool checkAndResetDailyLimit(double amount);  
};