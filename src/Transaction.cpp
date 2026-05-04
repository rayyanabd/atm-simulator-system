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

Transaction::Transaction(TransactionType t, double amt, double bal, string time, string desc) {
    type = t;
    amount = amt;
    balanceAfter = bal;
    timestamp = time;
    description = desc;
}

string Transaction::typeToString(TransactionType value) {
    switch (value) {
    case TransactionType::WITHDRAWAL: return "WITHDRAWAL";
    case TransactionType::DEPOSIT: return "DEPOSIT";
    case TransactionType::FAST_CASH: return "FAST_CASH";
    case TransactionType::TRANSFER: return "TRANSFER";
    case TransactionType::PIN_CHANGE: return "PIN_CHANGE";
    case TransactionType::ACCOUNT_CREATED: return "ACCOUNT_CREATED";
    case TransactionType::BALANCE_INQUIRY: return "BALANCE_INQUIRY";
    default: return "OTHER";
    }
}

string Transaction::typeToString() const {
    return typeToString(type);
}

TransactionType Transaction::stringToType(const string& value) {
    if (value == "WITHDRAW" || value == "WITHDRAWAL") return TransactionType::WITHDRAWAL;
    if (value == "DEPOSIT") return TransactionType::DEPOSIT;
    if (value == "FAST_CASH") return TransactionType::FAST_CASH;
    if (value == "TRANSFER") return TransactionType::TRANSFER;
    if (value == "PIN_CHANGE") return TransactionType::PIN_CHANGE;
    if (value == "ACCOUNT_CREATED") return TransactionType::ACCOUNT_CREATED;
    if (value == "BALANCE_INQUIRY") return TransactionType::BALANCE_INQUIRY;
    return TransactionType::TRANSFER;
}

void Transaction::printTransaction() const {
    cout << left << setw(12) << typeToString()
        << "| Rs." << setw(8) << amount
        << "| Bal: Rs." << setw(8) << balanceAfter
        << "| " << timestamp << endl;
}
