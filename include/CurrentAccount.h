#pragma once
#include "Account.h"
#include "Constants.h"
class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;
    double overdraftLimit;

public:

    CurrentAccount();
    string generateAccountno();
    bool isAlphabetic(string str);
    bool isNumeric(string str);
    void  createAccount();
    void saveAccountData();
    bool debit(double amount) override;
    std::string getAccountType() override { return "Current Account"; }
};