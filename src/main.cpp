#include <iostream>
#include "../include/Account.h"
#include "../include/Constants.h"

using namespace std;

//  Helper: safe integer input
bool readInt(int& out) {
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }
    return true;
}

//  Helper: admin PIN prompt that does NOT
//  count toward the user account lockout
bool checkAdminPIN() {
    string pin;
    cout << "Enter Super Admin PIN: ";
    cin >> pin;
    return (pin == ADMIN_PIN);
}

int main() {
    Account myAccount;
    string accNum;

    cout << "==============================\n";
    cout << "    ATM SYSTEM — Phase 1      \n";
    cout << "==============================\n";
    cout << "Enter Account Number: ";
    cin >> accNum;


    int currentRole = ROLE_NONE;

    while (!myAccount.isAccountLocked()) {
        currentRole = myAccount.enterPIN();
        if (currentRole != ROLE_NONE) break;
    }

    if (myAccount.isAccountLocked() && currentRole == ROLE_NONE) {
        cout << "[SESSION ENDED] Too many failed attempts.\n";
        return 1;
    }

    if (currentRole == ROLE_SUPER_ADMIN) {
        myAccount.adminMenu();
        return 0;
    }

    int choice = -1;

    while (choice != 5) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Withdraw            (pending Member 2)\n";
        cout << "2. Deposit             (pending Member 3)\n";
        cout << "3. Mini Statement      (pending Member 4)\n";
        cout << "4. Admin Menu\n";
        cout << "5. Exit\n";
        cout << "================================\n";
        cout << "Choice: ";

        if (!readInt(choice)) {
            cout << "[ERROR] Invalid input. Enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice) {
            case 1:
            case 2:
            case 3:
                cout << "[PENDING] Feature awaiting other members' integration.\n";
                break;

            case 4:
                cout << "\n[ADMIN] Super Admin PIN required.\n";
                if (checkAdminPIN()) {
                    myAccount.adminMenu();
                } else {
                    cout << "[DENIED] Incorrect Super Admin PIN.\n";
                }
                break;

            case 5:
                cout << "Thank you for using the ATM. Goodbye!\n";
                break;

            default:
                cout << "[ERROR] Invalid choice. Enter 1-5.\n";
        }
    }

    return 0;
}