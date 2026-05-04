#pragma once
#include <string>
using namespace std;


const int MAX_ACCOUNTS = 1000;
const int MAX_HISTORY = 10; // Number of transactions in mini-statement
const int STARTING_ACC_NUM = 1001;
const double ATM_WITHDRAWAL_TRANSACTION_LIMIT = 20000.0;
const double ATM_DAILY_WITHDRAWAL_LIMIT = 50000.0;
const int PIN_LENGTH = 4;
const int MAX_PIN_ATTEMPTS = 3;
const int MINI_STATEMENT_COUNT = 5;
const string LOCKED_ACCOUNT_MESSAGE = "Your account has been locked due to multiple failed login attempts. Please contact support or visit the branch to unlock it.";

// Role Constants
const string ADMIN_PIN = "0000";
const int ROLE_NONE = 0;
const int ROLE_USER = 1;
const int ROLE_SUPER_ADMIN = 2;

// Enums for clarity
enum class TransactionType {
    WITHDRAWAL,
    DEPOSIT,
    FAST_CASH,
    TRANSFER,
    PIN_CHANGE,
    ACCOUNT_CREATED,
    BALANCE_INQUIRY
};
enum class ATMState { IDLE, AUTHENTICATING, LOGGED_IN, ADMIN_MODE };
