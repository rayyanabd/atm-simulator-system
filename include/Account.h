#pragma once
#include <string>
#include "ATM.h"
#include "Constants.h"

using namespace std;


  
class Account {
private:
    bool isLocked;
    int pinAttempts;
    string accountPIN;
    static const int MAX_ATTEMPTS = 3;

    void lockAccount();
  
  protected:
    int accountNum;
    string name;
    double balance;

public:
    Account();

    int enterPIN();
    bool verifyPIN(const string& enteredPIN);
    bool isAccountLocked() const;
    void unlockAccount();
    void resetPIN(const string& newPIN);
    string getPIN() const;        
    void adminMenu();

    Account(int accNum, std::string nm, double bal, std::string p);
    virtual ~Account() {}

    int getAccountNum() const { return accountNum; }
    string getName() const { return name; }
    double getBalance() const { return balance; }
    bool validatePin(std::string p) const;

    void deposit(double amount);
    virtual bool withdraw(double amount); // Virtual so children can override logic
};


