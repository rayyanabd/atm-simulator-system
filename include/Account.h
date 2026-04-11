
#pragma once
#include <iostream>
#include <string>
using namespace std;
class Account {
protected:
	string accountName;
	char* PIN;
	double balance;
public:
	Account() {
		accountName = "";
		PIN = nullptr;
		balance = 0.0;
	}
	double getBalance() { 
		return balance; 
	}
	string getAccountName() { 
		return accountName;
	}
	// to be implemented by others
	virtual string getAccountType() = 0;
	virtual bool debit(double amount) = 0;
};