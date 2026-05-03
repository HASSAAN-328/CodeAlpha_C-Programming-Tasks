#include <iostream>
#include <vector>
#include <string>
#include <iomanip>
#include <ctime>
#include <sstream>
#include <stdexcept>
#include <algorithm>
#include <map>

using namespace std;

// ============================================================
//  Utility: get current timestamp as string
// ============================================================
string currentTimestamp() {
    time_t now = time(nullptr);
    const char* timeStr = ctime(&now);
    if (!timeStr) return string();
    string ts(timeStr);
    if (!ts.empty() && ts.back() == '\n') ts.pop_back();
    return ts;
}

// ============================================================
//  Class: Transaction
// ============================================================
class Transaction {
public:
    enum class Type { DEPOSIT, WITHDRAWAL, TRANSFER_IN, TRANSFER_OUT };

    Transaction(Type type, double amount, double balanceAfter,
                const string& description)
        : type_(type), amount_(amount), balanceAfter_(balanceAfter),
          description_(description), timestamp_(currentTimestamp()) {}

    // Getters
    Type        getType()         const { return type_; }
    double      getAmount()       const { return amount_; }
    double      getBalanceAfter() const { return balanceAfter_; }
    string      getDescription()  const { return description_; }
    string      getTimestamp()    const { return timestamp_; }

    string typeToString() const {
        switch (type_) {
            case Type::DEPOSIT:       return "Deposit";
            case Type::WITHDRAWAL:    return "Withdrawal";
            case Type::TRANSFER_IN:   return "Transfer In";
            case Type::TRANSFER_OUT:  return "Transfer Out";
        }
        return "Unknown";
    }

    void display() const {
        cout << left
             << setw(16) << typeToString()
             << setw(12) << fixed << setprecision(2) << amount_
             << setw(12) << fixed << setprecision(2) << balanceAfter_
             << description_ << "\n"
             << "   Timestamp: " << timestamp_ << "\n";
    }

private:
    Type   type_;
    double amount_;
    double balanceAfter_;
    string description_;
    string timestamp_;
};

// ============================================================
//  Class: Account
// ============================================================
class Account {
public:
    enum class Type { SAVINGS, CHECKING };

    Account(const string& accountNumber, Type type, double initialBalance = 0.0)
        : accountNumber_(accountNumber), type_(type),
          balance_(initialBalance), isActive_(true) {
        if (initialBalance < 0)
            throw invalid_argument("Initial balance cannot be negative.");
    }

    // ---- Core Operations ----
    void deposit(double amount, const string& description = "Deposit") {
        if (!isActive_) throw runtime_error("Account is inactive.");
        if (amount <= 0)  throw invalid_argument("Deposit amount must be positive.");
        balance_ += amount;
        history_.emplace_back(Transaction::Type::DEPOSIT, amount, balance_, description);
        cout << "  [+] Deposited $" << fixed << setprecision(2) << amount
             << " | New Balance: $" << balance_ << "\n";
    }

    void withdraw(double amount, const string& description = "Withdrawal") {
        if (!isActive_) throw runtime_error("Account is inactive.");
        if (amount <= 0) throw invalid_argument("Withdrawal amount must be positive.");
        if (amount > balance_) throw runtime_error("Insufficient funds.");
        balance_ -= amount;
        history_.emplace_back(Transaction::Type::WITHDRAWAL, amount, balance_, description);
        cout << "  [-] Withdrawn $" << fixed << setprecision(2) << amount
             << " | New Balance: $" << balance_ << "\n";
    }

    // Internal helpers used by transfer logic
    void creditTransfer(double amount, const string& fromAccount) {
        if (!isActive_) throw runtime_error("Destination account is inactive.");
        balance_ += amount;
        history_.emplace_back(Transaction::Type::TRANSFER_IN, amount, balance_,
                               "Transfer from " + fromAccount);
    }

    void debitTransfer(double amount, const string& toAccount) {
        if (!isActive_) throw runtime_error("Source account is inactive.");
        if (amount <= 0) throw invalid_argument("Transfer amount must be positive.");
        if (amount > balance_) throw runtime_error("Insufficient funds for transfer.");
        balance_ -= amount;
        history_.emplace_back(Transaction::Type::TRANSFER_OUT, amount, balance_,
                               "Transfer to " + toAccount);
    }

    // ---- Display ----
    void displayInfo() const {
        cout << "\n  Account Number : " << accountNumber_
             << "\n  Type           : " << typeToString()
             << "\n  Balance        : $" << fixed << setprecision(2) << balance_
             << "\n  Status         : " << (isActive_ ? "Active" : "Inactive") << "\n";
    }

    void displayRecentTransactions(int n = 5) const {
        int total = static_cast<int>(history_.size());
        int start = max(0, total - n);
        cout << "\n  --- Last " << min(n, total) << " transaction(s) for account "
             << accountNumber_ << " ---\n"
             << left
             << setw(16) << "Type"
             << setw(12) << "Amount($)"
             << setw(12) << "Balance($)"
             << "Description\n"
             << string(60, '-') << "\n";
        for (int i = start; i < total; ++i)
            history_[i].display();
        if (total == 0) cout << "  No transactions yet.\n";
    }

    void displayAllTransactions() const {
        cout << "\n  --- Full Transaction History for " << accountNumber_ << " ---\n"
             << left
             << setw(16) << "Type"
             << setw(12) << "Amount($)"
             << setw(12) << "Balance($)"
             << "Description\n"
             << string(60, '-') << "\n";
        for (const auto& t : history_) t.display();
        if (history_.empty()) cout << "  No transactions yet.\n";
    }

    // ---- Getters ----
    string getAccountNumber() const { return accountNumber_; }
    double getBalance()       const { return balance_; }
    bool   isActive()         const { return isActive_; }
    Type   getType()          const { return type_; }

    void setActive(bool status) { isActive_ = status; }

    string typeToString() const {
        return type_ == Type::SAVINGS ? "Savings" : "Checking";
    }

private:
    string               accountNumber_;
    Type                 type_;
    double               balance_;
    bool                 isActive_;
    vector<Transaction>  history_;
};

// ============================================================
//  Class: Customer
// ============================================================
class Customer {
public:
    Customer(const string& customerId, const string& name,
             const string& email, const string& phone)
        : customerId_(customerId), name_(name),
          email_(email), phone_(phone) {}

    // ---- Account Management ----
    void addAccount(Account* account) {
        accounts_.push_back(account);
    }

    Account* getAccount(const string& accountNumber) {
        for (auto* acc : accounts_)
            if (acc->getAccountNumber() == accountNumber) return acc;
        return nullptr;
    }

    // ---- Display ----
    void displayInfo() const {
        cout << "\n  Customer ID : " << customerId_
             << "\n  Name        : " << name_
             << "\n  Email       : " << email_
             << "\n  Phone       : " << phone_
             << "\n  Accounts    : " << accounts_.size() << "\n";
    }

    void displayAccounts() const {
        if (accounts_.empty()) { cout << "  No accounts linked.\n"; return; }
        for (const auto* acc : accounts_) acc->displayInfo();
    }

    // ---- Getters ----
    string getCustomerId() const { return customerId_; }
    string getName()       const { return name_; }
    string getEmail()      const { return email_; }

    const vector<Account*>& getAccounts() const { return accounts_; }

private:
    string          customerId_;
    string          name_;
    string          email_;
    string          phone_;
    vector<Account*> accounts_;
};

// ============================================================
//  Class: Bank  (central manager)
// ============================================================
class Bank {
public:
    Bank(const string& name) : bankName_(name),
        nextCustomerId_(1000), nextAccountId_(100000) {}

    ~Bank() {
        for (auto& p : customers_) delete p.second;
        for (auto& p : accounts_)  delete p.second;
    }

    // ---- Customer Operations ----
    Customer* createCustomer(const string& name, const string& email,
                              const string& phone) {
        string id = "CUST" + to_string(nextCustomerId_++);
        auto* c   = new Customer(id, name, email, phone);
        customers_[id] = c;
        cout << "\n  Customer created successfully! ID: " << id << "\n";
        return c;
    }

    Customer* findCustomer(const string& customerId) {
        auto it = customers_.find(customerId);
        return (it != customers_.end()) ? it->second : nullptr;
    }

    // ---- Account Operations ----
    Account* createAccount(const string& customerId,
                            Account::Type type,
                            double initialDeposit = 0.0) {
        Customer* cust = findCustomer(customerId);
        if (!cust) throw runtime_error("Customer not found: " + customerId);

        string accNum = to_string(nextAccountId_++);
        auto* acc = new Account(accNum, type, initialDeposit);
        accounts_[accNum] = acc;
        cust->addAccount(acc);

        if (initialDeposit > 0)
            acc->deposit(initialDeposit, "Initial deposit");

        cout << "  Account " << accNum << " (" << acc->typeToString()
             << ") created for " << cust->getName() << "\n";
        return acc;
    }

    Account* findAccount(const string& accountNumber) {
        auto it = accounts_.find(accountNumber);
        return (it != accounts_.end()) ? it->second : nullptr;
    }

    // ---- Transfer ----
    void transfer(const string& fromAccNum, const string& toAccNum, double amount) {
        Account* from = findAccount(fromAccNum);
        Account* to   = findAccount(toAccNum);
        if (!from) throw runtime_error("Source account not found: "      + fromAccNum);
        if (!to)   throw runtime_error("Destination account not found: " + toAccNum);

        from->debitTransfer(amount, toAccNum);
        to->creditTransfer(amount, fromAccNum);

        cout << "  [<->] Transferred $" << fixed << setprecision(2) << amount
             << " from " << fromAccNum << " to " << toAccNum << "\n";
    }

    // ---- Reporting ----
    void displayAllCustomers() const {
        cout << "\n=== All Customers (" << customers_.size() << ") ===\n";
        for (const auto& p : customers_) p.second->displayInfo();
    }

    void displayAllAccounts() const {
        cout << "\n=== All Accounts (" << accounts_.size() << ") ===\n";
        for (const auto& p : accounts_) p.second->displayInfo();
    }

    string getBankName() const { return bankName_; }

private:
    string                  bankName_;
    int                     nextCustomerId_;
    int                     nextAccountId_;
    map<string, Customer*>  customers_;
    map<string, Account*>   accounts_;
};

// ============================================================
//  Helper: pretty section header
// ============================================================
void printSection(const string& title) {
    cout << "\n" << string(60, '=') << "\n  " << title
         << "\n" << string(60, '=') << "\n";
}

// ============================================================
//  main(): Demonstration
// ============================================================
int main() {
    cout << fixed << setprecision(2);

    Bank bank("Nexus National Bank");
    cout << "\n  Welcome to " << bank.getBankName() << "\n";

    // ----------------------------------------------------------
    //  1. Create Customers
    // ----------------------------------------------------------
    printSection("1. Creating Customers");
    Customer* alice = bank.createCustomer("Alice Johnson", "alice@email.com", "555-0101");
    Customer* bob   = bank.createCustomer("Bob Smith",    "bob@email.com",   "555-0202");
    Customer* carol = bank.createCustomer("Carol White",  "carol@email.com", "555-0303");

    // ----------------------------------------------------------
    //  2. Create Accounts
    // ----------------------------------------------------------
    printSection("2. Creating Accounts");
    Account* aliceSavings  = bank.createAccount(alice->getCustomerId(), Account::Type::SAVINGS,   1000.00);
    Account* aliceChecking = bank.createAccount(alice->getCustomerId(), Account::Type::CHECKING,   500.00);
    Account* bobChecking   = bank.createAccount(bob->getCustomerId(),   Account::Type::CHECKING,   750.00);
    Account* carolSavings  = bank.createAccount(carol->getCustomerId(), Account::Type::SAVINGS,   2000.00);

    // ----------------------------------------------------------
    //  3. Deposits
    // ----------------------------------------------------------
    printSection("3. Deposits");
    aliceSavings->deposit(500.00, "Salary credit");
    bobChecking->deposit(200.00,  "Freelance payment");
    carolSavings->deposit(1000.00, "Bonus credit");

    // ----------------------------------------------------------
    //  4. Withdrawals
    // ----------------------------------------------------------
    printSection("4. Withdrawals");
    aliceChecking->withdraw(100.00, "Grocery shopping");
    bobChecking->withdraw(50.00,    "ATM withdrawal");

    try {
        bobChecking->withdraw(99999.00, "Should fail");
    } catch (const runtime_error& e) {
        cout << "  [Error] " << e.what() << "\n";
    }

    // ----------------------------------------------------------
    //  5. Fund Transfers
    // ----------------------------------------------------------
    printSection("5. Fund Transfers");
    bank.transfer(aliceSavings->getAccountNumber(),
                  bobChecking->getAccountNumber(), 300.00);

    bank.transfer(carolSavings->getAccountNumber(),
                  aliceChecking->getAccountNumber(), 150.00);

    try {
        bank.transfer(bobChecking->getAccountNumber(),
                      carolSavings->getAccountNumber(), 999999.00);
    } catch (const runtime_error& e) {
        cout << "  [Error] " << e.what() << "\n";
    }

    // ----------------------------------------------------------
    //  6. Account Information & Balances
    // ----------------------------------------------------------
    printSection("6. Account Information");
    cout << "\n--- Alice's Accounts ---";
    alice->displayAccounts();

    cout << "\n--- Bob's Accounts ---";
    bob->displayAccounts();

    cout << "\n--- Carol's Accounts ---";
    carol->displayAccounts();

    // ----------------------------------------------------------
    //  7. Transaction History
    // ----------------------------------------------------------
    printSection("7. Recent Transactions (last 5)");
    aliceSavings->displayRecentTransactions(5);
    bobChecking->displayRecentTransactions(5);

    printSection("8. Full Transaction History");
    carolSavings->displayAllTransactions();

    // ----------------------------------------------------------
    //  9. All Customers & Accounts summary
    // ----------------------------------------------------------
    printSection("9. Bank Summary");
    bank.displayAllCustomers();
    bank.displayAllAccounts();

    cout << "\n" << string(60, '=')
         << "\n  Thank you for using " << bank.getBankName() << "!\n"
         << string(60, '=') << "\n";

    return 0;
}
