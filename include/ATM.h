
#pragma once
#include <iostream>
#include <string>
#include "Account.h"
#include "CurrentAccount.h"
using namespace std;

class ATM {
private:
    Account* currentAccount;   
public:
   // ATM(Account* acc);    
    void withdraw();
    void fastCash();
};