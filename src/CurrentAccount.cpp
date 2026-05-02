#include "../include/CurrentAccount.h"
#include <iostream>

using namespace std;

CurrentAccount::CurrentAccount() : Account() {
    dailyLimit = 50000.0;
    dailyWithdrawn = 0.0;
    overdraftLimit = 10000.0;
    accountNumber = "1001"; // HArd coded for testing
    pin = "1234";
    balance = 50000;
}

bool CurrentAccount::debit(double amount) {

    // ensures the cumulative daily withdrawal doesn't exceed 50,000

    if (dailyWithdrawn + amount > dailyLimit) {
        double remaining = dailyLimit - dailyWithdrawn;
        cout << "Daily withdrawal limit reached. Remaining for today: Rs. " << remaining << endl;
        return false;
    }

    // Balance check
    if (amount > (balance + overdraftLimit)) {
        cout << "Insufficient funds.\n";
        return false;
    }

    balance -= amount;
    dailyWithdrawn += amount;
    addTransaction(TransactionType::WITHDRAWAL, amount, "ATM Withdrawal");
    return true;
}