#include "../include/CurrentAccount.h"
#include <iostream>
#include<fstream>
using namespace std;

CurrentAccount::CurrentAccount() : Account() {
    dailyLimit = 50000.0;
    dailyWithdrawn = 0.0;
    overdraftLimit = 10000.0;
}

void  CurrentAccount::createAccount()
{
        cout << "Enter your name" << endl;
        cin >> accountHolder;

        cout << "Enter CNIC Number" << endl;
        cin >> CNIC;

        cout << "Enter phone Number" << endl;
        cin >> phone_number;

        cout << "Enter AccountNumber" << endl;
        cin >> accountNumber;

        do {
            cout << "Enter 4 digit pin" << endl;
            cin >> pin;
            if (pin.length() != 4)
            {
                cout << "ERROR! please enter 4 digit code" << endl;
            }
        } while (pin.length() != 4);

        cout << "Account Created successfully!!" << endl;

        ofstream outfile("account.txt", ios::app);
        outfile << accountNumber << "," << accountHolder <<"," << CNIC <<"," << phone_number<<"," <<balance<<pin<<endl;
        outfile.close();
}

CurrentAccount::CurrentAccount(string accno, string name, string cnic, string phoneno, double b,string pin)
{
    accountHolder = name;
    CNIC = cnic;
    phone_number=phoneno;
    accountNumber = accno;
    balance = b;
    this->pin = pin;
    dailyLimit = 50000.0;
    dailyWithdrawn = 0.0;
    overdraftLimit = 10000.0;

}
bool CurrentAccount::debit(double amount) {
    // This logic ensures the cumulative daily withdrawal doesn't exceed 50,000
    if (dailyWithdrawn + amount > dailyLimit) {
        double remaining = dailyLimit - dailyWithdrawn;
        cout << "[LIMIT] Daily withdrawal limit reached. Remaining for today: Rs. " << remaining << endl;
        return false;
    }

    // Balance check
    if (amount > (balance + overdraftLimit)) {
        cout << "[BALANCE] Insufficient funds.\n";
        return false;
    }

    balance -= amount;
    dailyWithdrawn += amount;
    addTransaction(TransactionType::WITHDRAWAL, amount, "ATM Withdrawal");
    return true;
}