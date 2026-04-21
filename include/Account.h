#pragma once
#include <string>
using namespace std;

class Account {
private:
    bool isLocked;
    int pinAttempts;
    string accountPIN;
    static const int MAX_ATTEMPTS = 3;

    void lockAccount();

public:
    Account();

    int enterPIN();
    bool verifyPIN(const string& enteredPIN);
    bool isAccountLocked() const;
    void unlockAccount();
    void resetPIN(const string& newPIN);
    string getPIN() const;        
    void adminMenu();
};