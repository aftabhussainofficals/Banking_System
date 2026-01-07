#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <random>
#include <limits>
#include <algorithm>
#include <windows.h>
using namespace std;
struct Transaction {
    string timestamp;
    string accountNumber;
    string type;
    double amount;
    double balance;
    string toJson() const;
    static Transaction fromJson(const string& jsonStr);
};
class User {
private:
    string accountNumber;
    string username;
    string password;
    string name;
    string accountType;  
    string cardNumber;
    string cardPin;
    double balance;
    bool hasCard; 
    
public:
    User();
    User(string username, string password, string name, string accountType);
    string getAccountNumber() const;
    string getUsername() const;
    string getName() const;
    string getAccountType() const;
    string getCardNumber() const;
    double getBalance() const;
    bool getHasCard() const;
    bool checkPassword(string password) const;
    bool checkCardPin(string pin) const;
    void deposit(double amount);
    bool withdraw(double amount);
    void requestATMCard();
    void changeCardPin(string newPin);
    string toJson() const;
    static User fromJson(const string& jsonStr);
private:
    string generateAccountNumber();
    string generateCardNumber();
    string generateCardPin();
};
class FileHandler {
public:
    static void saveUser(const User& user);
    static void saveAllUsers(const vector<User>& users);
    static vector<User> loadAllUsers();
    static void saveTransaction(const User& user, const string& type, double amount);
    static vector<string> loadTransactions(const string& accountNumber);
private:
    static const string USERS_FILE;
    static const string TRANSACTIONS_FILE;
    static string getCurrentTimestamp();
    static vector<Transaction> loadAllTransactions();
    static void saveAllTransactions(const vector<Transaction>& transactions);
};
class BankingSystem {
private:
    vector<User> users;
    User* currentUser;
public:
    BankingSystem();
    ~BankingSystem();
    void run();
    void showMainMenu();
    void registerUser();
    bool login();
    void atmLogin();  
    void userDashboard();
    void showUserMenu();
    void deposit();
    void withdraw();
    void transfer();
    void showBalance();
    void showAccountInfo();
    void showTransactionHistory();
    void manageATMCard();  
    void requestNewCard();
    void changeCardPin();
    void logout();
    void saveAllData();
    void loadAllData();
    void atmDashboard();  
    void atmWithdraw();   
    void atmDeposit();    
};
const string FileHandler::USERS_FILE = "data/users.json";
const string FileHandler::TRANSACTIONS_FILE = "data/transaction.json";
void setColor(int color);
User::User() : balance(0.0), hasCard(false) {
    accountNumber = generateAccountNumber();
}
User::User(string username, string password, string name, string accountType) 
    : username(username), password(password), name(name), accountType(accountType), 
      balance(0.0), hasCard(false) {
    accountNumber = generateAccountNumber();
}
string User::getAccountNumber() const {
    return accountNumber;
}
string User::getUsername() const {
    return username;
}
string User::getName() const {
    return name;
}
string User::getAccountType() const {
    return accountType;
}
string User::getCardNumber() const {
    return cardNumber;
}
double User::getBalance() const {
    return balance;
}
bool User::getHasCard() const {
    return hasCard;
}
bool User::checkPassword(string password) const {
    return this->password == password;
}
bool User::checkCardPin(string pin) const {
    return hasCard && this->cardPin == pin;
}
void User::deposit(double amount) {
    if (amount > 0) {
        balance += amount;
    }
}
bool User::withdraw(double amount) {
    if (amount > 0 && balance >= amount) {
        balance -= amount;
        return true;
    }
    return false;
}
void User::requestATMCard() {
    if (!hasCard) {
        cardNumber = generateCardNumber();
        cardPin = generateCardPin();
        hasCard = true;
        cout << "\nATM Card issued successfully!\n";
        cout << "Card Number: " << cardNumber << "\n";
        cout << "PIN: " << cardPin << " (Keep this safe!)\n";
    } else {
        cout << "\nYou already have an ATM card.\n";
    }
}
void User::changeCardPin(string newPin) {
    if (hasCard) {
        if (newPin.length() == 4) {
            cardPin = newPin;
            cout << "\nPIN changed successfully!\n";
        } else {
            cout << "\nPIN must be 4 digits!\n";
        }
    } else {
        cout << "\nNo ATM card found. Please request a card first.\n";
    }
}
string User::generateAccountNumber() {
    static int counter = 1000;
    stringstream ss;
    ss << "ACC" << setw(7) << setfill('0') << ++counter;
    return ss.str();
}
string User::generateCardNumber() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);
    stringstream ss;
    ss << "4"; 
    for (int i = 0; i < 15; i++) {
        ss << dis(gen);
        if ((i + 1) % 4 == 0 && i < 14) {
            ss << " ";
        }
    }
    return ss.str();
}
string User::generateCardPin() {
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, 9);
    stringstream ss;
    for (int i = 0; i < 4; i++) {
        ss << dis(gen);
    }
    return ss.str();
}
string User::toJson() const {
    stringstream ss;
    ss << "{";
    ss << "\"accountNumber\":\"" << accountNumber << "\",";
    ss << "\"username\":\"" << username << "\",";
    ss << "\"password\":\"" << password << "\",";
    ss << "\"name\":\"" << name << "\",";
    ss << "\"accountType\":\"" << accountType << "\",";
    ss << "\"cardNumber\":\"" << cardNumber << "\",";
    ss << "\"cardPin\":\"" << cardPin << "\",";
    ss << "\"hasCard\":" << (hasCard ? "true" : "false") << ",";
    ss << "\"balance\":" << balance;
    ss << "}";
    return ss.str();
}
User User::fromJson(const string& jsonStr) {
    User user;
    size_t pos;
    pos = jsonStr.find("\"accountNumber\":\"");
    if (pos != string::npos) {
        pos += 17;
        size_t end = jsonStr.find("\"", pos);
        user.accountNumber = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"username\":\"");
    if (pos != string::npos) {
        pos += 12;
        size_t end = jsonStr.find("\"", pos);
        user.username = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"password\":\"");
    if (pos != string::npos) {
        pos += 12;
        size_t end = jsonStr.find("\"", pos);
        user.password = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"name\":\"");
    if (pos != string::npos) {
        pos += 8;
        size_t end = jsonStr.find("\"", pos);
        user.name = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"accountType\":\"");
    if (pos != string::npos) {
        pos += 15;
        size_t end = jsonStr.find("\"", pos);
        user.accountType = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"cardNumber\":\"");
    if (pos != string::npos) {
        pos += 14;
        size_t end = jsonStr.find("\"", pos);
        user.cardNumber = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"cardPin\":\"");
    if (pos != string::npos) {
        pos += 11;
        size_t end = jsonStr.find("\"", pos);
        user.cardPin = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"hasCard\":");
    if (pos != string::npos) {
        pos += 10;
        size_t end = jsonStr.find(",", pos);
        string hasCardStr = jsonStr.substr(pos, end - pos);
        user.hasCard = (hasCardStr == "true");
    }
    pos = jsonStr.find("\"balance\":");
    if (pos != string::npos) {
        pos += 10;
        size_t end = jsonStr.find("}", pos);
        string balanceStr = jsonStr.substr(pos, end - pos);
        user.balance = stod(balanceStr);
    }
    return user;
}
string Transaction::toJson() const {
    stringstream ss;
    ss << "{";
    ss << "\"timestamp\":\"" << timestamp << "\",";
    ss << "\"accountNumber\":\"" << accountNumber << "\",";
    ss << "\"type\":\"" << type << "\",";
    ss << "\"amount\":" << amount << ",";
    ss << "\"balance\":" << balance;
    ss << "}";
    return ss.str();
}
Transaction Transaction::fromJson(const string& jsonStr) {
    Transaction trans;
    size_t pos;
    pos = jsonStr.find("\"timestamp\":\"");
    if (pos != string::npos) {
        pos += 14;
        size_t end = jsonStr.find("\"", pos);
        trans.timestamp = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"accountNumber\":\"");
    if (pos != string::npos) {
        pos += 17;
        size_t end = jsonStr.find("\"", pos);
        trans.accountNumber = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"type\":\"");
    if (pos != string::npos) {
        pos += 8;
        size_t end = jsonStr.find("\"", pos);
        trans.type = jsonStr.substr(pos, end - pos);
    }
    pos = jsonStr.find("\"amount\":");
    if (pos != string::npos) {
        pos += 9;
        size_t end = jsonStr.find(",", pos);
        string amountStr = jsonStr.substr(pos, end - pos);
        trans.amount = stod(amountStr);
    }
    pos = jsonStr.find("\"balance\":");
    if (pos != string::npos) {
        pos += 10;
        size_t end = jsonStr.find("}", pos);
        string balanceStr = jsonStr.substr(pos, end - pos);
        trans.balance = stod(balanceStr);
    }
    return trans;
}
string FileHandler::getCurrentTimestamp() {
    time_t now = time(nullptr);
    tm* local = localtime(&now);
    stringstream ss;
    ss << put_time(local, "%Y-%m-%d %H:%M:%S");
    return ss.str();
}
vector<Transaction> FileHandler::loadAllTransactions() {
    vector<Transaction> transactions;
    ifstream file(TRANSACTIONS_FILE);
    if (!file.is_open()) {
        return transactions;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "[" || line == "]") {
            continue;
        }
        if (line.back() == ',') {
            line.pop_back();
        }
        transactions.push_back(Transaction::fromJson(line));
    }
    file.close();
    return transactions;
}
void FileHandler::saveAllTransactions(const vector<Transaction>& transactions) {
    ofstream file(TRANSACTIONS_FILE);
    if (!file.is_open()) {
        cerr << "Error: Could not open transactions file.\n";
        return;
    }
    file << "[\n";
    for (size_t i = 0; i < transactions.size(); ++i) {
        file << transactions[i].toJson();
        if (i != transactions.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    file << "]\n";
    file.close();
}
void FileHandler::saveUser(const User& user) {
    auto users = loadAllUsers();
    bool found = false;
    for (auto& u : users) {
        if (u.getAccountNumber() == user.getAccountNumber()) {
            u = user;
            found = true;
            break;
        }
    }
    if (!found) {
        users.push_back(user);
    }
    saveAllUsers(users);
}
void FileHandler::saveAllUsers(const vector<User>& users) {
    ofstream file(USERS_FILE);
    if (!file.is_open()) {
        cerr << "Error: Could not open users file.\n";
        return;
    }
    file << "[\n";
    for (size_t i = 0; i < users.size(); ++i) {
        file << users[i].toJson();
        if (i != users.size() - 1) {
            file << ",";
        }
        file << "\n";
    }
    file << "]\n";
    file.close();
}
vector<User> FileHandler::loadAllUsers() {
    vector<User> users;
    ifstream file(USERS_FILE);
    if (!file.is_open()) {
        return users;
    }
    string line;
    while (getline(file, line)) {
        if (line.empty() || line == "[" || line == "]") {
            continue;
        }
        if (line.back() == ',') {
            line.pop_back();
        }
        users.push_back(User::fromJson(line));
    }
    file.close();
    return users;
}
void FileHandler::saveTransaction(const User& user, const string& type, double amount) {
    auto transactions = loadAllTransactions();
    string timestamp = getCurrentTimestamp();
    Transaction newTrans = {timestamp, user.getAccountNumber(), type, amount, user.getBalance()};
    transactions.push_back(newTrans);
    saveAllTransactions(transactions);
}
vector<string> FileHandler::loadTransactions(const string& accountNumber) {
    vector<string> transactions;
    auto allTransactions = loadAllTransactions();
    for (const auto& trans : allTransactions) {
        if (trans.accountNumber == accountNumber) {
            stringstream formatted;
            formatted << left << setw(19) << trans.timestamp << "| "
                     << setw(18) << trans.type << "| $"
                     << setw(9) << trans.amount << "| $"
                     << trans.balance;
            transactions.push_back(formatted.str());
        }
    }
    return transactions;
}
BankingSystem::BankingSystem() : currentUser(nullptr) {
    loadAllData();
}
BankingSystem::~BankingSystem() {
    saveAllData();
}
void BankingSystem::run() {
    int choice;
    do {
        showMainMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch(choice) {
            case 1:
                registerUser();
                break;
            case 2:
                if (login()) {
                    userDashboard();
                }
                break;
            case 3:
                atmLogin();  
                break;
            case 4:
                cout << "\nThank you for using Banking System. Goodbye!\n";
                break;
            default:
                cout << "Invalid choice! Please try again.\n";
        }
    } while(choice != 4);
}
void BankingSystem::showMainMenu() {
    cout << "\n=== BANKING SYSTEM ===\n";
    cout << "1. Register New Account\n";
    cout << "2. Login (Username/Password)\n";
    cout << "3. ATM Card Login\n";
    cout << "4. Exit\n";
}
void BankingSystem::registerUser() {
    string username, password, name, accountType;
    int typeChoice;
    cout << "\n=== REGISTER NEW ACCOUNT ===\n";
    cout << "Enter username: ";
    getline(cin, username);
    for (const auto& user : users) {
        if (user.getUsername() == username) {
            cout << "Username already exists! Please choose another.\n";
            return;
        }
    }
    cout << "Enter password: ";
    getline(cin, password);
    cout << "Enter your full name: ";
    getline(cin, name);
    cout << "\nSelect Account Type:\n";
    cout << "1. Savings Account\n";
    cout << "2. Current Account\n";
    cout << "3. Fixed Deposit Account\n";
    cout << "Enter choice (1-3): ";
    cin >> typeChoice;
    cin.ignore();
    switch(typeChoice) {
        case 1:
            accountType = "Savings";
            break;
        case 2:
            accountType = "Current";
            break;
        case 3:
            accountType = "Fixed";
            break;
        default:
            accountType = "Savings";
            cout << "Invalid choice. Setting to Savings Account.\n";
    }
    User newUser(username, password, name, accountType);
    users.push_back(newUser);
    FileHandler::saveUser(newUser);
    cout << "\n Account created successfully!\n";
    cout << "Account Number: " << newUser.getAccountNumber() << "\n";
    cout << "Account Type: " << accountType << "\n";
}
bool BankingSystem::login() {
    string username, password;
    cout << "\n=== LOGIN ===\n";
    cout << "Enter username: ";
    getline(cin, username);
    cout << "Enter password: ";
    getline(cin, password);
    for (auto& user : users) {
        if (user.getUsername() == username && user.checkPassword(password)) {
            currentUser = &user;
            cout << "\n Login successful! Welcome " << user.getName() << "!\n";
            return true;
        }
    }
    cout << " Invalid username or password!\n";
    return false;
}
void BankingSystem::atmLogin() {
    string cardNumber, pin;
    cout << "\n=== ATM CARD LOGIN ===\n";
    cout << "Enter card number (format: XXXX XXXX XXXX XXXX): ";
    getline(cin, cardNumber);
    cout << "Enter 4-digit PIN: ";
    getline(cin, pin);
    for (auto& user : users) {
        if (user.getHasCard() && user.getCardNumber() == cardNumber && user.checkCardPin(pin)) {
            currentUser = &user;
            cout << "\n ATM Login successful! Welcome " << user.getName() << "!\n";
            atmDashboard(); 
            return;
        }
    }
    cout << " Invalid card number or PIN!\n";
}
void BankingSystem::atmDashboard() {
    int choice;
    do {
        cout << "\n=== ATM SERVICES ===\n";
        cout << "1. Check Balance\n";
        cout << "2. Withdraw Cash\n";
        cout << "3. Deposit Cash\n";
        cout << "4. Change PIN\n";
        cout << "5. Exit ATM\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch(choice) {
            case 1:
                showBalance();
                break;
            case 2:
                atmWithdraw();
                break;
            case 3:
                atmDeposit();
                break;
            case 4:
                changeCardPin();
                break;
            case 5:
                cout << "\nThank you for using ATM. Please take your card.\n";
                logout();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 5 && currentUser != nullptr);
}
void BankingSystem::atmWithdraw() {
    if (!currentUser) return;
    double amount;
    cout << "\n=== ATM WITHDRAWAL ===\n";
    cout << "Enter amount to withdraw: $";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    if (amount > 1000) {
        cout << "ATM withdrawal limit is $1000 per transaction.\n";
        return;
    }
    if (currentUser->withdraw(amount)) {
        cout << "\n Please take your cash: $" << amount << "\n";
        cout << "Available balance: $" << currentUser->getBalance() << "\n";
        FileHandler::saveTransaction(*currentUser, "ATM_WITHDRAWAL", amount);
        FileHandler::saveUser(*currentUser);
    } else {
        cout << "\n Insufficient balance or invalid amount!\n";
    }
}
void BankingSystem::atmDeposit() {
    if (!currentUser) return;
    double amount;
    cout << "\n=== ATM DEPOSIT ===\n";
    cout << "Enter amount to deposit: $";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    if (amount > 5000) {
        cout << "ATM deposit limit is $5000 per transaction.\n";
        return;
    }
    currentUser->deposit(amount);
    cout << "\n Successfully deposited: $" << amount << "\n";
    cout << "Available balance: $" << currentUser->getBalance() << "\n";
    FileHandler::saveTransaction(*currentUser, "ATM_DEPOSIT", amount);
    FileHandler::saveUser(*currentUser);
}
void BankingSystem::userDashboard() {
    int choice;
    do {
        showUserMenu();
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        switch(choice) {
            case 1:
                deposit();
                break;
            case 2:
                withdraw();
                break;
            case 3:
                showBalance();
                break;
            case 4:
                showAccountInfo();
                break;
            case 5:
                transfer();
                break;
            case 6:
                showTransactionHistory();
                break;
            case 7:
                manageATMCard();
                break;
            case 8:
                logout();
                break;
            default:
                cout << "Invalid choice!\n";
        }
    } while(choice != 8 && currentUser != nullptr);
}
void BankingSystem::showUserMenu() {
    cout << "\n=== USER DASHBOARD ===\n";
    cout << "1. Deposit Money\n";
    cout << "2. Withdraw Money\n";
    cout << "3. Check Balance\n";
    cout << "4. Account Information\n";
    cout << "5. Transfer Money\n";
    cout << "6. Transaction History\n";
    cout << "7. Manage ATM Card\n";
    cout << "8. Logout\n";
    cout << "9. Loan Services\n";
}
void BankingSystem::deposit() {
    if (!currentUser) return;
    double amount;
    cout << "\n=== DEPOSIT MONEY ===\n";
    cout << "Enter amount to deposit: $";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    currentUser->deposit(amount);
    cout << "\n Successfully deposited: $" << amount << "\n";
    cout << "Available balance: $" << currentUser->getBalance() << "\n";
    FileHandler::saveTransaction(*currentUser, "DEPOSIT", amount);
    FileHandler::saveUser(*currentUser);
}
void BankingSystem::withdraw() {
    if (!currentUser) return;
    double amount;
    cout << "\n=== WITHDRAW MONEY ===\n";
    cout << "Enter amount to withdraw: $";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    if (currentUser->withdraw(amount)) {
        cout << "\n Successfully withdrawn: $" << amount << "\n";
        cout << "Available balance: $" << currentUser->getBalance() << "\n";
        FileHandler::saveTransaction(*currentUser, "WITHDRAW", amount);
        FileHandler::saveUser(*currentUser);
    } else {
        cout << "\n Insufficient balance!\n";
    }
}
void BankingSystem::transfer() {
    if (!currentUser) return;
    string targetAccount;
    double amount;
    cout << "\n=== TRANSFER MONEY ===\n";
    cout << "Enter target account number: ";
    cin >> targetAccount;
    cout << "Enter amount to transfer: $";
    cin >> amount;
    if (amount <= 0) {
        cout << "Invalid amount!\n";
        return;
    }
    if (!currentUser->withdraw(amount)) {
        cout << "Insufficient balance!\n";
        return;
    }
    for (auto& user : users) {
        if (user.getAccountNumber() == targetAccount && &user != currentUser) {
            user.deposit(amount);
            FileHandler::saveUser(user);
            cout << "\n Successfully transferred $" << amount << " to account " << targetAccount << "\n";
            FileHandler::saveTransaction(*currentUser, "TRANSFER_OUT:" + targetAccount, amount);
            FileHandler::saveTransaction(user, "TRANSFER_IN:" + currentUser->getAccountNumber(), amount);
            FileHandler::saveUser(*currentUser);
            return;
        }
    }
    currentUser->deposit(amount);
    cout << "\n Target account not found!\n";
}
void BankingSystem::showBalance() {
    if (!currentUser) return;
    cout << "\n=== ACCOUNT BALANCE ===\n";
    cout << "Account: " << currentUser->getAccountNumber() << "\n";
    cout << "Balance: $" << fixed << setprecision(2) << currentUser->getBalance() << "\n";
}
void BankingSystem::showAccountInfo() {
    if (!currentUser) return;
    cout << "\n=== ACCOUNT INFORMATION ===\n";
    cout << "Account Number: " << currentUser->getAccountNumber() << "\n";
    cout << "Account Holder: " << currentUser->getName() << "\n";
    cout << "Account Type: " << currentUser->getAccountType() << "\n";
    cout << "Balance: $" << fixed << setprecision(2) << currentUser->getBalance() << "\n";
    if (currentUser->getHasCard()) {
        cout << "ATM Card: " << currentUser->getCardNumber() << "\n";
        cout << "Note: PIN is hidden for security\n";
    } else {
        cout << "ATM Card: Not issued\n";
    }
}
void BankingSystem::showTransactionHistory() {
    if (!currentUser) return;
    cout << "\n=== TRANSACTION HISTORY ===\n";
    auto transactions = FileHandler::loadTransactions(currentUser->getAccountNumber());
    if (transactions.empty()) {
        cout << "No transactions found.\n";
    } else {
        cout << "Date/Time           | Type               | Amount    | Balance\n";
        cout << "----------------------------------------------------------------\n";
        for (const auto& trans : transactions) {
            cout << trans << "\n";
        }
    }
}
void BankingSystem::manageATMCard() {
    if (!currentUser) return;
    int choice;
    do {
        cout << "\n=== ATM CARD MANAGEMENT ===\n";
        cout << "1. Request New ATM Card\n";
        cout << "2. Change PIN\n";
        cout << "3. View Card Details\n";
        cout << "4. Back to Main Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;
        cin.ignore();
        switch(choice) {
            case 1:
                requestNewCard();
                break;
            case 2:
                changeCardPin();
                break;
            case 3:
                if (currentUser->getHasCard()) {
                    cout << "\n=== CARD DETAILS ===\n";
                    cout << "Card Number: " << currentUser->getCardNumber() << "\n";
                    cout << "Linked Account: " << currentUser->getAccountNumber() << "\n";
                    cout << "Account Holder: " << currentUser->getName() << "\n";
                } else {
                    cout << "\nNo ATM card issued.\n";
                }
                break;
            case 4:
                return;
            default:
                cout << "Invalid choice!\n";
        }
    } while(true);
}
void BankingSystem::requestNewCard() {
    if (!currentUser) return;
    currentUser->requestATMCard();
    FileHandler::saveUser(*currentUser);
}
void BankingSystem::changeCardPin() {
    if (!currentUser) return;
    if (!currentUser->getHasCard()) {
        cout << "\nNo ATM card found. Please request a card first.\n";
        return;
    }
    string oldPin, newPin;
    cout << "\n=== CHANGE CARD PIN ===\n";
    cout << "Enter current 4-digit PIN: ";
    getline(cin, oldPin);
    if (!currentUser->checkCardPin(oldPin)) {
        cout << "\n Invalid PIN!\n";
        return;
    }
    cout << "Enter new 4-digit PIN: ";
    getline(cin, newPin);
    if (newPin.length() != 4) {
        cout << "\n PIN must be 4 digits!\n";
        return;
    }
    cout << "Confirm new 4-digit PIN: ";
    string confirmPin;
    getline(cin, confirmPin);
    if (newPin != confirmPin) {
        cout << "\n PINs don't match!\n";
        return;
    }
    currentUser->changeCardPin(newPin);
    FileHandler::saveUser(*currentUser);
}
void BankingSystem::logout() {
    currentUser = nullptr;
    cout << "\n Successfully logged out.\n";
}
void BankingSystem::saveAllData() {
    FileHandler::saveAllUsers(users);
}
void BankingSystem::loadAllData() {
    users = FileHandler::loadAllUsers();
}
void setColor(int color) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, color);
}
int main() {
    system("cls");  
    setColor(11); 
    cout << "========================================\n";
    cout << "      WELCOME TO BANKING SYSTEM\n";
    cout << "       WITH ATM CARD SERVICES\n";
    cout << "========================================\n";
    setColor(7); 
    BankingSystem bankingSystem;
    bankingSystem.run();
    setColor(10);  
    cout << "\nThank you for banking with us!\n";
    setColor(7);
    return 0;
}