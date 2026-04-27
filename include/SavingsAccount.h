#pragma once
#include "Account.h"

class SavingsAccount : public Account {
public:
    SavingsAccount(int accNum, std::string nm, double bal, std::string p)
        : Account(accNum, nm, bal, p) {
    }
    // Add savings specific methods here
};