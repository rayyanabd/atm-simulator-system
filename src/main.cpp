#include "../include/ATM.h"
#include "../include/CurrentAccount.h"

int main() {
    int accountexist;
    cout << "\n====================================\n";
    cout << "      WELCOME TO THE ATM SYSTEM     \n";
    cout << "====================================\n";
    do {
        cout << "1.Create account           2. Existing account   3.Admin portal" << endl;
        cin >> accountexist;
        if (accountexist != 1 && accountexist != 2 && accountexist != 3)
        {
            cout << "Error! enter 1 or 2 or 3" << endl;
        }
    } while (accountexist != 1 && accountexist != 2 && accountexist != 3);

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
    if (accountexist == 2)
    {
        ATM myATM;
        myATM.start();
    }
    if (accountexist == 3)
    {
        ATM myATM;
        myATM.adminPortal();
    }

    

    return 0;
}