#include "../include/ATM.h"
#include "../include/CurrentAccount.h"

int main() {
    int accountexist;
    ATM* myATM = nullptr;
    CurrentAccount* user = nullptr;
    cout << "\n====================================\n";
    cout << "      WELCOME TO THE ATM SYSTEM     \n";
    cout << "====================================\n";
    do {
        cout << "1.Create account           2. Existing account       3.Admin portal" << endl;
        cin >> accountexist;
        if (accountexist != 1 && accountexist != 2 && accountexist != 3)
        {
            cout << "Error! enter 1 or 2 or 3" << endl;
        }
    } while (accountexist != 1 && accountexist != 2 && accountexist != 3);
   
    if (accountexist == 1)
    {
       
        CurrentAccount* user = new CurrentAccount;

        user->createAccount();

        ATM* myATM = new ATM();
        myATM->start();
    }
    if (accountexist == 2)
    {
        ATM* myATM = new ATM();

        myATM->start();
    }
    if (accountexist == 3)
    {
        ATM* myATM = new ATM();

        myATM->adminPortal();
    }
    if (myATM != nullptr)
    {
        delete myATM;
        myATM = nullptr;
    }
    if (user != nullptr)
    {
        delete user;
        user = nullptr;
    }
   
    return 0;
}