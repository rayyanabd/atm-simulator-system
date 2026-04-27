#ifndef ATM_H
#define ATM_H
#include "Account.h"
#include "Constants.h"

class ATM {
private:
    Account* accounts[MAX_ACCOUNTS]; // Using pointers for polymorphism
    int totalAccounts;
    int accCounter;

    int searchAcc(int num);
    bool checkPinFormat(std::string p);
    double getAmountInput(std::string msg);
    int getIntInput(std::string msg);

public:
    ATM();
    ~ATM();
    void start();
    void openAccount();
    void depositMoney();
};

#endif