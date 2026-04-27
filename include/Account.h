#pragma once

#include "ATM.h"
#include "Constants.h"
#ifndef ACCOUNT_H
#define ACCOUNT_H
#include <string>

class Account {
protected:
    int accountNum;
    std::string name;
    double balance;
    std::string pin;

public:
    Account(int accNum, std::string nm, double bal, std::string p);
    virtual ~Account() {}

    int getAccountNum() const { return accountNum; }
    std::string getName() const { return name; }
    double getBalance() const { return balance; }
    bool validatePin(std::string p) const;

    void deposit(double amount);
    virtual bool withdraw(double amount); // Virtual so children can override logic
};

#endif