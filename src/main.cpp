#include "../include/ATM.h"

int main() {
    // 1. Create ATM with 100,000 cash
    ATM myATM(100000.0);

    // 2. Load persistent account and transaction data.
    // If no data files exist yet, the backend creates starter accounts.
    myATM.loadData();

    // 3. Launch system
    myATM.start();

    return 0;
}
