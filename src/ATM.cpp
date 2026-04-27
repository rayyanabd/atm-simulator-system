#include "ATM.h"
#include <iostream>
using namespace std;

ATM::ATM() : totalAccounts(0), accCounter(STARTING_ACC_NUM) {}

// Logic for searchAcc, openAccount, etc., goes here, 
// interacting with the Account objects instead of arrays.