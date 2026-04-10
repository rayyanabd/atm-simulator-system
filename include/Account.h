#pragma once


#include "Constants.h"

#include<string>

class Account
{
	std::string accountName;
	char* PIN;
	double balance;

public:
	Account();
	void changePIN();
	void miniStatement();
	~Account();
};
class Date
{
	int day;
	int month;
	int year;
public:
	Date();
	void incrementDate();
};