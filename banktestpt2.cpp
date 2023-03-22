#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;

class BankAccount {
public:
    string accountHolderName;
    double balance;
    vector<double> transactions;
    double getBalance(){ 
        return balance;
        }

    BankAccount(string accountHolderName, double balance) {
        this->accountHolderName = accountHolderName;
        this->balance = balance;
    }

    void deposit(double amount) {
        balance += amount;
        transactions.push_back(amount);
        cout << "Deposit of $" << amount << " successful." << endl;
    }

    void withdraw(double amount) {
        if (balance < amount) {
            cout << "Insufficient funds." << endl;
        } else {
            balance -= amount;
            transactions.push_back(-amount);
            cout << "Withdrawal of $" << amount << " successful." << endl;
        }
    }

    void printTransactions() {
        cout << "Transaction history for account holder " << accountHolderName << ":" << endl;
        for (double transaction : transactions) {
            if (transaction > 0) {
                cout << "Deposit of $" << transaction << endl;
            } else {
                cout << "Withdrawal of $" << -transaction << endl;
            }
        }
    }

    void writeToStream(ofstream& stream) {
        stream << accountHolderName << endl;
        stream << balance << endl;
        stream << transactions.size() << endl;
        for (double transaction : transactions) {
            stream << transaction << endl;
        }
    }

    static BankAccount* readFromStream(ifstream& stream) {
        string accountHolderName;
        double balance;
        int numTransactions;

        getline(stream, accountHolderName);
        stream >> balance;
        stream >> numTransactions;

        BankAccount* account = new BankAccount(accountHolderName, balance);

        for (int i = 0; i < numTransactions; i++) {
            double transaction;
            stream >> transaction;
            account->transactions.push_back(transaction);
        }

        return account;
    }
};

class Bank {
private:
    vector<BankAccount*> accounts;

public:
    Bank() {
        ifstream inputFile("banking.txt");
        if (inputFile.is_open()) {
            while (!inputFile.eof()) {
                BankAccount* account = BankAccount::readFromStream(inputFile);
                if (account != nullptr) {
                    accounts.push_back(account);
                }
            }
            inputFile.close();
        }
    }

    ~Bank() {
        ofstream outputFile("banking.txt");
        if (outputFile.is_open()) {
            for (BankAccount* account : accounts) {
                account->writeToStream(outputFile);
                delete account;
            }
            outputFile.close();
        }
    }

    void addAccount(string accountHolderName, double balance) {
        BankAccount* account = new BankAccount(accountHolderName, balance);
        accounts.push_back(account);
    }

    BankAccount* findAccount(string accountHolderName) {
        for (BankAccount* account : accounts) {
            if (account->accountHolderName == accountHolderName) {
                return account;
            }
        }
        return nullptr;
    }
};

int main() {
    Bank bank;

    while (true) {
        cout << "Enter your name or enter 'quit' to exit the program: ";
        string name;
        cin >> name;
        if (name == "quit") {
            break;
        }

        BankAccount* account = bank.findAccount(name);
        if (account == nullptr) {
            cout << "No account found for " << name << ". Creating new account." << endl;
            cout << "Enter initial balance: ";
            double balance;
            cin >> balance;
            bank.addAccount(name, balance);
            account = bank.findAccount(name);
        }

        while (true) {
            cout << "Enter 'b' to view balance, 'd' to deposit, 'w' to withdraw, 't' to view transaction history, or 'quit' to exit: ";
            string action;
            cin >> action;
            if (action == "quit") {
                break;
            } else if (action == "d") {
                cout << "Enter deposit amount: ";
                double amount;
                cin >> amount;
                account->deposit(amount);
            } else if (action == "w") {
                cout << "Enter withdrawal amount: ";
                double amount;
                cin >> amount;
                account->withdraw(amount);
            } else if (action == "t") {
                account->printTransactions();
            } else if (action == "b"){
                cout << "Current balance: $" << account->getBalance() << endl;
            } else {
                cout << "Invalid action. Please try again." << endl;
            }
        }
    }    
    system("cls");
    return 0;
}  