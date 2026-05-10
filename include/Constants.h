#pragma once
#include <string>
using namespace std;


const int MAX_ACCOUNTS = 1000;
const int MAX_HISTORY = 10; // Number of transactions in mini-statement


// Role Constants
const string ADMIN_PIN = "0000";
const int ROLE_NONE = 0;
const int ROLE_USER = 1;
const int ROLE_SUPER_ADMIN = 2;

// Enums for clarity
enum class TransactionType { WITHDRAWAL, DEPOSIT, FAST_CASH, TRANSFER };
enum class ATMState { IDLE, AUTHENTICATING, LOGGED_IN, ADMIN_MODE };