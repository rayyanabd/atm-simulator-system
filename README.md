# atm-simulator-system
ATM Simulation System - OOP Project. 

This GitHub Repository aims to document the learning process associated with building an ATM simulation system from scratch using principles of OOP.

Qt framework has been used to develop the front-end.

Program Structure:

ATM System

│

├── WelcomeScreen

│     ├──  Open New Account

│     └── Enter Account Number

│

├── Authentication

│     ├── enterPIN()

│     │     ├── Admin PIN → Admin Meni

│     │     └── User PIN → ATM Main Menu

│     └── 3 wrong attempts → lockout

│

│

├── Main Menu

│     ├── 1. Balance Inquiry

│     ├── 2. Mini Statement

│     ├── 3. Cash Withdrawal

│     ├── 4. Cash Deposit

│     ├── 5. Fast Cash     

│     ├── 6. PIN Change    

│     └── 7. Exit

│

└── Admin Menu       

|     ├── View all accounts
      
|     ├── View Account balance

|     ├── Lock/Unlock Accounts

|     └── Exit
