#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>

using namespace std;

using ll = long long;

class Account {
public:
    string name;
    string password;
    ll account_no;
    double balance;

    void display() const {
        cout << "\n===== Account Details =====\n";
        cout << "Name          : " << name << '\n';
        cout << "Account Number: " << account_no << '\n';
        cout << "Balance       : " << fixed << setprecision(2)
             << balance << '\n';
    }
};

vector<Account> accounts;

ll generateAccountNumber() {
    static mt19937_64 rng(
        chrono::steady_clock::now().time_since_epoch().count()
    );

    return 1000000000LL + rng() % 9000000000LL;
}

double personalLoanRate(double amount) {
    if (amount > 100000)
        return 10;
    else if (amount > 50000)
        return 7;
    else if (amount > 10000)
        return 5;

    return 12;
}

double carLoanRate(double amount) {
    if (amount > 1000000)
        return 6.1;
    else if (amount > 500000)
        return 5.7;
    else if (amount > 250000)
        return 5;

    return 6.7;
}

double homeLoanRate(double amount) {
    if (amount > 5000000)
        return 10;
    else if (amount > 3000000)
        return 9.5;
    else if (amount > 2000000)
        return 9;

    return 11;
}

void saveAccounts() {
    ofstream fout("accounts.txt");

    for (const auto &acc : accounts) {
        fout << acc.name << '\n';
        fout << acc.password << '\n';
        fout << acc.account_no << '\n';
        fout << acc.balance << '\n';
    }
}

void loadAccounts() {
    ifstream fin("accounts.txt");

    if (!fin)
        return;

    Account acc;

    while (getline(fin, acc.name)) {
        getline(fin, acc.password);
        fin >> acc.account_no;
        fin >> acc.balance;
        fin.ignore();

        accounts.push_back(acc);
    }
}

void createAccount() {
    Account acc;

    cout << "Enter your name: ";
    getline(cin, acc.name);

    while (true) {
        string p1, p2;

        cout << "Enter password: ";
        getline(cin, p1);

        cout << "Confirm password: ";
        getline(cin, p2);

        if (p1 != p2) {
            cout << "Passwords do not match. Try again.\n";
        } else {
            acc.password = p1;
            break;
        }
    }

    acc.account_no = generateAccountNumber();

    cout << "Enter initial balance: ";
    cin >> acc.balance;
    cin.ignore();

    accounts.push_back(acc);
    saveAccounts();

    cout << "\nAccount created successfully!\n";
    acc.display();
}

Account* login() {
    ll acc_no;
    string pass;

    cout << "Enter account number: ";
    cin >> acc_no;
    cin.ignore();

    cout << "Enter password: ";
    getline(cin, pass);

    for (auto &acc : accounts) {
        if (acc.account_no == acc_no && acc.password == pass) {
            cout << "Login successful!\n";
            return &acc;
        }
    }

    cout << "Invalid credentials.\n";
    return nullptr;
}

void deposit(Account &acc) {
    double amount;

    cout << "Enter amount to deposit: ";
    cin >> amount;
    cin.ignore();

    acc.balance += amount;

    saveAccounts();

    cout << "Deposit successful!\n";
}

void withdrawMoney(Account &acc) {
    double amount;

    cout << "Enter amount to withdraw: ";
    cin >> amount;
    cin.ignore();

    if (amount > acc.balance) {
        cout << "Insufficient balance.\n";
        return;
    }

    acc.balance -= amount;

    saveAccounts();

    cout << "Withdrawal successful!\n";
}

void fixedDeposit(Account &acc) {
    double amount;
    int years;

    cout << "Enter amount for fixed deposit: ";
    cin >> amount;

    if (amount > acc.balance) {
        cout << "Insufficient balance.\n";
        cin.ignore();
        return;
    }

    cout << "Enter time in years: ";
    cin >> years;
    cin.ignore();

    double rate = homeLoanRate(amount);
    double maturity = amount + (amount * rate * years / 100.0);

    acc.balance -= amount;

    saveAccounts();

    cout << "Interest Rate : " << rate << "%\n";
    cout << "Maturity Value: " << maturity << '\n';
}

void applyLoan(Account &acc) {
    int type;
    double amount;
    int years;

    cout << "\n1. Personal Loan\n";
    cout << "2. Car Loan\n";
    cout << "3. Home Loan\n";
    cout << "Enter choice: ";
    cin >> type;

    cout << "Enter loan amount: ";
    cin >> amount;

    cout << "Enter time in years: ";
    cin >> years;
    cin.ignore();

    double rate;

    if (type == 1)
        rate = personalLoanRate(amount);
    else if (type == 2)
        rate = carLoanRate(amount);
    else
        rate = homeLoanRate(amount);

    double total = amount + (amount * rate * years / 100.0);

    acc.balance += amount;

    saveAccounts();

    cout << "Loan Approved!\n";
    cout << "Interest Rate : " << rate << "%\n";
    cout << "Total Repayment: " << total << '\n';
}

void changePassword(Account &acc) {
    string oldPass;

    cout << "Enter current password: ";
    getline(cin, oldPass);

    if (oldPass != acc.password) {
        cout << "Wrong password.\n";
        return;
    }

    string newPass1, newPass2;

    cout << "Enter new password: ";
    getline(cin, newPass1);

    cout << "Confirm new password: ";
    getline(cin, newPass2);

    if (newPass1 != newPass2) {
        cout << "Passwords do not match.\n";
        return;
    }

    acc.password = newPass1;

    saveAccounts();

    cout << "Password changed successfully!\n";
}

void accountMenu(Account &acc) {
    while (true) {
        cout << "\n===== Banking Menu =====\n";
        cout << "1. Deposit Money\n";
        cout << "2. Withdraw Money\n";
        cout << "3. Fixed Deposit\n";
        cout << "4. Apply Loan\n";
        cout << "5. Check Balance\n";
        cout << "6. Change Password\n";
        cout << "7. Logout\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                deposit(acc);
                break;

            case 2:
                withdrawMoney(acc);
                break;

            case 3:
                fixedDeposit(acc);
                break;

            case 4:
                applyLoan(acc);
                break;

            case 5:
                acc.display();
                break;

            case 6:
                changePassword(acc);
                break;

            case 7:
                return;

            default:
                cout << "Invalid choice.\n";
        }
    }
}

int main() {
    loadAccounts();

    while (true) {
        cout << "\n========================\n";
        cout << "       Cyber Bank       \n";
        cout << "========================\n";

        cout << "1. Create Account\n";
        cout << "2. Login\n";
        cout << "3. Exit\n";
        cout << "Enter choice: ";

        int choice;
        cin >> choice;
        cin.ignore();

        switch (choice) {
            case 1:
                createAccount();
                break;

            case 2: {
                Account *acc = login();

                if (acc != nullptr)
                    accountMenu(*acc);

                break;
            }

            case 3:
                cout << "Thank you for banking with us!\n";
                return 0;

            default:
                cout << "Invalid choice.\n";
        }
    }
}