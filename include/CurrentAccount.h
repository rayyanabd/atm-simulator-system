#pragma once
#include "Account.h"

class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;
    double overdraftLimit;

public:

    CurrentAccount();
    
    void  createAccount();
    void saveAccountData();
    bool debit(double amount) override;
    std::string getAccountType() override { return "Current Account"; }
};