# atm-simulator-system
ATM Simulation System - OOP Project. 

This GitHub Repository aims to document the learning process associated with building an ATM simulation system from scratch using principles of OOP.

Program Structure:

ATM System

│

├── WelcomeScreen

│     ├── Press 1 → OpenNewAccount()

│     └── Enter Account Number → insertCard()

│

├── Authentication

│     ├── enterPIN()

│     │     ├── Admin PIN → adminMenu()

│     │     └── User PIN → showAccountType()

│     └── 3 wrong attempts → lockout

│

├── showAccountType()  

│     └── proceed to mainMenu()

│

├── mainMenu()

│     ├── 1. Balance

│     ├── 2. Mini Statement

│     ├── 3. Cash Withdrawal

│     ├── 4. Cash Deposit

│     ├── 5. Fast Cash     

│     ├── 6. PIN Change    

│     └── 7. Exit

│

└── adminMenu()        

|  ├── View all accounts
      
|     ├── Search for accounts

|    ├── Check ATM cash level

|   └── Exit
