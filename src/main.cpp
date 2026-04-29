#include "../include/ATM.h"
#include "../include/CurrentAccount.h"

int main() {
    // 1. Create ATM with 100,000 cash
    ATM myATM(100000.0);

    // 2. Add a test account (Since we haven't done file handling yet)
    // Account details: Number="1001", PIN="1234", Balance=50000
    myATM.addAccount(new CurrentAccount());

    // 3. Launch system
    myATM.start();

    return 0;
}