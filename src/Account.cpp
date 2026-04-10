#include "../include/Account.h"

#include<iostream>
#include<string>
using namespace std;

bool digitPIN4(string arr)//validiation for 4 digit PIN
{
	int digit = 0;
	for (int i = 0; arr[i] != '\0'; i++)
	{
		digit++;
	}
	if (digit == 4)
	{
		return true;
	}
	else
		return false;
}

Account::Account()
{
	accountName = "0000";
	PIN = nullptr;
	balance=0.0;

}

void Account::changePIN()
{
	string PINcheck;
	string PINcheck1;
	int checkentry = 1, answer;

	cout << "For your security choose a unique  PIN" << endl;
	do {
		do {
			cout << "Enter current PIN" << endl;//entering old PIN
			cin >> PINcheck;
			if (!digitPIN4(PINcheck))
			{
				cout << "Error! enter 4 digit pin code" << endl;
			}
		} while (!digitPIN4(PINcheck));

		for (int j = 0; j < 4; j++)
		{
			if (PINcheck[j] != PIN[j])
			{
				cout << "invalid PIN" << endl;
				checkentry = -1;
				break;
			}
		}
		if (checkentry == -1)
		{
			cout << "PIN dont match" << endl;
		}
	} while (checkentry==-1);

	do{
		cout << "Enter new PIN" << endl;//entering New PIN
		cin >> PINcheck;
		if (!digitPIN4(PINcheck))
		{
			cout << "Error! enter 4 digit pin code" << endl;
		}
	} while (!digitPIN4(PINcheck));


	do{
		cout << "Re-enter new PIN to confirm" << endl;//Confirming PIN
			cin >> PINcheck1;
		if (!digitPIN4(PINcheck1))
		{
			cout << "Error! enter 4 digit pin code" << endl;
		}
		else
			if (PINcheck1 != PINcheck)
			{
				cout << "not same PIN" << endl;
				checkentry = -1;
			}

	} while (!digitPIN4(PINcheck1) || checkentry==-1);


	do {
		cout << "1.(Cancel new PIN)   or   2.(Confirm new PIN)" << endl;
		cin >> answer;
		} while (answer != 1 && answer != 2);

	if (answer == 2)
	{
		delete[]PIN;
		for (int k = 0; k < 4; k++)
		{
			PIN[k] = PINcheck[k];
		}
		cout << "PIN changed" << endl;
	}
}

void Account::miniStatement()
{
	cout << "Date         |     " << "Transaction Type       |         " << "Amount    |       " << "Balance     " << endl;

}

Account::~Account()
{
	delete[]PIN;
	PIN = nullptr;
}
Date::Date()//date for miniStatement
{
	day = 1;
	month = 1;
	year = 2026;
}
void Date::incrementDate()
{
	int maxDay;
	if (month == 4 || month == 6 || month == 9 || month == 11)
	{
		maxDay = 30;
	}
	else if (month == 2)
	{
		maxDay = 28;
	}
	else {
		maxDay = 31;
	}

	if (day + 1 < maxDay)
	{
		day++;
	}
	else {
		day = 1;
		if (month + 1 < 12)
		{
			month++;
		}
		else {
			month = 1;
			year++;
		}
	}


}