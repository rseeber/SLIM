#include "../login.hpp"
//#include "sockets.hpp"

#include <iostream>
#include <list>             
#include <fstream>
#include <string>
#include <algorithm>
#include <openssl/sha.h>
#include <openssl/rand.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

using namespace std;

int createSingleUser();
int createUsersInteractive();
int userLogin();
void handleLogin();


int main(int argc, char** argv){
    initDB();

    //interactive mode
    if(argc == 2){
        //interactive user creation
        if(strcmp(argv[1], "-i") == 0){
            return createUsersInteractive();
        }
        //login as user
        if(strcmp(argv[1], "-l") == 0){
            return userLogin();
        }
    }
    //just make a single default value user
    return createSingleUser();
}

int userLogin(){

    initCookieDB();
    //print logged in users
    cout << 
        "===================\n" << 
        " Logged in users \n" << 
        "===================\n";
    cout << getLoggedInUsers_string() << "\n\n";

    // Login or log out
    cout << "What do you want to do?\n\ta) log a user in\n\tb) log out a user\n\n> ";
    char opt;
    cin >> opt;

    //log in
    if(opt == 'a'){
        handleLogin();
    }
    //logout
    else if(opt == 'b'){
        cout << "What user would you like to log out?\n\n";
        cout << "Username: ";
        string username;
        cin >> username;

        //logout with username
        logout(username);
    }

    saveCookieDB();
    
    return 0;
}

void handleLogin(){
    string username, passwd;
    cookie c;
    int repeat = 0;
    //login attempt loop
    do{
        if(repeat){
            cout << "Oops! Wrong username or password. Try again.\n";
        }
        cout << "=== Log In ===\n";
        
        cout << "Username: ";
        cin >> username;
        cout << "Password: ";
        cin >> passwd;
        repeat = 1;

    //keep forcing them to log in until they get the password right
    }while(loginAsUser(username, passwd, &c) < 0);

    cout << "Login Successful!!\n\n";
    cout << "Welcome back " << c.user << "!\n";
    cout << "Login token = " << c.token << "\n";
}

int createUsersInteractive(){
    int cnt;
    cout << "How many users do you wish to create?\n> ";
    cin >> cnt;

    //create some number of users iteratively
    for(int i = 0; i < cnt; ++i){
        string user;
        string email;
        string password;
        //grab user input for current user
        cout << "===========" << endl << " USER #" << i+1 << endl << "===========" << endl;
        cout << "Input username\n> ";
        cin >> user;
        cout << "Input email\n> ";
        cin >> email;
        cout << "Input password\n> ";
        cin >> password;

        //save current user
        //error checking
        if (addUser(user, email, password) < 0){
            cout << "Error: couldn't add user, username already taken!\nTry again.\n";
            --i;    //don't increase the count of users if the user wasn't actually added
            continue;
        }
        cout << "Successfully added user '" << user << "'" << endl;
    }
    
    cout << "Successfully created " << cnt << " users." << endl;
    
    saveDB();

    cout << "Successfully updated database!" << endl;
    

    //now make them try to login until they get the password right, or they send interrupt signal (^C).
    int pass = 0;
    while(!pass){
        string user, password;
        cout << "Attempt to login as a user!" << endl;
        cout << "Username: ";
        cin >> user;
        cout << "Password: ";
        cin >> password;
        
        cookie c;
        if (loginAsUser(user, password, &c) == 0){
            cout << "Successfully logged in as " << user << ". Token = " << c.token << endl;
            pass = 1;
        }
        else{
            cout << "Username or password does not match. Try again." << endl;
        }
    }
    return 0;
}

int createSingleUser(){
    string user = "River";
    string email = "river@email.com";
    string password = "badPassword1";

    if(addUser(user, email, password) < 0){
        cout << "(Did you forget to delete 'users.txt'?)" << endl;
        return -1;
    }
    
    cookie c;
    if (loginAsUser(user, password, &c) == 0){
        cout << "Logged in as " << user << ". Token = " << c.token << endl;
    }
    else {
        cout << "error while logging in!! Checking users.txt may help." << endl;
    }

    //save db even if login fails, so as to help debug the issue
    saveDB();

    return 0;
}