#pragma once
#include <string>
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
    Transaction(TransactionType t, double amt, double bal, std::string time, std::string desc);
    void printTransaction() const;
    TransactionType getType() const { return type; }
    double getAmount() const { return amount; }
    double getBalanceAfter() const { return balanceAfter; }
    std::string getTimestamp() const { return timestamp; }
    std::string getDescription() const { return description; }
    std::string typeToString() const;
    static std::string typeToString(TransactionType type);
    static TransactionType stringToType(const std::string& value);
};
