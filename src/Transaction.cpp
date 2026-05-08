#define _CRT_SECURE_NO_WARNINGS 
#include "../include/Transaction.h"
#include <iostream>
#include <iomanip>
#include <ctime>

using namespace std;

Transaction::Transaction(TransactionType t, double amt, double bal, string desc) {
    type = t;
    amount = amt;
    balanceAfter = bal;
    description = desc;

    // Get current system time
    time_t now = time(0);
    char* dt = ctime(&now);
    timestamp = string(dt);
    if (!timestamp.empty()) timestamp.pop_back(); // Remove newline
}

Transaction::Transaction(TransactionType t, double amt, double bal, string desc,string time) {
    type = t;
    amount = amt;
    balanceAfter = bal;
    description = desc;

    timestamp = time;
}

// Phase 2 — save transaction to file
void Transaction::saveToFile(string accNum) {
    ofstream file("transactions.txt", ios::app);
    if (!file.is_open()) { cout << "Error saving transaction!\n"; return; }
    file << accNum << "," << typeToString() << "," << amount << ","
        << balanceAfter << "," << description<<","<<timestamp << "\n";
    file.close();
}
string Transaction::typeToString() const {
    switch (type) {
    case TransactionType::WITHDRAWAL: return "WITHDRAW";
    case TransactionType::DEPOSIT:    return "DEPOSIT";
    case TransactionType::FAST_CASH:  return "FAST_CASH";
    default: return "OTHER";
    }
}

void Transaction::printTransaction() const {
    cout << left << setw(12) << typeToString()
        << "| Rs." << setw(8) << amount
        << "| Bal: Rs." << setw(8) << balanceAfter
        << "| " << timestamp << endl;
}
TransactionType Transaction::stringtoType(string s)
{
    if (s == "WITHDRAW")
    {
        return TransactionType::WITHDRAWAL;
    }
     if (s == "DEPOSIT")
    {
        return TransactionType::DEPOSIT;
    }
     if (s == "FAST_CASH")
     {
         return TransactionType::FAST_CASH;

     }
}
// Phase 2 — load all transactions from file
void Transaction::loadFromFile() {
    ifstream file("transactions.txt");
    if (!file.is_open()) {
        cout << "No transaction history found.\n";
        return;
    }
    string accnum, amt,type, bal, desc,time;
   
    cout << "\n=== TRANSACTION HISTORY ===\n";
    while (getline(file,accnum,',') &&
        getline(file, type, ',')&&
        getline(file, amt, ',')&&
        getline(file, bal, ',')&&
        getline(file, desc,',') &&
        getline(file,time)
        ) {
        double amount = stod(amt);
        double balance = stod(bal);

        //converting string to type
        TransactionType t = stringtoType(type);
        
        
    }
    file.close();
}