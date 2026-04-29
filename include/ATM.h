#pragma once
#include <iostream>
#include <string>
#include "Account.h"
#include "CurrentAccount.h"
#include "Constants.h"
using namespace std;

class ATM {
private:
    Account* currentAccount;   
    Account* accounts[MAX_ACCOUNTS]; // Using pointers for polymorphism
    int totalAccounts;
    int accCounter;

    int searchAcc(int num);
    bool checkPinFormat(std::string p);
    double getAmountInput(std::string msg);
    int getIntInput(std::string msg);
  
public:
   // ATM(Account* acc);    
    void withdraw();
    void fastCash();
    ATM();
    ~ATM();
    void start();
    void openAccount();
    void depositMoney();
};

