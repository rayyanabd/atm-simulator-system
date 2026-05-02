
#pragma once
#include <string>
#include "Constants.h"
    using namespace std;

    class Transaction {
    private:
        TransactionType type;
        double amount;
        double balanceAfter;
        string description;
        string accountNumber;

    public:
        Transaction(TransactionType t, double amt, double bal, string desc);
        void printTransaction() const;
        string typeToString() const;

        void saveToFile(string accNum);
        static void loadFromFile();
    };