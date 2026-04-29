#pragma once
#include <string>

class Transaction {
    std::string type;
    double amount;
    std::string timestamp;
public:
    Transaction(std::string t, double amt);
    void display();
};