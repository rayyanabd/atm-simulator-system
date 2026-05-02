
#define _CRT_SECURE_NO_WARNINGS
#include "../include/Transaction.h"
#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

Transaction::Transaction(TransactionType t, double amt, double bal, string desc) {
    type = t;
    amount = amt;
    balanceAfter = bal;
    description = desc;
}

string Transaction::typeToString() const {
    switch (type) {
    case TransactionType::WITHDRAWAL: return "WITHDRAWAL";
    case TransactionType::DEPOSIT:    return "DEPOSIT";
    case TransactionType::FAST_CASH:  return "FAST_CASH";
    default: return "OTHER";
    }
}

void Transaction::printTransaction() const {
    cout << typeToString()
        << " | Rs." << amount
        << " | Balance: Rs." << balanceAfter
        << " | " << description << "\n";
}

// Phase 2 — save transaction to file
void Transaction::saveToFile(string accNum) {
    ofstream file("transactions.txt", ios::app);
    if (!file.is_open()) { cout << "Error saving transaction!\n"; return; }
    file << accNum << ","<< typeToString() << ","<< amount << ","
        << balanceAfter << ","<< description << "\n";
    file.close();
}

// Phase 2 — load all transactions from file
void Transaction::loadFromFile() {
    ifstream file("transactions.txt");
    if (!file.is_open()) {
        cout << "No transaction history found.\n";
        return;
    }
    string line;
    cout << "\n=== TRANSACTION HISTORY ===\n";
    while (getline(file, line)) {
        stringstream ss(line);
        string acc, t, amt, bal, desc;
        getline(ss, acc, ',');
        getline(ss, t, ',');
        getline(ss, amt, ',');
        getline(ss, bal, ',');
        getline(ss, desc, ',');
        cout << acc << " | " << t<< " | Rs." << amt<< " | Bal: Rs." << bal<< " | " << desc << "\n";
    }
    file.close();
}