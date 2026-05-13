#pragma once
#include "Account.h"
#include "Constants.h"

class SavingsAccount : public Account {
public:
    SavingsAccount();
    bool debit(double amount) override;
    std::string getAccountType() override { return "Savings Account"; }

    // Interest application
    void applyInterest();
};
