// upto 17 jan 2026
#include <bits/stdc++.h>
#include <conio.h>
using namespace std;

struct account
{
    long long int accountnum;
    int pin;
    string name;
    long long int balance;
};
struct transaction
{
    long long int accountnum;
    string action;
    int amount;
    int amountleft;
    string date;
    string time;
};

string gettime()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int hour = ltm->tm_hour;
    int min = ltm->tm_min;

    return to_string(hour) + ":" +
           to_string(min);
}

string getdate()
{
    time_t now = time(0);
    tm *ltm = localtime(&now);

    int day = ltm->tm_mday;
    int month = 1 + ltm->tm_mon;
    int year = 1900 + ltm->tm_year;

    return to_string(day) + "-" +
           to_string(month) + "-" +
           to_string(year);
}

int takepin()
{
    int pin = 0;
    char ch;
    while (true)
    {
        ch = getch();
        if (ch == 13)
            break;
        if (ch >= '0' && ch <= '9')
        {
            cout << "*";
            pin = pin * 10 + (ch - '0');
        }
    }
    cout << endl;
    return pin;
}

int encrypt(int pin)
{
    int encryptedpin = pin * (pin + 1);
    return encryptedpin;
}

int disencrypt(int pin)
{
    int low = 1000, high = 9999, mid;
    while (low <= high)
    {
        mid = low + (high - low) / 2;
        if (mid * (mid + 1) == pin)
            return mid;
        else if (mid * (mid + 1) > pin)
            high = mid - 1;
        else
            low = mid + 1;
    }
    return 0;
}

vector<account> loadusers()
{
    vector<account> users;
    ifstream fin("accounts.txt");
    account temp;
    if (!fin)
    {
        cout << "File missing";
    }

    while (fin >> temp.accountnum >> temp.pin >> temp.name >> temp.balance)
    {
        users.push_back(temp);
    }
    fin.close();
    return users;
}

vector<transaction> loadtransaction()
{
    vector<transaction> transactions;
    ifstream fin("transactiondetails.txt");
    if (!fin)
    {
        cout << "File Missing" << endl;
    }
    transaction temp;
    while (fin >> temp.accountnum >> temp.action >> temp.amount >> temp.amountleft >> temp.date >> temp.time)
    {
        transactions.push_back(temp);
    }
    fin.close();
    return transactions;
}

int verifyuser(vector<account> &users, long long int acc, int pin)
{
    int index = -1;
    for (int i = 0; i < users.size(); i++)
    {
        if (acc == users[i].accountnum && pin == users[i].pin)
        {
            index = i;
            break;
        }
    }
    return index;
}

void savetransaction(account user, string type, int amount)
{

    ofstream fout("transactiondetails.txt", ios::app);
    fout << user.accountnum << " "
         << type << " "
         << amount << " "
         << user.balance << " "
         << getdate() << " "
         << gettime() << endl;

    fout.close();
}

void withdrawmoney(account &user)
{
    int amt;
    cout << "Enter Amount You want to Withdraw" << endl;
    cin >> amt;
    if (amt <= 0)
    {
        cout << "Please Enter a Valid Amount" << endl;
    }
    else if (amt > user.balance)
    {
        cout << "Insufficient balance" << endl;
    }
    else
    {
        user.balance -= amt;
        cout << "Transaction Succesful" << endl;
        cout << "Remaining Balance: " << user.balance << endl;
        string ch = "DEBIT";
        savetransaction(user, ch, amt);
    }
}

void showtransactionhistory(long long int accountnum)
{
    int num, idx = 0;
    cout << "Enter number of previous transactions you want to view" << endl;
    cin >> num;
    vector<transaction> transactions = loadtransaction();
    bool transactionfound = false;
    for (int i = transactions.size() - 1; i >= 0; i--)
    {
        if (accountnum == transactions[i].accountnum)
        {
            transactionfound = true;
            num--;
            if (num == 0)
            {
                idx = i;
                break;
            }
        }
    }
    if (!transactionfound)
    {
        cout << "NO Transactions Found" << endl;
    }

    else
    {

        cout << "===================================================" << endl;
        cout << "                TRANSACTION HISTORY                 " << endl;
        cout << "===================================================" << endl;
        cout << left
             << setw(10) << "Type"
             << setw(12) << "Amount"
             << setw(12) << "Balance"
             << setw(12) << "Date"
             << setw(10) << "Time" << endl;

        for (int i = idx; i < transactions.size(); i++)
        {
            if (accountnum == transactions[i].accountnum)
            {
                cout << left
                     << setw(10) << transactions[i].action
                     << setw(12) << transactions[i].amount
                     << setw(12) << transactions[i].amountleft
                     << setw(12) << transactions[i].date
                     << setw(10) << transactions[i].time << endl;
            }
        }
    }
}

void saveusers(vector<account> &users)
{
    ofstream fout("accounts.txt");
    for (int i = 0; i < users.size(); i++)
    {
        fout << users[i].accountnum << " " << users[i].pin << " " << users[i].name << " " << users[i].balance << endl;
    }
    fout.close();
}

void changepin(account &user)
{
    cout << "Enter Your Old PIN" << endl;
    int enteredpin;
    int enteredencryptedpin;
    enteredpin = takepin();
    enteredencryptedpin = encrypt(enteredpin);
    if (enteredencryptedpin != user.pin)
    {
        cout << "Incorrect PIN" << endl;
    }
    else
    {
        cout << "Enter New PIN" << endl;
        int newpin;
        newpin = takepin();
        if (newpin < 1000 || newpin > 9999)
        {
            cout << "PIN should contain only 4 digits" << endl;
        }
        else if (encrypt(newpin) == user.pin)
        {
            cout << "New PIN is same as Old PIN" << endl;
        }
        else
        {
            int newpin2;
            cout << "Enter New PIN again to CONFIRM" << endl;
            newpin2 = takepin();
            if (newpin != newpin2)
            {
                cout << "Both PINs are diffrent" << endl;
            }
            else
            {
                user.pin = encrypt(newpin);
                cout << "PIN changed Successfully" << endl;
            }
        }
    }
}

void checkbalance(account &user)
{
    cout << "Your available balace is: " << user.balance << endl;
}

void depositemoney(account &user)
{
    int amt;
    cout << "Enter Amount You want to Deposite" << endl;
    cin >> amt;
    if (amt <= 0)
    {
        cout << "Please Enter a Valid Amount" << endl;
    }

    else
    {
        user.balance += amt;
        cout << "Deposition Succesful" << endl;
        cout << "New Balance: " << user.balance << endl;
        string ch = "CREDIT";
        savetransaction(user, ch, amt);
    }
}

void savenewaccount(int acc, int pin, string name)
{
    ofstream fout("accounts.txt", ios::app);
    fout << acc << " " << encrypt(pin) << " " << name << " " << 0 << endl;
}

void createnewaccount()
{
    string ch;
    int pin1, pin2;
    vector<account> users = loadusers();
    int acc = users[users.size() - 1].accountnum + 1;
    bool accfound = false;
    if (users.size() == 0)
    {
        acc = 1000;
    }
    else
    {
        while (!accfound)
        {
            for (int i = 0; i < users.size(); i++)
            {
                if (acc == users[i].accountnum)
                {
                    if (acc == 9999)
                        acc = 1000;
                    else
                        acc++;
                    break;
                }
                accfound = true;
            }
        }
    }
    cout << "Enter Your Name" << endl;
    cin >> ch;
    bool pinapproved = false;
    while (!pinapproved)
    {
        pinapproved = true;

        cout << "Enter PIN for your new account" << endl;
        pin1 = takepin();

        cout << "Enter New PIN again to CONFIRM" << endl;
        pin2 = takepin();
        if (pin1 != pin2)
        {
            pinapproved = false;
            cout << "Both PINs are diffrent" << endl;
        }
    }

    string ok;
    cout << "Your Account number will be: " << acc << endl;
    cout << "press any key for OK" << endl;
    cin >> ok;
    savenewaccount(acc, pin1, ch);
}

void changename(account &user)
{
    string ch;
    cout << "Enter Your New NAME" << endl;
    cin >> ch;
    user.name = ch;
    cout << "Your NAME changed successfully" << endl;
}
int main()
{
    int index = -1;
    vector<account> users = loadusers();

    bool first = false;
    while (true)
    {

        cout << "1. Login" << endl;
        cout << "2. Create NEW Account" << endl;
        int c1;
        cin >> c1;
        if (c1 == 1)
        {

            

            while (true)
            {

                bool login = true;
                long long int enteredaccountnum;
                int enteredpin;
                cout << "Enter Your Account Number" << endl;
                cin >> enteredaccountnum;
                if (cin.fail())
                {
                    cin.clear();
                    cin.ignore(1000, '\n');
                    cout << "Invalid input! Enter numbers only" << endl;
                    continue;
                }

                cout << "Enter Your ATM PIN" << endl;
                enteredpin = takepin();
                int enteredencryptedpin = encrypt(enteredpin);

                index = verifyuser(users, enteredaccountnum, enteredencryptedpin);

                if (index == -1)
                {
                    cout << "Sorry! Entered Account Number or PIN is Invalid" << endl;
                    login = false;
                }
                if (login)
                {

                    first = true;
                    break;
                }
            }
            if (first)
                break;
        }

        else if (c1 == 2)
        {
            createnewaccount();
        }
        else
            cout << "Enter Valid Choice" << endl;
    }

    cout << "Welcome! " << users[index].name << endl;
    while (true)
    {
        cout << "1. Withdraw               2. Deposite" << endl;
        cout << "3. Change PIN             4. Check balance" << endl;
        cout << "5. Transaction History    6. Change Name" << endl;
        cout << "7.EXIT" << endl;
        int choice;
        cin >> choice;
        if (cin.fail())
        {
            cin.clear();
            cin.ignore(1000, '\n');
            cout << "Invalid input! Enter numbers only" << endl;
            continue;
        }
        bool change = false;
        if (choice == 1)
        {
            withdrawmoney(users[index]);
            change = true;
        }

        else if (choice == 2)
        {
            depositemoney(users[index]);
            change = true;
        }

        else if (choice == 3)
        {
            changepin(users[index]);
            change = true;
        }

        else if (choice == 4)
        {
            checkbalance(users[index]);
        }

        else if (choice == 5)
        {
            showtransactionhistory(users[index].accountnum);
        }

        else if (choice == 6)
        {
            changename(users[index]);
            change = true;
        }

        else if (choice == 7)
            break;

        else
            cout << "Enter Valid Choice" << endl;

        if (change)
        {
            saveusers(users);
        }
    }
}

