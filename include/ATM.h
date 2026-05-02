#pragma once
#include <iostream>
#include <string>
#include "Account.h"
#include"CurrentAccount.h"
#include "Constants.h"

class ATM {
private:
    Account* accounts[MAX_ACCOUNTS];
    int accountCount;
    Account* currentAccount;
    double cashAvailable;

    int searchAcc(string accNum);
    void showMainMenu(); //

public:
    ATM();
    ATM(double initialCash);
    ~ATM();



    void addAccount(Account* account);
    bool insertCard(std::string accNum);
    bool enterPIN(std::string pin);
    void ejectCard();

   
    void withdraw(double amount);
    void deposit();     
    void checkBalance(); 
    void changePIN();    
    void fastCash();
    void miniStatement();

    // System Entry Point
    void start();
};