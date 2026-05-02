#pragma once
#include <iostream>
#include <string>
#include "Account.h"
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
    ATM(double initialCash);
    ~ATM();



    void addAccount(Account* account);
    bool insertCard(std::string accNum);
    bool enterPIN(std::string pin);
    void ejectCard();
    Account* getCurrentAccount();
    const Account* getCurrentAccount() const;

   
    void withdraw(double amount);
    bool withdrawAmount(double amount);
    void deposit();     
    bool depositAmount(double amount);
    void checkBalance(); 
    void changePIN();    
    void fastCash();
    void miniStatement();

    // System Entry Point
    void start();
};
