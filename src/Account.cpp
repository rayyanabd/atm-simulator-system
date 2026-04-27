
#include "ATM.h"
#include <iostream>
using namespace std;

#include "ACCOUNT_H"

Account::Account(int accNum, std::string nm, double bal, std::string p)
    : accountNum(accNum), name(nm), balance(bal), pin(p) {
}

bool Account::validatePin(std::string p) const {
    return pin == p;
}

void Account::deposit(double amount) {
    if (amount > 0) balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}