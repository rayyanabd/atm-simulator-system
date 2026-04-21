#include <iostream>
#include "../include/Account.h"
#include "../include/Constants.h"

using namespace std;

bool readInt(int& out) {
    if (!(cin >> out)) {
        cin.clear();
        cin.ignore(1000, '\n');
        return false;
    }
    return true;
}

int main() {
    Account myAccount;
    string accNum;

    cout << "==============================\n";
    cout << "==============================\n";
    cout << "Enter Account Number: ";
    cin >> accNum;

    int currentRole = ROLE_NONE;

    // PIN Entry Loop
    while (!myAccount.isAccountLocked()) {
        currentRole = myAccount.enterPIN();

        if (currentRole != ROLE_NONE) break;

        if (myAccount.isAccountLocked()) {
            cout << "🚫 Session terminated.\n";
            return 1;
        }
    }

    // Role-based Routing
    if (currentRole == ROLE_SUPER_ADMIN) {
        myAccount.adminMenu();
        return 0;
    }

    // Main User Menu
    int choice = -1;
    while (choice != 5) {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. Withdraw (Pending)\n";
        cout << "2. Deposit (Pending)\n";
        cout << "3. Mini Statement (Pending)\n";
        cout << "4. Admin Menu\n";
        cout << "5. Exit\n";
        cout << "================================\n";
        cout << "Choice: ";

        if (!readInt(choice)) {
            cout << "❌ Invalid input. Enter a number between 1 and 5.\n";
            continue;
        }

        switch (choice) {
            case 1:
            case 2:
            case 3:
                cout << "⏳ Feature pending other members' integration.\n";
                break;
            case 4: {
                cout << "\n🔐 Admin access requires Super Admin PIN.\n";
                int adminCheck = myAccount.enterPIN();
                if (adminCheck == ROLE_SUPER_ADMIN) {
                    myAccount.adminMenu();
                } else {
                    cout << "⛔ Access denied. Super Admin PIN required.\n";
                }
                break;
            }
            case 5:
                cout << "👋 Thank you for using the ATM. Goodbye!\n";
                break;
            default:
                cout << "❌ Invalid choice. Enter 1-5.\n";
        }
    }

    return 0;
}