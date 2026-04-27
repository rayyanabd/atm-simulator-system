#pragma once

#include "SavingsAccount.h"
#include "CurrentAccount.h"
#include "Constants.h"

class ATM {

private:
	 
};
#ifndef ATM_H
#define ATM_H

#include <string>

bool checkPin(std::string p);
int searchAcc(int num);
double getAmount(std::string msg);
int getInt(std::string msg);

#endif