#include "ATM.h"
#include <iostream>
#include <fstream>

using namespace std;

ATM::ATM(double initialCash)
{
    cashAvailable = initialCash;
    totalAccounts = 0;
    lastAccNumber = STARTING_ACC_NUM - 1;
    currentAccount = nullptr;

    for (int i = 0; i < MAX_ACCOUNTS; i++)
    {
        accounts[i] = nullptr;
    }
}

ATM::~ATM()
{
    for (int i = 0; i < totalAccounts; i++)
    {
        delete accounts[i];
    }
}

bool ATM::isValidPin(string p)
{
    if (p.length() != 4)
        return false;

    for (int i = 0; i < 4; i++)
    {
        if (p[i] < '0' || p[i] > '9')
            return false;
    }

    return true;
}

int ATM::findAccount(string accNum)
{
    for (int i = 0; i < totalAccounts; i++)
    {
        if (accounts[i] != nullptr && accounts[i]->getAccountNumber() == accNum)
            return i;
    }

    return -1;
}

void ATM::addAccount(Account* account)
{
    if (totalAccounts < MAX_ACCOUNTS)
    {
        accounts[totalAccounts++] = account;
    }
}

bool ATM::insertCard(string accNum)
{
    int index = findAccount(accNum);

    if (index != -1)
    {
        currentAccount = accounts[index];
        return true;
    }

    cout << "Account not found.\n";
    return false;
}

bool ATM::enterPIN(string pin)
{
    if (currentAccount == nullptr)
        return false;

    if (currentAccount->validatePIN(pin))
    {
        return true;
    }

    return false;
}

void ATM::ejectCard()
{
    currentAccount = nullptr;
    cout << "Card ejected.\n";
}

void ATM::withdraw(double amount)
{
    if (currentAccount == nullptr)
        return;

    if (amount <= 0)
    {
        cout << "Invalid amount.\n";
        return;
    }

    if (amount > cashAvailable)
    {
        cout << "ATM has insufficient cash.\n";
        return;
    }

    if (currentAccount->debit(amount))
    {
        cashAvailable -= amount;
        cout << "Please collect your cash.\n";
    }
    else
    {
        cout << "Withdrawal failed.\n";
    }
}

void ATM::deposit()
{
    if (currentAccount == nullptr)
        return;

    double amount;
    cout << "Enter amount to deposit: ";
    cin >> amount;

    if (amount <= 0)
    {
        cout << "Invalid amount.\n";
        return;
    }

    currentAccount->credit(amount);
    cashAvailable += amount;

    cout << "Deposit successful.\n";
}

void ATM::checkBalance()
{
    if (currentAccount == nullptr)
        return;

    cout << "Current Balance: Rs. " << currentAccount->getBalance() << "\n";
}

void ATM::changePIN()
{
    if (currentAccount == nullptr)
        return;

    string oldPin, newPin;

    cout << "Enter current PIN: ";
    cin >> oldPin;

    if (!currentAccount->validatePIN(oldPin))
    {
        cout << "Wrong PIN.\n";
        return;
    }

    cout << "Enter new 4-digit PIN: ";
    cin >> newPin;

    if (!isValidPin(newPin))
    {
        cout << "Invalid PIN format.\n";
        return;
    }

    currentAccount->changePIN(newPin);
    cout << "PIN changed successfully.\n";
}

void ATM::fastCash()
{
    int choice;
    double options[] = { 500, 1000, 2000, 5000, 10000, 20000 };

    cout << "\n1. 500\n2. 1000\n3. 2000\n4. 5000\n5. 10000\n6. 20000\n";
    cout << "Choice: ";
    cin >> choice;

    if (choice >= 1 && choice <= 6)
    {
        withdraw(options[choice - 1]);
    }
    else
    {
        cout << "Invalid choice.\n";
    }
}

void ATM::miniStatement()
{
    if (currentAccount != nullptr)
    {
        currentAccount->printMiniStatement();
    }
}

void ATM::saveWithdrawals()
{
    ofstream file("withdrawals.txt");

    if (!file)
        return;

    for (int i = 0; i < totalAccounts; i++)
    {
        if (accounts[i] != nullptr)
        {
            file << accounts[i]->getAccountNumber() << " "
                << accounts[i]->getDailyWithdrawalTotal() << "\n";
        }
    }

    file.close();
}

void ATM::loadWithdrawals()
{
    ifstream file("withdrawals.txt");

    if (!file)
        return;

    string accNum;
    double total;

    while (file >> accNum >> total)
    {
        int index = findAccount(accNum);

        if (index != -1)
        {
            accounts[index]->setDailyWithdrawalTotal(total);
        }
    }

    file.close();
}

void ATM::addNewAccount()
{
    cout << "Add new account feature depends on derived account classes.\n";
}

void ATM::adminPortal()
{
    string pin;
    cout << "Enter admin PIN: ";
    cin >> pin;

    if (pin != ADMIN_PIN)
    {
        cout << "Wrong admin PIN.\n";
        return;
    }

    cout << "\n--- All Accounts ---\n";

    for (int i = 0; i < totalAccounts; i++)
    {
        if (accounts[i] != nullptr)
        {
            cout << accounts[i]->getAccountNumber()
                << "\tRs. " << accounts[i]->getBalance() << "\n";
        }
    }
}

void ATM::showMainMenu()
{
    int choice = -1;

    while (choice != 0)
    {
        cout << "\n1. Balance\n";
        cout << "2. Withdraw\n";
        cout << "3. Deposit\n";
        cout << "4. Fast Cash\n";
        cout << "5. Change PIN\n";
        cout << "6. Mini Statement\n";
        cout << "0. Logout\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            checkBalance();
            break;

        case 2:
        {
            double amt;
            cout << "Enter amount: ";
            cin >> amt;
            withdraw(amt);
            break;
        }

        case 3:
            deposit();
            break;

        case 4:
            fastCash();
            break;

        case 5:
            changePIN();
            break;

        case 6:
            miniStatement();
            break;

        case 0:
            ejectCard();
            break;

        default:
            cout << "Invalid choice.\n";
        }
    }
}

void ATM::start()
{
    string accNum, pin;

    cout << "Enter account number: ";
    cin >> accNum;

    if (!insertCard(accNum))
        return;

    cout << "Enter PIN: ";
    cin >> pin;

    if (!enterPIN(pin))
    {
        cout << "Wrong PIN.\n";
        ejectCard();
        return;
    }

    showMainMenu();
}