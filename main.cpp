#include <iostream>
#include <list>
#include <fstream>
#include <string>
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

//UTILITY FUNCTIONS
//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);


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