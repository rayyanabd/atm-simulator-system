#pragma once

#include "ATM.h"
#include "Constants.h"
#ifndef ACCOUNT_H
#define ACCOUNT_H

#include <string>
#include <vector>

struct Account {
    int accNum;
    std::string name;
    double balance;
    std::string pin;
};

// This tells other files that the list of accounts exists elsewhere
extern std::vector<Account> allAccounts;
extern int accCounter;

#endif