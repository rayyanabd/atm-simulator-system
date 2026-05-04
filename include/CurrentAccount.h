#pragma once
#include "Account.h"

class CurrentAccount : public Account {
private:
    double dailyLimit;
    double dailyWithdrawn;

public:

    CurrentAccount();
    CurrentAccount(std::string accNum, std::string nm, double bal, std::string p, std::string cnicNo = "", double withdrawnToday = 0.0);
    bool debit(double amount) override;
    bool debit(double amount, TransactionType type, const std::string& desc) override;
    std::string getAccountType() const override { return "Current Account"; }
    double getDailyWithdrawn() const override { return dailyWithdrawn; }
};
