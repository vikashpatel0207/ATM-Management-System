// upto 13 jan 2026
#include<bits/stdc++.h>
#include<conio.h>
using namespace std;

struct account{
    long long int accountnum;
    int pin;
    string name;
    long long int balance;

};

int takepin(){
    int pin=0;
    char ch;
    while(true){
        ch=getch();
        if(ch==13) break;
        if(ch>='0' && ch<='9'){
            cout<<"*";
        pin=pin*10+(ch-'0');
        }
    }
    cout<<endl;
    return pin;
}


vector<account> loadusers(){
    vector<account> users;
    ifstream fin("accounts.txt");
    account temp;
    if(!fin){
        cout<<"File missing";
    }

    while(fin >> temp.accountnum >> temp.pin >> temp.name >> temp.balance) {
        users.push_back(temp);
    }
    fin.close();
    return users;

}

int verifyuser(vector<account> &users, long long int acc, int pin){
    int index=-1;
    for(int i=0; i<users.size(); i++){
        if(acc==users[i].accountnum && pin==users[i].pin){
            index=i;
            break;
        }
    }
    return index;
     
}

void withdrawmoney(account &user){
    int amt;
    cout<<"Enter Amount You want to Withdraw"<<endl;
    cin>>amt;
    if(amt<=0){
        cout<<"Please Enter a Valid Amount"<<endl;
    }
    else if(amt>user.balance){
        cout<<"Insuuficient balance"<<endl;
        
    }
    else {
        user.balance -=amt;
        cout<<"Transaction Succesful"<<endl;
        cout<<"Remaining Balance: "<<user.balance<<endl;
    }

}

void saveusers(vector<account> &users){
    ofstream fout("accounts.txt");
    for(int i=0; i<users.size(); i++){
        fout<< users[i].accountnum<<" "<<
               users[i].pin<<" "<<
               users[i].name<<" "<<
               users[i].balance<<endl;
              
    }
    fout.close();

}

void changepin(account &user){
    cout<<"Enter Your Old PIN"<<endl;
    int enteredpin;
    enteredpin=takepin();
    if(enteredpin!=user.pin){
        cout<<"Incorrect PIN"<<endl;
    }
    else {
        cout<<"Enter New PIN"<<endl;
        int newpin;
        newpin=takepin();
        if(newpin<1000 || newpin>9999){
            cout<<"PIN should contain only 4 digits"<<endl;
        }
        else if(newpin==user.pin){
            cout<<"New PIN is same as Old PIN"<<endl;
        }
        else{
            int newpin2;
            cout<<"Enter New PIN again to CONFIRM"<<endl;
            newpin2=takepin();
            if(newpin!=newpin2){
                cout<<"Both PINs are diffrent"<<endl;
            }
            else{
                user.pin=newpin;
            cout<<"PIN changed Successfully"<<endl;
            }
        }

    }
}

void checkbalance(account &user){
    cout<<"Your available balace is: "<<user.balance<<endl;
}



int main(){
    
    vector<account> users= loadusers();
    
    long long int enteredaccountnum;
    int enteredpin;
    cout<<"Enter Your Account Number"<<endl;
    cin>>enteredaccountnum;
    cout<<"Enter Your ATM PIN"<<endl;
    enteredpin=takepin();
    
    

    int index=verifyuser(users, enteredaccountnum, enteredpin);
   
    if(index==-1){
        cout<<"Sorry! Entered Account Number or PIN is Invalid"<<endl;
        return 0;

    }
     
    cout<<"Welcome! "<<users[index].name<<endl;
    while(true){
    cout<<"1. Withdraw money"<<endl;
    cout<<"2. Change PIN"<<endl;
    cout<<"3. Check balance"<<endl;
    cout<<"4. Exit"<<endl;
    int choice;
    cin>>choice;
    bool change=false;
    if(choice==1){
        withdrawmoney(users[index]);
        change=true;
    }
    else if(choice==2){
        changepin(users[index]);
        change=true;
    }
    else if(choice==3){
        checkbalance(users[index]);
    }
    else if(choice==4) break;

    else cout<<"Enter Correct Choice"<<endl;

    if(change){
        saveusers(users);
    }
    
    
    
    
    }
}

