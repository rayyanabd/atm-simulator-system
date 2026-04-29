#include "../include/Account.h"

//#include<string>
#include "../include/Constants.h"
#include <iostream>

using namespace std;
//
//bool digitPIN4(string arr)//validiation for 4 digit PIN
//{
//	int digit = 0;
//	for (int i = 0; arr[i] != '\0'; i++)
//	{
//		digit++;
//	}
//	if (digit == 4)
//	{
//		return true;
//	}
//	else
//		return false;
//}
//
//Account::Account()
//{
//	accountName = "0000";
//	PIN = nullptr;
//	balance=0.0;
//
//}
//
//void Account::changePIN()
//{
//	string PINcheck;
//	string PINcheck1;
//	int checkentry = 1, answer;
//
//	cout << "For your security choose a unique  PIN" << endl;
//	do {
//		do {
//			cout << "Enter current PIN" << endl;//entering old PIN
//			cin >> PINcheck;
//			if (!digitPIN4(PINcheck))
//			{
//				cout << "Error! enter 4 digit pin code" << endl;
//			}
//		} while (!digitPIN4(PINcheck));
//
//		for (int j = 0; j < 4; j++)
//		{
//			if (PINcheck[j] != PIN[j])
//			{
//				cout << "invalid PIN" << endl;
//				checkentry = -1;
//				break;
//			}
//		}
//		if (checkentry == -1)
//		{
//			cout << "PIN dont match" << endl;
//		}
//	} while (checkentry==-1);
//
//	do{
//		cout << "Enter new PIN" << endl;//entering New PIN
//		cin >> PINcheck;
//		if (!digitPIN4(PINcheck))
//		{
//			cout << "Error! enter 4 digit pin code" << endl;
//		}
//	} while (!digitPIN4(PINcheck));
//
//
//	do{
//		cout << "Re-enter new PIN to confirm" << endl;//Confirming PIN
//			cin >> PINcheck1;
//		if (!digitPIN4(PINcheck1))
//		{
//			cout << "Error! enter 4 digit pin code" << endl;
//		}
//		else
//			if (PINcheck1 != PINcheck)
//			{
//				cout << "not same PIN" << endl;
//				checkentry = -1;
//			}
//
//	} while (!digitPIN4(PINcheck1) || checkentry==-1);
//
//
//	do {
//		cout << "1.(Cancel new PIN)   or   2.(Confirm new PIN)" << endl;
//		cin >> answer;
//		} while (answer != 1 && answer != 2);
//
//	if (answer == 2)
//	{
//		delete[]PIN;
//		for (int k = 0; k < 4; k++)
//		{
//			PIN[k] = PINcheck[k];
//		}
//		cout << "PIN changed" << endl;
//	}
//}
//
//void Account::miniStatement()
//{
//	cout << "Date         |     " << "Transaction Type       |         " << "Amount    |       " << "Balance     " << endl;
//
//}
//
//Account::~Account()
//{
//	delete[]PIN;
//	PIN = nullptr;
//}
//Date::Date()//date for miniStatement
//{
//	day = 1;
//	month = 1;
//	year = 2026;
//}
//void Date::incrementDate()
//{
//	int maxDay;
//	if (month == 4 || month == 6 || month == 9 || month == 11)
//	{
//		maxDay = 30;
//	}
//	else if (month == 2)
//	{
//		maxDay = 28;
//	}
//	else {
//		maxDay = 31;
//	}
//
//	if (day + 1 < maxDay)
//	{
//		day++;
//	}
//	else {
//		day = 1;
//		if (month + 1 < 12)
//		{
//			month++;
//		}
//		else {
//			month = 1;
//			year++;
//		}
//	}
//
//
//}



//  Helper: validate that a string is exactly
//  4 numeric digits (used for PIN checks)
static bool isValidPINFormat(const string& pin) {
    if (pin.length() != 4) return false;
    for (char c : pin) {
        if (!isdigit(c)) return false;
    }
    return true;
}

//  Constructor
Account::Account()
    : isLocked(false),
      pinAttempts(0),
      accountPIN("1234")   // Default PIN for Phase 1 testing
{}

//  Private: lock this account

void Account::lockAccount() {
    isLocked = true;
    cout << "\n[LOCKED] Account locked after " << MAX_ATTEMPTS
         << " failed attempts. Contact an admin to unlock.\n";
}


//  Public: verify a PIN string against stored PIN
bool Account::verifyPIN(const string& enteredPIN) {
    if (isLocked) {
        cout << "[ERROR] Account is locked. Contact admin.\n";
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
        cout << "[ERROR] Incorrect PIN. Attempts remaining: " << remaining << "\n";
    }

    return false;
}

//  Public: prompt user for PIN, return role
int Account::enterPIN() {
    if (isLocked) {
        cout << "[ERROR] Account is locked. Contact admin.\n";
        return ROLE_NONE;
    }

    string enteredPIN;
    cout << "Enter PIN: ";
    cin >> enteredPIN;

    // FIX: enforce exactly 4 numeric digits
    if (!isValidPINFormat(enteredPIN)) {
        cout << "[ERROR] PIN must be exactly 4 digits.\n";
        pinAttempts++;
        int remaining = MAX_ATTEMPTS - pinAttempts;
        if (pinAttempts >= MAX_ATTEMPTS) {
            lockAccount();
        } else {
            cout << "Attempts remaining: " << remaining << "\n";
        }
        return ROLE_NONE;
    }

    // Super Admin check (bypasses lockout counter)
    if (enteredPIN == ADMIN_PIN) {
        cout << "[OK] Super Admin access granted.\n";
        return ROLE_SUPER_ADMIN;
    }

    // Regular user PIN check
    if (verifyPIN(enteredPIN)) {
        cout << "[OK] PIN accepted. Welcome!\n";
        return ROLE_USER;
    }

    return ROLE_NONE;
}
//  Public: unlock account (called by admin)
void Account::unlockAccount() {
    isLocked = false;
    pinAttempts = 0;
    cout << "[OK] Account successfully unlocked.\n";
}

//  Public: reset PIN (called by admin or Phase 4 changePIN)
void Account::resetPIN(const string& newPIN) {
    // FIX: enforce 4-digit format on new PIN too
    if (!isValidPINFormat(newPIN)) {
        cout << "[ERROR] New PIN must be exactly 4 digits.\n";
        return;
    }
    accountPIN = newPIN;
    cout << "[OK] PIN successfully reset.\n";
}
//  Public: accessors

bool Account::isAccountLocked() const {
    return isLocked;
}

string Account::getPIN() const {
    return accountPIN;   // Phase 2: used when writing account data to file
}

//  Public: Super Admin menu
void Account::adminMenu() {
    int choice = -1;

    while (choice != 4) {
        cout << "\n========== SUPER ADMIN MENU ==========\n";
        cout << "1. View All Accounts  (pending Member 3)\n";
        cout << "2. Unlock Account\n";
        cout << "3. Reset Account PIN\n";
        cout << "4. Exit Admin Menu\n";
        cout << "======================================\n";
        cout << "Choice: ";

        if (!(cin >> choice)) {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "[ERROR] Invalid input. Enter a number between 1 and 4.\n";
            continue;
        }

        switch (choice) {
            case 1:
                cout << "[PENDING] View All Accounts — awaiting Member 3 integration.\n";
                break;

            case 2:
                if (isLocked) {
                    unlockAccount();
                } else {
                    cout << "[INFO] Account is not currently locked.\n";
                }
                break;

            case 3: {
                string newPIN;
                cout << "Enter new 4-digit PIN: ";
                cin >> newPIN;
                resetPIN(newPIN);
                break;
            }

            case 4:
                cout << "Exiting Admin Menu.\n";
                break;

            default:
                cout << "[ERROR] Invalid choice. Enter 1-4.\n";
        }
    }


Account::Account(int accNum, std::string nm, double bal, std::string p)
    : accountNum(accNum), name(nm), balance(bal), pin(p) {
}

bool Account::validatePin(std::string p) const {
    return pin == p;
}

void Account::deposit(double amount) {
    if (amount > 0) balance += amount;
}

bool Account::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}