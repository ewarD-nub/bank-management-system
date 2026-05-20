# bank-management-system

A CLI-based banking management system written in C++ that allows users to create and manage multiple bank accounts on a local computer.

The project was initially developed as a basic banking application and later refactored into a cleaner object-oriented system with support for transaction history, money transfer, loans, fixed deposits, and persistent file storage.

The program allows users to create a new bank account or log into an existing account. During account creation, the user enters details such as name and password, after which a unique account number is generated automatically. All account information is stored in a local file (`accounts.txt`) which acts as the system database.

To access an existing account, the user must enter the correct account number and password. Once logged in, the user can perform several banking operations through a menu-driven interface.

The available operations include:

- Deposit Money
- Withdraw Money
- Transfer Money
- Check Balance
- View Transaction History
- Change Password
- Apply for Loans
- Fixed Deposit
- Log Out

Depositing and withdrawing money updates the user balance immediately and stores the updated data in the file system.

The transfer feature allows users to send money from one account to another using the receiver's account number.

The transaction history system records deposits, withdrawals, and transfers along with timestamps.

Changing the password requires the user to verify the current password before setting a new one.

The loan system supports:

- Personal Loan
- Car Loan
- Home Loan

The system calculates loan interest rates and repayment amounts based on the selected loan type and amount entered by the user.

The fixed deposit feature allows users to deposit money for a selected duration and calculates maturity amount using simple interest.

After every operation, account details are automatically saved to the storage file to ensure data persistence.

The logout option returns the user back to the main menu where they can either create another account, login again, or exit the application.

---

# Features

- Multiple account support
- Secure login system
- Unique account number generation
- Deposit and withdrawal system
- Money transfer between accounts
- Loan management system
- Fixed deposit simulation
- Transaction history tracking
- Password change functionality
- Persistent file storage using `accounts.txt`

---

# Technologies Used

- C++17
- Object-Oriented Programming (OOP)
- STL (`vector`, `string`, `fstream`)
- File handling
- Random number generation

---

# How to Compile

```bash
g++ -std=c++17 main.cpp -o bank