#ifndef CONSTANTS_H
#define CONSTANTS_H

#include <string>
using namespace std;

// Account limits
const int MAX_ACCOUNTS = 1000;
const int MAX_HISTORY = 10;
const int STARTING_ACC_NUM = 1001;

// Transaction limits
const double DAILY_LIMIT = 25000.0;

// Admin settings
const string ADMIN_PIN = "0000";

// Role constants
const int ROLE_NONE = 0;
const int ROLE_USER = 1;
const int ROLE_SUPER_ADMIN = 2;

// Transaction types
enum class TransactionType
{
    WITHDRAWAL,
    DEPOSIT,
    FAST_CASH,
    TRANSFER
};

// ATM states
enum class ATMState
{
    IDLE,
    AUTHENTICATING,
    LOGGED_IN,
    ADMIN_MODE
};

#endif