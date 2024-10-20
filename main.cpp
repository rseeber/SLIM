#include <iostream>
#include <list>
#include <fstream>
#include <string>
#include <algorithm>
using namespace std;

//creates a new user in the database
int addUser(string user, string email, string passwd);
//change passwd
int editUser(string user, string oldPass, string newPass);
//login, returinging a temporary login cookie for the user
string loginAsUser(string user, string passwd);
//write the requested data to the buffer pointed to by buf
int getUserData(string user, string cookie, int dataID, string* buf);
//load the database into ram as an array of structs
int initDB();
//literally just print the whole database
void printDB();
//saves the database back into users.txt
void saveDB();

//UTILITY FUNCTIONS
//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);
//hashes passwd, generating a new salt stored at *salt. Hash is stored at *buf.
int hashPasswd(string passwd, string* salt, string* buf);


struct login {
    string user;
    string email;
    string passHash;
    string salt;
};

//this is where we load the users.txt data into
list<login> myLogins;

int main(){
    initDB();
    //after init, the user can choose which actions they want to take
    addUser("James", "jj-jamison@gmail.com", "reallybadPasswd");
    printDB();
    saveDB();
}

void printDB(){
    int i = 0;
    for(login l : myLogins){
        cout << "[" << i++ << "]: " << l.user << "\t\t" << l.email << "\t\t" << l.passHash << "\t\t" << l.salt << endl;
    }
}

//loads the database into ram as a list of login structs
int initDB(){
    string line;
    ifstream ifs("users.txt");
    //save the current line of the document to `line`
    for(int i = 0; ifs.good(); ++i){
        login l;
        //now break up the line into valid pieces to save as each part of the login
        ifs >> l.user >> l.email >> l.passHash >> l.salt;

        //append l to myLogins
        myLogins.push_back(l);
    }
    ifs.close();

    return 0;
}

void saveDB(){
    ofstream ofs("users.txt");
    for(login l : myLogins){
        //write the data from the list of structs into the text file
        ofs << l.user << "\t" << l.email << "\t" << l.passHash << "\t" << l.salt;
        
        //only print a newline if we're NOT on the last entry
        if(myLogins.back().user != l.user){   //compare the usernames
            ofs << endl;
        }
    }
}



int addUser(string user, string email, string passwd){
    login l;
    l.user = user;

    //first check that this user does not already exist in the system
    if(binary_search(myLogins.begin(), myLogins.end(), l)){
        return -1;
    }
    
    //otherwise, proceed
    l.email = email;
    hashPasswd(passwd, &l.salt, &l.passHash);
    //append l to myLogins
    myLogins.push_back(l);
    //sort myLogins
    myLogins.sort();
    return 0;
}

int hashPasswd(string passwd, string salt, string* buf){
    buf->assign("thisisabadhash");
    return 0;
}

int hashPasswd(string passwd, string* salt, string* buf){
    salt->assign("~~badSalt~~");
    return hashPasswd(passwd, *salt, buf);
}

bool operator<(const login& first, const login& second){
    return (first.user < second.user) ? true : false;
}

bool operator==(const login& a, const login& b){
    return (a.user == b.user) ? true : false;
}