#include "../include/ATM.h"
#include "../include/CurrentAccount.h"

int main() {
    int accountexist;
    cout << "\n====================================\n";
    cout << "      WELCOME TO THE ATM SYSTEM     \n";
    cout << "====================================\n";
    do {
        cout << "1.Create account           2. Existing account" << endl;
        cin >> accountexist;
        if (accountexist != 1 && accountexist != 2)
        {
            cout << "Error! enter 1 or 2" << endl;
        }
    } while (accountexist != 1 && accountexist != 2);

    if (accountexist == 1)
    {
        CurrentAccount user;
        CurrentAccount* accountptr = &user;
        user.createAccount();
         ATM myATM(user.getBalance());
        myATM.addAccount(accountptr);
        // 3. Launch system
        myATM.start();
    }
    else
    {
        ATM myATM;
        myATM.start();
    }


    

    return 0;
}