#pragma once
#include <iostream>
#include <string>
#include <vector>
#include "Account.h"
#include "Constants.h"

class ATM {
private:
    Account* accounts[MAX_ACCOUNTS];
    int accountCount;
    Account* currentAccount;
    double cashAvailable;
    std::string dataDirectory;

  
    int searchAcc(string accNum);
    void clearAccounts();
    std::string accountsFilePath() const;
    std::string transactionsFilePath() const;
    std::string generateAccountNumber() const;
    void seedDefaultAccounts();
    void showMainMenu(); //

public:
    ATM(double initialCash, std::string dataDir = "");
    ~ATM();



    void addAccount(Account* account);
    bool insertCard(std::string accNum);
    bool insertCardByLastFourDigits(std::string lastFourDigits);
    Account* findAccountByLastFourDigits(std::string lastFourDigits) const;
    bool enterPIN(std::string pin);
    bool authenticatePin(std::string pin, std::string& message);
    void ejectCard();
    Account* getCurrentAccount();
    const Account* getCurrentAccount() const;
    double getCashAvailable() const { return cashAvailable; }

   
    void withdraw(double amount);
    bool withdrawAmount(double amount);
    bool withdrawAmount(double amount, std::string& message, bool fastCashTransaction = false);
    void deposit();     
    bool depositAmount(double amount);
    bool depositAmount(double amount, std::string& message);
    void checkBalance(); 
    void changePIN();    
    bool changeCurrentPIN(std::string currentPIN, std::string newPIN, std::string& message);
    void fastCash();
    void miniStatement();
    std::vector<Transaction> getCurrentTransactions(int count) const;
    bool createAccount(std::string cnicNo, std::string newPIN, std::string accountType, std::string& generatedAccountNumber, std::string& message);
    bool loadData();
    bool saveData() const;

    // System Entry Point
    void start();
};
