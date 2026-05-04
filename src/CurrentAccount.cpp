#include "../include/CurrentAccount.h"
#include <cmath>
#include <iostream>

using namespace std;

CurrentAccount::CurrentAccount() : Account() {
    dailyLimit = ATM_DAILY_WITHDRAWAL_LIMIT;
    dailyWithdrawn = 0.0;
    accountNumber = "1001"; // Default for testing
    accountHolder = "Default Current User";
    cnic = "3520212345671";
    pin = "1234";
    balance = 50000;
}

CurrentAccount::CurrentAccount(string accNum, string nm, double bal, string p, string cnicNo, double withdrawnToday)
    : Account(accNum, nm, bal, p, cnicNo) {
    dailyLimit = ATM_DAILY_WITHDRAWAL_LIMIT;
    dailyWithdrawn = withdrawnToday;
}

bool CurrentAccount::debit(double amount) {
    return debit(amount, TransactionType::WITHDRAWAL, "ATM Withdrawal");
}

bool CurrentAccount::debit(double amount, TransactionType type, const string& desc) {
    if (!std::isfinite(amount) || amount <= 0) {
        lastError = "Withdrawal amount must be greater than zero.";
        return false;
    }

    // This logic ensures the cumulative daily withdrawal doesn't exceed 50,000
    if (dailyWithdrawn + amount > dailyLimit) {
        double remaining = dailyLimit - dailyWithdrawn;
        lastError = "Daily withdrawal limit reached. Remaining for today: Rs. " + to_string(remaining);
        cout << "[LIMIT] Daily withdrawal limit reached. Remaining for today: Rs. " << remaining << endl;
        return false;
    }

    // Balance check
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
