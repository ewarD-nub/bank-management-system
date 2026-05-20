#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <algorithm>

using namespace std;

using ll = long long;

class Transaction {
public:
    string type;
    double amount;
    string timestamp;

    Transaction() {}

    Transaction(string type, double amount, string timestamp)
        : type(type), amount(amount), timestamp(timestamp) {}
};

class Account {
private:
    string name;
    string password;
    ll accountNumber;
    double balance;
    vector<Transaction> history;

public:
    Account() {
        balance = 0;
    }

    Account(
        const string &name,
        const string &password,
        ll accountNumber,
        double balance
    ) {
        this->name = name;
        this->password = password;
        this->accountNumber = accountNumber;
        this->balance = balance;
    }

    string getName() const {
        return name;
    }

    string getPassword() const {
        return password;
    }

    ll getAccountNumber() const {
        return accountNumber;
    }

    double getBalance() const {
        return balance;
    }

    void setPassword(const string &newPassword) {
        password = newPassword;
    }

    bool checkPassword(const string &pass) const {
        return password == pass;
    }

    void deposit(double amount) {
        balance += amount;
        addTransaction("Deposit", amount);
    }

    bool withdraw(double amount) {
        if (amount > balance)
            return false;

        balance -= amount;
        addTransaction("Withdraw", amount);
        return true;
    }

    bool transfer(Account &receiver, double amount) {
        if (amount > balance)
            return false;

        balance -= amount;
        receiver.balance += amount;

        addTransaction(
            "Transferred to " + to_string(receiver.accountNumber),
            amount
        );

        receiver.addTransaction(
            "Received from " + to_string(accountNumber),
            amount
        );

        return true;
    }

    void addTransaction(const string &type, double amount) {
        history.push_back(
            Transaction(type, amount, currentTime())
        );
    }

    static string currentTime() {
        time_t now = time(nullptr);
        string timeStr = ctime(&now);

        if (!timeStr.empty() && timeStr.back() == '\n')
            timeStr.pop_back();

        return timeStr;
    }

    void display() const {
        cout << "\n===== Account Details =====\n";
        cout << "Name          : " << name << '\n';
        cout << "Account Number: " << accountNumber << '\n';
        cout << "Balance       : " << fixed << setprecision(2)
             << balance << '\n';
    }

    void showTransactions() const {
        cout << "\n===== Transaction History =====\n";

        if (history.empty()) {
            cout << "No transactions found.\n";
            return;
        }

        for (const auto &t : history) {
            cout << t.timestamp << " | "
                 << t.type << " | "
                 << t.amount << '\n';
        }
    }

    string serialize() const {
        stringstream ss;

        ss << name << '\n';
        ss << password << '\n';
        ss << accountNumber << '\n';
        ss << balance << '\n';

        return ss.str();
    }

    static Account deserialize(ifstream &fin) {
        Account acc;

        getline(fin, acc.name);
        getline(fin, acc.password);

        fin >> acc.accountNumber;
        fin >> acc.balance;
        fin.ignore();

        return acc;
    }
};

class BankSystem {
private:
    vector<Account> accounts;

public:
    void loadAccounts() {
        ifstream fin("accounts.txt");

        if (!fin)
            return;

        while (fin.peek() != EOF) {
            Account acc = Account::deserialize(fin);

            if (!acc.getName().empty())
                accounts.push_back(acc);
        }
    }

    void saveAccounts() {
        ofstream fout("accounts.txt");

        for (const auto &acc : accounts)
            fout << acc.serialize();
    }

    ll generateAccountNumber() {
        static mt19937_64 rng(
            chrono::steady_clock::now().time_since_epoch().count()
        );

        ll number;

        do {
            number = 1000000000LL + rng() % 9000000000LL;
        } while (findAccount(number) != nullptr);

        return number;
    }

    void createAccount() {
        string name;
        string password;
        double balance;

        cout << "Enter your name: ";
        getline(cin, name);

        while (true) {
            string p1, p2;

            cout << "Enter password: ";
            getline(cin, p1);

            cout << "Confirm password: ";
            getline(cin, p2);

            if (p1 != p2) {
                cout << "Passwords do not match.\n";
            } else {
                password = p1;
                break;
            }
        }

        cout << "Enter initial balance: ";
        cin >> balance;
        cin.ignore();

        ll accNo = generateAccountNumber();

        Account acc(name, password, accNo, balance);

        accounts.push_back(acc);
        saveAccounts();

        cout << "\nAccount created successfully!\n";
        cout << "Your Account Number: " << accNo << '\n';
    }

    Account* login() {
        ll accNo;
        string password;

        cout << "Enter account number: ";
        cin >> accNo;
        cin.ignore();

        cout << "Enter password: ";
        getline(cin, password);

        for (auto &acc : accounts) {
            if (
                acc.getAccountNumber() == accNo &&
                acc.checkPassword(password)
            ) {
                cout << "Login successful!\n";
                return &acc;
            }
        }

        cout << "Invalid credentials.\n";
        return nullptr;
    }

    Account* findAccount(ll accountNo) {
        for (auto &acc : accounts) {
            if (acc.getAccountNumber() == accountNo)
                return &acc;
        }

        return nullptr;
    }

    void transferMoney(Account &sender) {
        ll receiverNo;
        double amount;

        cout << "Enter receiver account number: ";
        cin >> receiverNo;

        cout << "Enter amount: ";
        cin >> amount;
        cin.ignore();

        Account *receiver = findAccount(receiverNo);

        if (receiver == nullptr) {
            cout << "Receiver account not found.\n";
            return;
        }

        if (sender.transfer(*receiver, amount)) {
            saveAccounts();
            cout << "Transfer successful!\n";
        } else {
            cout << "Insufficient balance.\n";
        }
    }

    void changePassword(Account &acc) {
        string oldPass;

        cout << "Enter current password: ";
        getline(cin, oldPass);

        if (!acc.checkPassword(oldPass)) {
            cout << "Wrong password.\n";
            return;
        }

        string p1, p2;

        cout << "Enter new password: ";
        getline(cin, p1);

        cout << "Confirm new password: ";
        getline(cin, p2);

        if (p1 != p2) {
            cout << "Passwords do not match.\n";
            return;
        }

        acc.setPassword(p1);

        saveAccounts();

        cout << "Password updated successfully!\n";
    }

    void accountMenu(Account &acc) {
        while (true) {
            cout << "\n===== Banking Menu =====\n";
            cout << "1. Deposit\n";
            cout << "2. Withdraw\n";
            cout << "3. Transfer Money\n";
            cout << "4. Check Balance\n";
            cout << "5. Transaction History\n";
            cout << "6. Change Password\n";
            cout << "7. Logout\n";
            cout << "Enter choice: ";

            int choice;
            cin >> choice;
            cin.ignore();

            switch (choice) {
                case 1: {
                    double amount;

                    cout << "Enter amount: ";
                    cin >> amount;
                    cin.ignore();

                    acc.deposit(amount);
                    saveAccounts();

                    cout << "Deposit successful!\n";
                    break;
                }

                case 2: {
                    double amount;

                    cout << "Enter amount: ";
                    cin >> amount;
                    cin.ignore();

                    if (acc.withdraw(amount)) {
                        saveAccounts();
                        cout << "Withdrawal successful!\n";
                    } else {
                        cout << "Insufficient balance.\n";
                    }

                    break;
                }

                case 3:
                    transferMoney(acc);
                    break;

                case 4:
                    acc.display();
                    break;

                case 5:
                    acc.showTransactions();
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

    void mainMenu() {
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
                    return;

                default:
                    cout << "Invalid choice.\n";
            }
        }
    }
};

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    BankSystem bank;
    bank.mainMenu();

    return 0;
}