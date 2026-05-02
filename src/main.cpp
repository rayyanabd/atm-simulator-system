#include <iostream>
#include "ATM.h"
#include "CurrentAccount.h"

using namespace std;

int main()
{
    // Create ATM with initial cash
    ATM myATM(100000.0);

    // Load saved withdrawal records
    myATM.loadWithdrawals();

    // Add test account
    myATM.addAccount(new CurrentAccount());

    // Start ATM system
    myATM.start();

    // Save withdrawals before exit
    myATM.saveWithdrawals();

    return 0;
}