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
    Transaction(TransactionType t, double amt, double bal, std::string desc, std::string time);
    void printTransaction() const;
    std::string typeToString() const;
    TransactionType stringtoType(std:: string s);
    void saveToFile(string accNum);
     void loadFromFile();
};