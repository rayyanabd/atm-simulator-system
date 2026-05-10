#include "../include/ATM.h"
#include <iostream>
#include<fstream>
using namespace std;

ATM::ATM()
{
    cashAvailable = 0;
    accountCount = 0;
    currentAccount = nullptr;
    for (int i = 0; i < MAX_ACCOUNTS; i++) accounts[i] = nullptr;
}


ATM::~ATM() {
    for (int i = 0; i < accountCount; i++) {
        delete accounts[i];
    }

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

    readAccountdata();

    int choice;
    do {
        cout << "\n===== ADMIN PANEL =====\n";
        cout << "1. View Accounts\n";
        cout << "2. Lock/Unlock Account\n";
        cout << "3. Delete Account\n";
        cout << "4. Save & Exit\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1: viewAccounts(); break;
        case 2: toggleLock(); break;
        case 3: deleteAccount(); break;
        case 4: saveAccountsToFile(); break;
        }
    } while (choice != 4);

}
void ATM::deleteAccount()
{
    int flag = -1;
    string accNo;
    cout << "Enter account to delete: ";
    cin >> accNo;

    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i]->getAccountNumber() == accNo)
        {
            delete accounts[i];

            for (int j = i; j < accountCount - 1; j++)
            {
                accounts[j] = accounts[j + 1];
            }
            flag = 1;
            accountCount--;
            cout << "Account deleted.\n";
            
        }
    }
    if(flag==-1)
    cout << "Account not found.\n";
}
void ATM::saveAccountsToFile()
{
    ofstream outfile("account.txt");

    for (int i = 0; i < accountCount; i++)
    {
        outfile << accounts[i]->getAccountNumber() << ","
            << accounts[i]->getname() << ","
            << accounts[i]->getCNIC() << ","
            << accounts[i]->getPhone() << ","
            << accounts[i]->getBalance() << ","
            << accounts[i]->getPIN() << ","
            << accounts[i]->isAccountLocked()
            << endl;
    }

}
void ATM::toggleLock()
{
    cout << "Total accounts: " << accountCount << endl;
    string accNo;
    cout << "Enter account number: ";
    cin >> accNo;

    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i]->getAccountNumber() == accNo)
        {
            bool status = accounts[i]->isAccountLocked();
            accounts[i]->setLock(!status);

            cout << "Account is now "
                << (accounts[i]->isAccountLocked() ? "LOCKED" : "UNLOCKED") << endl;
            return;
        }
    }

    cout << "Account not found.\n";
}

void ATM::viewAccounts()
{
    cout << "\n--- All Accounts ---\n";

    for (int i = 0; i < accountCount; i++)
    {
        if (accounts[i] != nullptr)
        {
            cout << accounts[i]->getAccountNumber()
                << "\tRs. " << accounts[i]->getBalance()
                <<"\tAccount Status:  ";
            if (accounts[i]->isAccountLocked())
            {
                cout << "Locked" << endl;
            }
            else {
                cout << "Active Status" << endl;
            }
        }
    }
    cout << "[INFO] Accounts loaded: " << accountCount << endl;
}

void ATM::addAccount(Account* acc) {
    if (accountCount < MAX_ACCOUNTS) {
        accounts[accountCount++] = acc;
    }
}

int ATM::searchAcc(string accNum) {
   //file check
    readAccountdata();
    for (int i = 0; i < accountCount; i++) {
        if (accounts[i]->getAccountNumber() == accNum) return i;
    }
       
    return -1;
}
void ATM::readTransactiondata()
{
    ifstream file("transactions.txt");
    if (!file.is_open()) {
        return;
    }

    string accnum, type, amt, bal, desc, time;
    while (getline(file, accnum, ',') &&
        getline(file, type, ',') &&
        getline(file, amt, ',') &&
        getline(file, bal, ',') &&
        getline(file, desc, ',') &&
        getline(file, time)
        )
    {
        double tran_amount = stod(amt);
        double tran_balance = stod(bal);
        if (accnum == currentAccount->getAccountNumber()) {
            TransactionType ttype;

            if (type == "DEPOSIT") ttype = TransactionType::DEPOSIT;
            else if (type == "WITHDRAW") ttype = TransactionType::WITHDRAWAL;
            else if (type == "FAST_CASH") ttype = TransactionType::FAST_CASH;
            else ttype = TransactionType::WITHDRAWAL;

            currentAccount->addTransaction(ttype, tran_amount, desc,time);
        }
       
    }
    file.close();
}
void ATM::readAccountdata()
{
    string accNo, name, cnic, phone, pin, balanceStr, lockstr;
    ifstream infile("account.txt");
    if (!infile) {
        cout << "File not found!\n";
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
            double balance = stod(balanceStr);
            bool lock = (lockstr == "1");
            Account* tempAcc = new CurrentAccount();
            tempAcc->setdata(accNo, name, cnic, phone, balance, pin, lock);
            accounts[accountCount++] = tempAcc;
    }

}
void ATM::start() {
    int choice;
    bool systemRunning = true;

    while (systemRunning) {
        cout << "\n====================================\n";
        cout << "      WELCOME TO THE ATM SYSTEM     \n";
        cout << "====================================\n";
        cout << "1. Insert Card\n";
        cout << "0. Power Off System\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear(); cin.ignore(1000, '\n');
            cout << "[INVALID] Please enter 1 or 0.\n";
            continue;
        }

        if (choice == 0) {
            cout << "Shutting down system... Goodbye!\n";
            systemRunning = false;
        }
        else if (choice == 1) {
            int accAttempts = 0;
            bool sessionActive = false;

            // Account Number Identification 
            while (accAttempts < 3) {
                string accNum;
                cout << "\nEnter Account Number: ";
                cin >> accNum;

                if (insertCard(accNum)) {
                    // Check if account is already locked before proceeding
                    if (currentAccount->isAccountLocked()) {
                        cout << "ERROR: This account is currently locked. Please contact Admin.\n";
                        sessionActive = true;
                        break;
                    }

                    //  PIN Verification 
                    int pinAttempts = 0;
                    bool pinSuccess = false;

                    while (pinAttempts < 3) {
                        string pin;
                        cout << "Enter 4-Digit PIN: ";
                        cin >> pin;

                        // validatePIN handles the internal counter and locking logic
                        if (currentAccount->validatePIN(pin)) {
                            cout << "Login accepted.\n";
                            pinSuccess = true;
                            showMainMenu(); // Enter banking system
                            break;
                        }
                        else {
                            pinAttempts++;
                            if (currentAccount->isAccountLocked()) {
                                cout << "\n[SECURITY ALERT] Wrong PIN entered 3 times.\n";
                                cout << "[!] ACCOUNT OFFICIALLY LOCKED.\n";
                                break;
                            }
                            cout << "Incorrect PIN. Try again.\n";
                        }
                    }

                    sessionActive = true;
                    break;
                }
                else {
                    accAttempts++;
                    if (accAttempts < 3) cout << "[ERROR] Account not recognized.\n";
                }
            }

            // Security cleanup after 3 failed account number tries
            if (!sessionActive && accAttempts == 3) {
                cout << "\n[SECURITY] 3 Invalid Account attempts. CARD EJECTED.\n";
            }

            // Ensure card is "ejected" (currentAccount set to null) for next user
            ejectCard();
        }
    }
}

bool ATM::insertCard(string accNum) {
    int temp = searchAcc(accNum);
    if (temp!=-1) {
        currentAccount = accounts[temp];
        cashAvailable = currentAccount->getBalance();
       readTransactiondata();
        return true;
    }
    
    cout << "[ERROR] Card unrecognized or invalid account.\n";
    return false;
}

bool ATM::enterPIN(string pin) {
    if (currentAccount->validatePIN(pin)) {
        cout << "[SUCCESS] Access Granted.\n";
        return true;
    }
    cout << "[DENIED] Incorrect PIN.\n";
    currentAccount = nullptr;
    return false;
}

void ATM::withdraw(double amount) {
    
    if (amount > 20000) {
        cout << "DENIED: Maximum withdrawal per transaction is Rs. 20,000.\n";
        return;
    }

    if (amount > cashAvailable) {
        cout << "ERROR: ATM has insufficient cash reserve.\n";
        return;
    }

    // currentAccount->debit will handle the Daily Limit check (50,000)
    if (currentAccount->debit(amount)) {
        cashAvailable -= amount;
        cout << "[SUCCESS] Please collect your cash: Rs. " << amount << endl;


        saveAccountsToFile();

    }
}

void ATM::fastCash() {
   
    double options[] = { 500, 1000, 2000, 5000, 10000, 20000 };
    cout << "\n======= FAST CASH =======\n";
    for (int i = 0; i < 6; i++) {
        cout << i + 1 << ". Rs. " << options[i] << endl;
    }

    int choice;
    cout << "Choice: "; 
    cin >> choice;

    if (choice >= 1 && choice <= 6) {
        // Fast cash automatically passes the amount to the withdraw function
        withdraw(options[choice - 1]);
    }
    else {
        cout << "Selection out of range.\n";
    }
}

void ATM::showMainMenu() {
    int choice = -1;
    while (choice != 0) {
        cout << "\n---------- MAIN MENU ----------\n";
        cout << "1. Balance Inquiry\n";
        cout << "2. Cash Withdrawal\n";
        cout << "3. Cash Deposit\n";     
        cout << "4. Fast Cash\n";
        cout << "5. Change PIN\n";        
        cout << "6. Mini Statement\n";
        cout << "0. Logout / Exit\n";
        cout << "-------------------------------\n";
        cout << "Choice: ";
        cin >> choice;

        switch (choice) {
        case 1: checkBalance(); break;
        case 2: {
            double amt; cout << "Amount: "; cin >> amt;
            withdraw(amt); break;
        }
        case 3: deposit(); break;
        case 4: fastCash(); break;
        case 5: changePIN(); break;
        case 6: currentAccount->printMiniStatement(); break;
        case 0: ejectCard(); break;
        default: cout << "Invalid choice.\n";
        }
    }
}

void ATM::deposit() {
    double amount;
    cout << "\n======= CASH DEPOSIT =======\n";
    cout << "Enter amount to deposit: Rs. ";
    cin >> amount;


    if (amount > 50000) {
        cout << "Error: Maximum deposit limit per transaction is Rs. 50,000.\n";
        return;
    }

    if (amount <= 0) {
        cout << "Invalid amount.\n";
        return;
    }

    currentAccount->credit(amount);
    cashAvailable += amount;
    cout << "Deposit complete.\n";
    //saving to file
    Transaction t(TransactionType::DEPOSIT, amount,
        currentAccount->getBalance(), "ATM Deposit");
    t.saveToFile(currentAccount->getAccountNumber());

    saveAccountsToFile();
    
}

void ATM::checkBalance() {
    cout << "\n======= BALANCE INQUIRY =======\n";
    cout << "Account Holder: " << currentAccount->getAccountNumber() << endl;
    cout << "Current Balance: Rs. " << currentAccount->getBalance() << endl;
}

void ATM::changePIN() {
    string oldPin, newPin, confirmPin;
    cout << "\n======= PIN CHANGE =======\n";
    cout << "Enter Current PIN: ";
    cin >> oldPin;

    // Verify current PIN before allowing change
    if (currentAccount->validatePIN(oldPin)) {
        cout << "Enter New 4-Digit PIN: ";
        cin >> newPin;

        
        if (newPin.length() != 4) {
            cout << "Error: PIN must be exactly 4 digits.\n";
            return;
        }

        cout << "Confirm New PIN: ";
        cin >> confirmPin;

        if (newPin == confirmPin) {
            currentAccount->changePIN(newPin);
            cout << "PIN updated. Please use your new PIN next time.\n";

            saveAccountsToFile();

        }
        else {
            cout << "PINs do not match. Change aborted.\n";
        }
    }
    else {
        cout << "Incorrect current PIN.\n";
    }
}

void ATM::ejectCard() {
    cout << "[INFO] Card ejected. Goodbye!\n";
    currentAccount = nullptr;
}