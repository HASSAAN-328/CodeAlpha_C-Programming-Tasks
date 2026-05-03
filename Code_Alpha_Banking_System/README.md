# 🏦 Banking System — C++

A fully object-oriented **Banking System** implemented in C++17, featuring customer management, multiple account types, deposits, withdrawals, fund transfers, and complete transaction history tracking.

---

## 📋 Table of Contents

- [Features](#-features)
- [Class Architecture](#-class-architecture)
- [Getting Started](#-getting-started)
- [Usage / Demo Output](#-usage--demo-output)
- [Project Structure](#-project-structure)
- [Error Handling](#-error-handling)
- [Concepts Demonstrated](#-concepts-demonstrated)
- [Future Improvements](#-future-improvements)
- [License](#-license)

---

## ✨ Features

- ✅ Create and manage **customers** (name, email, phone)
- ✅ Open **Savings** or **Checking** accounts with initial deposits
- ✅ **Deposit** and **withdraw** funds with validation
- ✅ **Transfer funds** between any two accounts
- ✅ Full **transaction history** per account (type, amount, running balance, timestamp)
- ✅ View **recent N transactions** or the **complete history**
- ✅ **Account info display** — balance, type, status
- ✅ **Bank-wide summary** — all customers and all accounts
- ✅ Robust **exception handling** for invalid operations

---

## 🏗 Class Architecture

```
Bank
 ├── Customer (1..*)
 │    └── Account (1..*)
 │         └── Transaction (0..*)
```

### `Transaction`
Represents a single financial event.

| Attribute       | Type     | Description                              |
|-----------------|----------|------------------------------------------|
| `type_`         | `enum`   | DEPOSIT, WITHDRAWAL, TRANSFER_IN/OUT     |
| `amount_`       | `double` | Transaction amount                       |
| `balanceAfter_` | `double` | Running balance snapshot after the event |
| `description_`  | `string` | Human-readable label                     |
| `timestamp_`    | `string` | Auto-captured creation time             |

### `Account`
Manages funds and transaction records for one account.

| Method                         | Description                             |
|--------------------------------|-----------------------------------------|
| `deposit(amount, desc)`        | Credits the account                     |
| `withdraw(amount, desc)`       | Debits with balance check               |
| `creditTransfer(amount, from)` | Internal — used by `Bank::transfer()`   |
| `debitTransfer(amount, to)`    | Internal — used by `Bank::transfer()`   |
| `displayRecentTransactions(n)` | Shows last N transactions               |
| `displayAllTransactions()`     | Shows full history                      |
| `displayInfo()`                | Prints number, type, balance, status    |

### `Customer`
Holds personal info and a list of linked accounts.

| Attribute      | Type              | Description                    |
|----------------|-------------------|--------------------------------|
| `customerId_`  | `string`          | Auto-generated (e.g. CUST1000) |
| `name_`        | `string`          | Full name                      |
| `email_`       | `string`          | Email address                  |
| `phone_`       | `string`          | Phone number                   |
| `accounts_`    | `vector<Account*>`| All linked accounts            |

### `Bank`
Central controller — creates and manages all customers and accounts.

| Method                                        | Description                         |
|-----------------------------------------------|-------------------------------------|
| `createCustomer(name, email, phone)`          | Registers a new customer            |
| `createAccount(customerId, type, deposit)`    | Opens a new account                 |
| `transfer(fromAcc, toAcc, amount)`            | Atomic fund transfer                |
| `findCustomer(id)` / `findAccount(number)`    | Lookup by ID / account number       |
| `displayAllCustomers()` / `displayAllAccounts()` | Bank-wide reports               |

---

## 🚀 Getting Started

### Prerequisites

- A C++17-compatible compiler:
  - **GCC** 7+ → `g++`
  - **Clang** 5+ → `clang++`
  - **MSVC** 2017+ (Visual Studio)

### Build & Run

```bash
# Clone the repository
git clone https://github.com/your-username/banking-system-cpp.git
cd banking-system-cpp

# Compile
g++ -std=c++17 -Wall -o banking_system banking_system.cpp

# Run
./banking_system
```

> **Windows (MinGW):**
> ```bash
> g++ -std=c++17 -Wall -o banking_system.exe banking_system.cpp
> banking_system.exe
> ```

---

## 💻 Usage / Demo Output

The `main()` function walks through a complete demonstration:

```
Welcome to Nexus National Bank

============================================================
  1. Creating Customers
============================================================
  Customer created successfully! ID: CUST1000   ← Alice Johnson
  Customer created successfully! ID: CUST1001   ← Bob Smith
  Customer created successfully! ID: CUST1002   ← Carol White

============================================================
  2. Creating Accounts
============================================================
  Account 100000 (Savings)  created for Alice Johnson
  Account 100001 (Checking) created for Alice Johnson
  Account 100002 (Checking) created for Bob Smith
  Account 100003 (Savings)  created for Carol White

============================================================
  4. Withdrawals
============================================================
  [-] Withdrawn $100.00 | New Balance: $900.00
  [Error] Insufficient funds.          ← overdraft caught

============================================================
  5. Fund Transfers
============================================================
  [<->] Transferred $300.00 from 100000 to 100002
  [Error] Insufficient funds for transfer.

============================================================
  7. Recent Transactions (last 5)
============================================================
  --- Last 3 transaction(s) for account 100000 ---
Type            Amount($)   Balance($)  Description
------------------------------------------------------------
Deposit         1000.00     2000.00     Initial deposit
Deposit          500.00     2500.00     Salary credit
Transfer Out     300.00     2200.00     Transfer to 100002
```

---

## 📁 Project Structure

```
banking-system-cpp/
│
├── banking_system.cpp   # All source code (single-file project)
└── README.md            # This file
```

---

## ⚠️ Error Handling

All invalid operations throw standard C++ exceptions and are caught gracefully:

| Scenario                      | Exception Type       |
|-------------------------------|----------------------|
| Negative initial balance      | `invalid_argument`   |
| Deposit / withdrawal ≤ 0      | `invalid_argument`   |
| Withdrawal exceeds balance    | `runtime_error`      |
| Transfer exceeds balance      | `runtime_error`      |
| Account / customer not found  | `runtime_error`      |
| Operation on inactive account | `runtime_error`      |

---

## 🧠 Concepts Demonstrated

| Concept                  | Where Used                                      |
|--------------------------|-------------------------------------------------|
| OOP — Classes & Objects  | `Transaction`, `Account`, `Customer`, `Bank`    |
| Encapsulation            | Private attributes with public getters          |
| `enum class`             | Scoped enums for `Transaction::Type`, `Account::Type` |
| Exception Handling       | `try/catch`, `throw` for all invalid operations |
| RAII / Memory Management | `Bank` destructor cleans all heap allocations   |
| `std::map`               | O(log n) lookup for customers and accounts      |
| `std::vector`            | Dynamic list of accounts and transactions       |
| `<iomanip>`              | Formatted monetary output (`setw`, `setprecision`) |
| `<ctime>`                | Auto-timestamp on every transaction             |

---

## 🔮 Future Improvements

- [ ] Persistent storage (file I/O or SQLite)
- [ ] Interactive CLI menu (loop-based user input)
- [ ] Interest calculation for savings accounts
- [ ] Account deactivation / deletion
- [ ] Multiple currencies with conversion rates
- [ ] Password-protected customer login
- [ ] Unit tests (Google Test / Catch2)

---

## 📄 License

This project is released under the [MIT License](LICENSE).

---

> Built with ❤️ in C++17 — demonstrating clean OOP design and financial logic.
