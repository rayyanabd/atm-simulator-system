#pragma once
#include "Account.h"

class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;
    double overdraftLimit;

public:

    CurrentAccount();
    CurrentAccount(string accno, string name, string cnic, string phoneno, double b);
    void  createAccount();
    bool debit(double amount) override;
    std::string getAccountType() override { return "Current Account"; }
};