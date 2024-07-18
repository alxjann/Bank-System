#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using namespace std;
string fileName = "db.txt";

class User {
private:
    class UserAuth {
    public:
        string userName;
        string password;

        //login
        bool checkAccount(string userName, string password) {
            ifstream file(fileName);
            if (!file.is_open()) {
                cerr << "Unable to open account database!" << endl;
                system("pause");
                exit(-1);
            }

            string line;
            while (getline(file, line)) {
                size_t pos = line.find(":");
                size_t pos2 = line.find(":", pos + 1);
                if (pos != string::npos) {
                    string fileUser = line.substr(0, pos);
                    string filePassword = line.substr(pos + 1, pos2 - pos - 1);
                    if (fileUser == userName && filePassword == password) {
                        return true;
                    }
                }
            }
            return false;
        }

        //checks if the username already exists in the txt file
        bool isUserCreated(string userName) {
            ifstream file(fileName);
            if (!file.is_open()) {
                cerr << "Unable to open account database!" << endl;
                system("pause");
                exit(-1);
            }

            string line;
            while (getline(file, line)) {
                size_t pos = line.find(":");
                if (pos != string::npos) {
                    string fileUser = line.substr(0, pos);
                    if (fileUser == userName) {
                        return true;
                    }
                }
            }
            return false;
        }

        //register or add account to txt file
        void addAccount(string userName, string password) {
            ofstream file(fileName, ios_base::app);
            if (!file.is_open()) {
                cerr << "Unable to add account to database!" << endl;
                return;
            }

            file << userName << ":" << password << ":0.00" << "\n";
            file.close();

            if (checkAccount(userName, password))
                cout << "Account registered!\n";
            else
                cout << "Failed to register account. Please try again!\n";
            return;
        }
    };
public:
    UserAuth auth;

    float getBalance() {
        ifstream file(fileName);
        if (!file.is_open()) {
            cerr << "Unable to open account database!" << endl;
            system("pause");
            exit(-1);
        }
        
        string line;
        while (getline(file, line)) {
            size_t pos = line.find(":");
            size_t pos2 = line.find(":", pos + 1);
            if (pos != string::npos) {
                string fileUser = line.substr(0, pos);
                float balance = stof(line.substr(pos2 + 1));
                if (fileUser == auth.userName) {
                    return balance;
                }
            }
        }
    }

    void addBalance() {
        float addAmount;
        float currentBalance = getBalance();

        cout << "\n------- DEPOSIT -------\n";
        cout << "Your current balance is " << currentBalance << "\n";
        cout << "Amount to add in your account: ";
        
        while (!(cin >> addAmount)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }

        ifstream file1(fileName);
        if (!file1.is_open()) {
            cerr << "Unable to open account database!" << endl;
            system("pause");
            exit(-1);
        }

        string line;
        vector<string> lines;
        while (getline(file1, line)) {
            size_t pos = line.find(":");
            size_t pos2 = line.find(":", pos + 1);
            if (pos != string::npos) {
                string user = line.substr(0, pos);
                string password = line.substr(pos + 1, pos2 - pos - 1);
                if (user == auth.userName) {
                    line = user + ":" + password + ":" + to_string(currentBalance + addAmount);
                }
                lines.push_back(line);
            }
        }
        
        ofstream file2(fileName);
        if (!file2.is_open()) {
            cerr << "Unable to open account database!" << endl;
            system("pause");
            exit(-1);
        }

        for (const auto& l : lines) {
            file2 << l << endl;
        }
        file2.close();

        cout << "\nSuccessfully added " << addAmount << "\n";
        cout << "Your new balance is " << addAmount + currentBalance << "\n";
    }

    void withdrawBalance() {
        float withdrawAmount;
        float currentBalance = getBalance();

        cout << "\n------- WITHDRAW -------\n";
        cout << "Your current balance is " << currentBalance << "\n";
        cout << "Amount to withdraw: ";

        while (!(cin >> withdrawAmount)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }

        if (currentBalance < withdrawAmount) {
            cout << "You have insufficient amount of money in your account...\n";
        }
        else {
            ifstream file1(fileName);
            if (!file1.is_open()) {
                cerr << "Unable to open account database!" << endl;
                system("pause");
                exit(-1);
            }

            string line;
            vector<string> lines;
            while (getline(file1, line)) {
                size_t pos = line.find(":");
                size_t pos2 = line.find(":", pos + 1);
                if (pos != string::npos) {
                    string user = line.substr(0, pos);
                    string password = line.substr(pos + 1, pos2 - pos - 1);
                    if (user == auth.userName) {
                        line = user + ":" + password + ":" + to_string(currentBalance - withdrawAmount);
                    }
                    lines.push_back(line);
                }
            }

            ofstream file2(fileName);
            if (!file2.is_open()) {
                cerr << "Unable to open account database!" << endl;
                system("pause");
                exit(-1);
            }

            for (const auto& l : lines) {
                file2 << l << endl;
            }
            file2.close();

            cout << "\nYou have received " << withdrawAmount << "\n";
            cout << "Your new balance is: " << currentBalance - withdrawAmount << "\n";
        }
    }
};

void loginAccount(User& user) {
    string hasAccount;
    
    do {
        cout << "Do you have an account? (y/n): ";
        cin >> hasAccount;
    } while (hasAccount != "y" && hasAccount != "Y" && hasAccount != "n" && hasAccount != "N");

    if (hasAccount == "y" || hasAccount == "Y") {
        cout << "\nPlease login to your account.\n";
        
        do {
            cout << "\tusername: ";
            cin >> user.auth.userName;

            cout << "\tpassword: ";
            cin >> user.auth.password;

            if (!user.auth.checkAccount(user.auth.userName, user.auth.password))
                cerr << "Incorrect username or password. Please try again!\n";
        } while (!user.auth.checkAccount(user.auth.userName, user.auth.password));

    } 
    else if (hasAccount == "n" || hasAccount == "N") {
        cout << "\nPlease register an account.\n";

        do {
            cout << "\tusername: ";
            cin >> user.auth.userName;
            
            if (user.auth.isUserCreated(user.auth.userName))
                cerr << "Username already exist. Please try a new one!\n";
        } while (user.auth.isUserCreated(user.auth.userName));

        cout << "\tpassword: ";
        cin >> user.auth.password;

        user.auth.addAccount(user.auth.userName, user.auth.password);
    }
}

void menu(User& user) {
    bool exitMenu = false;

    do {
        int selection;
        cout << "\n-------Main Menu-------\n";
        cout << "Please make a selection:\n";
        cout << "  1. Check Balance\n  2. Withdraw\n  3. Deposit\n  4. Exit\n";
        cout << "\nPlease enter selection number: ";
        
        while (!(cin >> selection)) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number: ";
        }

        switch (selection) {
        case 1:
            cout << "\n------- BALANCE -------\n";
            cout << "--> Your Current Balance is: " << user.getBalance() << "\n";
            break;
        case 2:
            user.withdrawBalance();
            break;
        case 3:
            user.addBalance();
            break;
        case 4:
            exitMenu = true;
            break;
        default:
            cout << "Invalid selection. Please choose a number between 1 - 4.\n";
            break;
        }
    } while (!exitMenu);
}

int main()
{
    cout << "Welcom to C++ Bank!" << endl;
    User user;
    loginAccount(user);
    menu(user);
    system("pause");
}