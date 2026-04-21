#pragma once
#include <string>

class Account {
private:
    bool isLocked;
    int pinAttempts;
    std::string accountPIN;
    static constexpr int MAX_ATTEMPTS = 3;
    void lockAccount();

public:
    Account();
    int enterPIN();
    bool verifyPIN(const std::string& enteredPIN);
    bool isAccountLocked() const;
    
    void unlockAccount(); 
    void resetPIN(const std::string& newPIN);
    void adminMenu();
};
