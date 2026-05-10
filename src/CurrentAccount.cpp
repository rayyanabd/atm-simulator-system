#include "../include/CurrentAccount.h"
#include <iostream>
#include<fstream>
using namespace std;

CurrentAccount::CurrentAccount() : Account() {
    dailyLimit = 50000.0;
    dailyWithdrawn = 0.0;
    overdraftLimit = 10000.0;
}
bool CurrentAccount::isAlphabetic(string str)
{
    if (str.length() == 0)
        return false;

    for (int i = 0; i < str.length(); i++)
    {
        if (!((str[i] >= 'A' && str[i] <= 'Z') ||
            (str[i] >= 'a' && str[i] <= 'z') ||
            str[i] == ' '))
        {
            return false;
        }
    }

    return true;
}
bool CurrentAccount::isNumeric(string str)
{

    if (str.length() == 0)
        return false;

    for (int i = 0; i < str.length(); i++)
    {
      
        if (str[i] >= '0' && str[i] <= '9')
            continue;

      
        if (str[i] == '-')
            continue;
        return false;
    }

    return true;

}

string CurrentAccount::generateAccountno()
{
    string accNo, name, cnic, phone, pin, balanceStr, lockstr;
    int lastacc=0;
    ifstream infile("account.txt");
    if (!infile) {
        return "1001";
    }
    else
        while (getline(infile, accNo, ',') &&
            getline(infile, name, ',') &&
            getline(infile, cnic, ',') &&
            getline(infile, phone, ',') &&
            getline(infile, balanceStr, ',') &&
            getline(infile, pin, ',') &&
            getline(infile, lockstr)
            )

        {
            lastacc = stoi(accNo);
        }
    lastacc++;
    return to_string(lastacc);
}
void  CurrentAccount::createAccount()
{
    do {
        cout << "Enter Name" << endl;
        cin.ignore();
        getline(cin, accountHolder);

        if (!isAlphabetic(accountHolder))
            cout << "ERROR! Name should contain alphabets only.\n";

    } while (!isAlphabetic(accountHolder));


    do {
        cout << "Enter CNIC Number (13 digit input)" << endl;
        cin >> CNIC;

        if (!isNumeric(CNIC) || CNIC.length() !=13)
            cout << "ERROR! CNIC must contain 13 digits.\n";

    } while (!isNumeric(CNIC) || CNIC.length() != 13);


    do {
        cout << "Enter phone Number" << endl;
        cin >> phone_number;

        if (!isNumeric(phone_number))
            cout << "ERROR! Phone number must contain digits only.\n";

    } while (!isNumeric(phone_number));


   
        cout << "Your AccountNumber" << endl;
        accountNumber = generateAccountno();
        cout << accountNumber << endl;
       


    do {
        cout << "Enter 4 digit pin" << endl;
        cin >> pin;

        if (!isNumeric(pin) || pin.length() != 4)
            cout << "ERROR! Please enter exactly 4 digits.\n";

    } while (!isNumeric(pin) || pin.length() != 4);
    cout << "Account created successfully!!" << endl;

        saveAccountData();
       
}

void CurrentAccount::saveAccountData()
{
    ofstream outfile("account.txt", ios::app);
    outfile << accountNumber << "," << accountHolder << "," << CNIC << "," << phone_number << "," << balance << "," << pin<<","<<isLocked << endl;
    outfile.close();
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