#include "../include/SavingsAccount.h"
#include <cmath>
#include <iostream>

using namespace std;

SavingsAccount::SavingsAccount()
    : Account("1002", "Default Savings User", 25000.0, "4321", "3520212345672") {
    dailyLimit = ATM_DAILY_WITHDRAWAL_LIMIT;
    dailyWithdrawn = 0.0;
}

SavingsAccount::SavingsAccount(string accNum, string nm, double bal, string p, string cnicNo, double withdrawnToday)
    : Account(accNum, nm, bal, p, cnicNo) {
    dailyLimit = ATM_DAILY_WITHDRAWAL_LIMIT;
    dailyWithdrawn = withdrawnToday;
}

bool SavingsAccount::debit(double amount) {
    return debit(amount, TransactionType::WITHDRAWAL, "ATM Withdrawal");
}

bool SavingsAccount::debit(double amount, TransactionType type, const string& desc) {
    if (!std::isfinite(amount) || amount <= 0) {
        lastError = "Withdrawal amount must be greater than zero.";
        return false;
    }

    if (dailyWithdrawn + amount > dailyLimit) {
        double remaining = dailyLimit - dailyWithdrawn;
        lastError = "Daily withdrawal limit reached. Remaining for today: Rs. " + to_string(remaining);
        cout << "[LIMIT] Daily withdrawal limit reached. Remaining for today: Rs. " << remaining << endl;
        return false;
    }

    if (amount > balance) {
        lastError = "Insufficient balance.";
        cout << "[BALANCE] Insufficient balance.\n";
        return false;
    }

    balance -= amount;
    dailyWithdrawn += amount;
    addTransaction(type, amount, desc);
    lastError.clear();
    return true;
}
