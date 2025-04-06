#include "main.hpp"
#include "sockets.hpp"

using namespace std;

int createSingleUser();
int createUsersInteractive();


//this is where we load the users.txt data into
list<login> myLogins;
//list of all cookies, which are removed when they get accessed after expiration. These do not get saved to the harddrive on exit().
list<cookie> myCookies;

int main(int argc, char** argv){
    //interactive mode
    if(argc == 2 && strcmp(argv[1], "-i") == 0){
        return createUsersInteractive();
    }
    cout << "argc: " << argc << endl << "argv[1]: " << argv[1] << endl;
    //just make a single default value user
    return createSingleUser();
}

int createUsersInteractive(){
    initDB();

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
            cout << "Error: couldn't add user, username already taken!\nPress enter to continue...";
            cin;
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

    initDB();

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