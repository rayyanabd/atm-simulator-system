#pragma once
#include <string>
#include<fstream>
#include "Constants.h"
using namespace std;

class Transaction {
private:
    TransactionType type;
    double amount;
    double balanceAfter;
    std::string timestamp;
    std::string description;

public:
    Transaction(TransactionType t, double amt, double bal, std::string desc);
    void printTransaction() const;
    std::string typeToString() const;
    void saveToFile(string accNum);
};