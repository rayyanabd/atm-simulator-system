
#include "../include/ATM.h"
#include "../include/CurrentAccount.h"
using namespace std;

int main() {
    ATM myATM(100000.0);

    CurrentAccount* acc = new CurrentAccount();
    acc->setAccountNumber("1001");
    acc->setPin("1234");
    acc->setBalance(50000.0);

    acc->loadDailyLimit();

    myATM.addAccount(acc);
    myATM.start();

    return 0;
}