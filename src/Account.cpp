#include "../include/Account.h"
#include "../include/Constants.h"
#include <iostream>

using namespace std;

Account::Account()
    : isLocked(false),
      pinAttempts(0),
      accountPIN("1234") 
{}

void Account::lockAccount() {
    isLocked = true;
    cout << "\n🔒 Account locked after " << MAX_ATTEMPTS
         << " failed attempts. Contact admin to unlock.\n";
}

bool Account::verifyPIN(const string& enteredPIN) {
    if (isLocked) {
        cout << "❌ Account is locked. Contact admin.\n";
        return false;
    }

    if (enteredPIN == accountPIN) {
        pinAttempts = 0;
        return true;
    }

    pinAttempts++;
    int remaining = MAX_ATTEMPTS - pinAttempts;

    if (pinAttempts >= MAX_ATTEMPTS) {
        lockAccount();
    } else {
        cout << "❌ Incorrect PIN. Attempts remaining: " << remaining << "\n";
    }

    return false;
}

int Account::enterPIN() {
    if (isLocked) {
        cout << "❌ Account is locked. Contact admin.\n";
        return ROLE_NONE;
    }

    string enteredPIN;
    cout << "Enter PIN: ";
    cin >> enteredPIN;

    if (enteredPIN.empty()) {
        cout << "❌ PIN cannot be empty.\n";
        return ROLE_NONE;
    }

    for (char c : enteredPIN) {
        if (!isdigit(c)) {
            cout << "❌ PIN must contain digits only.\n";
            return ROLE_NONE;
        }
    }

    if (enteredPIN == ADMIN_PIN) {
        cout << "🔑 Super Admin access granted.\n";
        return ROLE_SUPER_ADMIN;
    }

    if (verifyPIN(enteredPIN)) {
        cout << "✅ PIN correct. Welcome!\n";
        return ROLE_USER;
    }

    return ROLE_NONE;
}

void Account::unlockAccount() {
    isLocked = false;
    pinAttempts = 0;
    cout << "✅ Account successfully unlocked.\n";
}

void Account::resetPIN(const string& newPIN) {
    if (newPIN.empty()) {
        cout << "❌ New PIN cannot be empty.\n";
        return;
    }
    for (char c : newPIN) {
        if (!isdigit(c)) {
            cout << "❌ PIN must contain digits only.\n";
            return;
        }
    }
    accountPIN = newPIN;
    cout << "✅ PIN successfully reset.\n";
}

bool Account::isAccountLocked() const {
    return isLocked;
}

void Account::adminMenu() {
    int choice = -1;

    while (choice != 4) {
        cout << "\n========== SUPER ADMIN MENU ==========\n";
        cout << "1. View All Accounts (Pending)\n";
        cout << "2. Unlock Account\n";
        cout << "3. Reset PIN\n";
        cout << "4. Exit Admin Menu\n";
        cout << "======================================\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "❌ Invalid input. Enter a number between 1 and 4.\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "⏳ Feature pending Member 3's integration.\n";
                break;
            case 2:
                if (isLocked) unlockAccount();
                else cout << "ℹ️ Account is not currently locked.\n";
                break;
            case 3: {
                string newPIN;
                cout << "Enter new PIN: ";
                cin >> newPIN;
                resetPIN(newPIN);
                break;
            }
            case 4:
                cout << "👋 Exiting Admin Menu.\n";
                break;
            default:
                cout << "❌ Invalid choice. Enter 1–4.\n";
        }
    }
}