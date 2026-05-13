#include "SavingsAccount.h"
#include <iostream>

SavingsAccount::SavingsAccount() : Account() {
}

bool SavingsAccount::debit(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        addTransaction(TransactionType::WITHDRAWAL, amount, "Cash Withdrawal (Savings)");
        return true;
    }
    return false;
}

void SavingsAccount::applyInterest() {
    double interest = balance * SAVINGS_INTEREST_RATE;
    if (interest > 0) {
        balance += interest;
        addTransaction(TransactionType::DEPOSIT, interest, "Annual Interest Added");
    }
}
