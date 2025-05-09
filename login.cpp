/*
 * Project: Simple LogIn Manager (SLIM) Library
 * Author: River Seeber
 * Date: April 2025
 * Version: 1.0.0
 */

/*
 *  COPYRIGHT 2025 River Seeber.
 *
 *  This file is part of SLIM (Simple LogIn Manager).
 *
 *  SLIM is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  SLIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with SLIM. If not, see <https://www.gnu.org/licenses/>. 
 */

#include "internal.hpp"

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

#define SALT_LEN 16
#define HASH_LEN 32
#define API_MAX_LEN 100
#define COOKIE_EXPIRY_LEN_SECONDS 600   //10 minutes = 60 seconds * 10 = 600 seconds


//this is where we load the users.txt data into
static list<login> myLogins;
//list of all cookies, which are removed when they get accessed after expiration.
static list<cookie> myCookies;

void printDB(){
    int i = 0;
    for(login l : myLogins){
        printUser(l, i);
        //cout << "[" << i++ << "]: " << l.user << "\t\t" << l.email << "\t\t" << l.passHash << "\t\t" << l.salt << endl;
    }
}

void printUser(login l, int i){
    cout << "[" << i++ << "]: " << l.user << "\t" << l.email << "\t" << l.passHash << "\t" << l.salt << endl;
}

//returns a list of logins corresponding to valid cookies in our cookie DB
list<login> getLoggedInUsers(){
    //initialize an empty list of logged in users
    list<login> users;
    //iterate through each cookie in the db
    for(cookie c : myCookies){
        //get the userID by validating it (to prove it's not expired!!)
        int userID;
        if((userID = validateToken(c.token)) < 0){
            cout << "Skipping expired token!\n";
            //cookie is expired, skip it!
            continue;
        }
        //the current login we're dealing with
        login l;
        //find the user and store them on l.
        if(findUserByID(userID, &l) < 0){
            printf("Warning: user '%s' exists in the cookieDB, but not in the userDB!\n", c.user);
            continue;   //if the user doesn't exist in the database, skip them.
        }
        //append that login VALUE to our list of logged in users
        users.push_back(l);
    }
    //return our list of logged in users
    return users;
}

// wrapper function for getLoggedInUsers(), to simply print 
// username and email of each user, with a newline seperating each user.
string getLoggedInUsers_string(){
    //collect all the logged in users as a list
    list<login> users = getLoggedInUsers();

    string output = "";
    //iterate through each logged in user
    for(login l : users){
        //append the username and email to the output, followed by newline
        output += "  * " + l.user + "\n";
    }
    return output;
}

//loads the database into ram as a list of login structs
void initDB(){
    string line;
    ifstream ifs("data/users.txt");
    //check if the file exists
    if(!ifs.good()){
        //create the file if it does not exist
        ofstream ofs("data/users.txt");
        ofs << "";
        ofs.close();
    }
    //save the current line of the document to `line`
    for(int i = 0; ifs.good(); ++i){
        login l;
        //now break up the line into valid pieces to save as each part of the login
        ifs >> l.userID >> l.user >> l.email >> l.passHash >> l.salt;

        //append l to myLogins
        myLogins.push_back(l);
    }
    ifs.close();
}

void initCookieDB(){
    //written VERY similiar to initDB() function
    string line;
    ifstream ifs("data/cookies.txt");
    //check if the file exists
    if(!ifs.good()){
        //create the file if it does not exist
        ofstream ofs("data/cookies.txt");
        ofs << "";
        ofs.close();
    }
    //take formatted input using >> operator. Expect 3 input fields per line.
    else for(int i = 0; ifs.good(); ++i){
        cookie c;
        //now break up the line into valid pieces to save as each part of the cookie
        ifs >> c.userID >> c.user >> c.token >> c.expiry;

        //append cookie to myLogins
        myCookies.push_back(c);
    }
    ifs.close();
}

void saveDB(){
    ofstream ofs("data/users.txt");
    for(login l : myLogins){
        //write the data from the list of structs into the text file
        ofs << l.userID << "\t" << l.user << "\t" << l.email << "\t" << l.passHash << "\t" << l.salt;
        
        //only print a newline if we're NOT on the last entry
        if(myLogins.back().userID != l.userID){   //compare the usernames
            ofs << endl;
        }
    }
    ofs.close();
}

void saveCookieDB(){
    //written VERY similiar to saveDB() function
    ofstream ofs("data/cookies.txt");
    for(cookie c : myCookies){
        //write the data from the list of structs into the text file
        ofs << c.userID << "\t" << c.user << "\t" << c.token << "\t" << c.expiry;
        
        //only print a newline if we're NOT on the last entry
        //NOTE: if a user can smuggle two cookies into our db, he can mess up our db formatting
        if(myCookies.back().userID != c.userID){   //compare the userID
            ofs << endl;
        }
    }
    ofs.close();
}

int addUser(string user, string email, string passwd){
    //first check that this user does not already exist in the system
    //NOTE: we do this because the username still must be unique to each user. No duplicates allowed.
    if(findUserByName(user, nullptr) >= 0){
        //cout << "error: cannot addUser '" << user << "', as this name is already in use!\n";
        return -1;
    }
    login l;
    //add basic values
    l.user = user;
    l.email = email;

    //generate a random userID
    int id;
    //generate random userID. If a user with that ID already exists, generate a new one until it's unique.
    while(RAND_bytes((unsigned char*)&id, sizeof(int)) < 0 && findUserByID(id, nullptr) >= 0);
    //we do abs() so we can return -1 on error for lookups
    l.userID = abs(id);

    //password hashing
    hashPasswd_generateSalt(passwd, &l.salt, &l.passHash);

    //append l to myLogins
    myLogins.push_back(l);
    //sort myLogins
    myLogins.sort();
    return 0;
}

int loginAsUser(string user, string passwd, cookie* cook){
    //find the login entry for the desired user
    login l;
    if(findUserByName(user, &l) < 0){
        return -1;
    }

    //check that the passwords match
    string hash;
    hashPasswd(passwd, l.salt, &hash);
    if(hash != l.passHash){
        cout << "error: couldn't login. Either passwords do not match, or database is corrupted. Aborting.\n";
        cout << "Input Hash : " << hash << endl;
        cout << "Database Hash: " << l.passHash << endl;
        return -1;
    }
    //cout << "Login successful, generating random login token/cookie...\n";
    //set a unique, random cookie value. And return success value.
    return generateCookie(l.userID, cook);
}

//revokes a cookie token early. 
int logout(unsigned int token){
    //find the cookie by username
    list<cookie>::iterator it = find(myCookies.begin(), myCookies.end(), token);
    //if found
    if(it != myCookies.end()){
        //revoke cookie
        //NOTE: this will remove ALL cookies for a logged in user, not just the particular token provided. This is a good thing.
        myCookies.remove(*it);
        //return the userID of the logged out user.
        return it->userID;
    }
    //not found
    return -1;
}

//wrapper for logout, which find the token by getting the userID from 
//the login db, then getting the token from the cookei db.
int logout(string username){
    //get the userID
    int userID;
    login l;
    findUserByName(username, &l);
    userID = l.userID;

    //find the cookie struct, so we can extract the token from it
    list<cookie>::iterator it;
    cookie c;
    if(findCookieByUserID(userID, &c) < 0){
        cout << "Error: couldn't find user with userID " << userID << ", and username " << username <<".\n";
        return -1;
    }

    //call inner function to logout with the token.
    return logout(it->token);
}

//searches database for an unexpired cookie with the given token value. Returns the associated userID, or -1 on failure.
int validateToken(unsigned int token){
    //check if the given token exists in the cookie database
    list<cookie>::iterator it = find(myCookies.begin(), myCookies.end(), token);

    //if we found the cookie
    if(it != myCookies.end()){
        //check if it's expired
        time_t t = time(nullptr);
        //distance between now and when the cookie expires, in seconds
        // positive means it's not expired yet (time till expiry)
        time_t diff = it->expiry - t;
        //we check t < 0 in case time() returned an error
        if(diff > 0 && t >= 0){
            return it->userID;
        }
    }
    return -1;
}

//removes a user associated with a given userID from the login database. returns 0 on success, or -1 if no such user exists.
int deleteUser(int userID){
    login l;
    if(findUserByID(userID, &l) < 0){
        //error, can't find user
        return -1;
    }
    //remove the user
    myLogins.remove(l);
    return 0;
}

//generates a cookie, setting it at the pointer cook, as well as REGISTERING IT IN THE COOKIE DATABASE
int generateCookie(int userID, cookie* cook){
    
    //generate random bytes for the token value, and assign them to `token`.
    unsigned int token;
    if(RAND_bytes((unsigned char*)(&token), sizeof(int)) < 0){
        cout << "error: couldn't generate random data\n";
        return -1;
    }
    //NOTE: if time() fails, it returns -1, so the token is instantly expired (it was created in 1970).
    //This relies on the basic assumption that the epoch started more than COOKIE_EXPIRY_LEN_SECONDS ago.
    time_t expiry = time(nullptr) + COOKIE_EXPIRY_LEN_SECONDS;
    
    //determine username
    login l;
    if(findUserByID(userID, &l) < 0){
        cout << "Error: couldn't find user with id = " << userID << endl;
        return -1;
    }
    string username = l.user;

    //create the cookie struct
    cookie c;
    //assign values
    c.userID = userID;
    c.user = username;
    c.token = token;
    c.expiry = expiry;

    //check to see if we already have this user in our cookie database
    list<cookie>::iterator it = find(myCookies.begin(), myCookies.end(), c);    //find(cookie) uses cookie.userID to compare
    findCookieByUserID(userID, &it);
    //if the user already has a registered cookie, delete the old one, and use the new one
    if(it != myCookies.end()){
        *it = c;
    }
    //otherwise, add a new cookie entry to the cookie database
    myCookies.push_back(c);
    myCookies.sort();   //unnecessary, since we're using a linear search algorithm. Remove?

    //assign the value of cook to be our newly generated cookie entry
    *cook = c;
    return 0;
}

//finds a user from the users database, and stores the login at the location pointed to by *l. 
// Returns 0 on success or -1 if the user was not found.
int findUserByID(int userID, login *l){
    list<login>::iterator it = find(myLogins.begin(), myLogins.end(), userID);
    //if not found
    if(it == myLogins.end()){
        return -1;
    }

    //convert iterator to login pointer
    *l = *it;
    return 0;
}

//finds a cookie from the cookie database, and puts the iterator at the location pointed to by *it. 
// Returns 0 on success or -1 on error.
int findCookieByUserID(int userID, cookie c){
    list<cookie>::iterator it;
    cookie tempCook;    //this allows us to use find().
    tempCook.userID = userID;
    //note: we cannot use function overloads, since
    // operator<(int, cookie) is already defined to work in terms
    // of token -- not userID.
    it = find(myCookies.begin(), myCookies.end(), tempCook);
    if(it == myCookies.end()){
        return -1;
    }
    c = *it;
    return 0;
}

//returns the current username for a given userID
string getUsername(int userID){
    list<login>::iterator it;
    //find the login object
    if(findUserByID(userID, &it) < 0){
        //error
        return "";
    }
    // pull out the username
    return it->user;
}

//changes the password for the user with the given userID. Generates new salt as well.
// NOTE: the server should verify the user before running this function
int editPasswd(int userID, string newPass){
    //find user
    list<login>::iterator it;
    if (findUserByID(userID, &it) < 0){
        //not found
        return -1;
    }
    //generate a new password
    string passHash, salt;
    if(hashPasswd_generateSalt(newPass, &salt, &passHash) < 0){
        //couldn't hash password. Try again.
        return -1;
    }
    //save the passHash and salt to our user.
    it->passHash = passHash;
    it->salt = salt;
    return 0;
}

//changes the username of the user with the given userID.
// NOTE: the server should verify the user before running this function
int editUsername(int userID, string newUsername){
    //find user
    list<login>::iterator it;
    if(findUserByID(userID, &it) < 0){
        return -1;
    }

    //set the username
    it->user = newUsername;
    return 0;

}

//changes the email for the user with the given userID.
// NOTE: the server should verify the user before running this function
int editEmail(int userID, string newEmail){
    //find user
    list<login>::iterator it;
    if(findUserByID(userID, &it) < 0){
        return -1;
    }

    //change email
    it->email = newEmail;
    return 0;
}


//TODO: rename this function or it's previous accidental override. Although they both have a decent
//overlap in functionality, they're intended for different ends of the application (i think). Make a good fix.

//goes through the database, and returns the login referring to the provided user
int findUserByName(string user, login* l){
    list<login>::iterator it = find(myLogins.begin(), myLogins.end(), user);
    if(it == myLogins.end()){
        //cout << "error: couldn't find specified user";
        return -1;
    }

    //save the login to l, unless l is a nullptr
    if(l != nullptr){
        *l = *it;
    }
    return 0;
}

//Hashes a password that you already have the salt for
int hashPasswd(string passwd, string salt, string* buf){
    //append salt to the password before hashing
    string saltedPasswd = passwd + salt;
    char* hash = (char*)malloc(33); //32 bytes + null terminator

    //hash the passwd+salt combo
    //cout << "hashing \"" << saltedPasswd << "\"\n";
    SHA256((unsigned char*)saltedPasswd.c_str(), saltedPasswd.size(), (unsigned char*)hash);

    char* hashHex = (char*)malloc(HASH_LEN*2 + 1);  //hex text for the hash output
    toHex((unsigned char*)hash, HASH_LEN, hashHex);
    //cout << "result: \"" << hashHex << "\"\n";

    buf->assign(hashHex);   //save the hash as hex text
    return 0;
}

//Hashes a password that you do not have the salt for, and assignes the salt to the pointer you pass in
int hashPasswd_generateSalt(string passwd, string* salt, string* buf){
    char* saltData = (char*)malloc(SALT_LEN + 1);   //binary data
    //get random bytes for salt
    if(RAND_bytes((unsigned char*)saltData, SALT_LEN) < 0){
        //check to ensure RAND_bytes didn't return an error
        cout << "ERROR: couldn't generate random salt\n";
        return -1;
    }
    
    char* saltHex = (char*)malloc(2*SALT_LEN + 1);  //hex value saved as readable hex text
    //the array for the hex version of the number
    toHex((unsigned char*)saltData, SALT_LEN, saltHex);

    salt->assign(saltHex);  //save the hex value to the salt pointer location

    return hashPasswd(passwd, *salt, buf);
}

//hashString should be pre-allocated to double + 1 the size of hashData -> (32 * 2) + 1 = 65
void toHex(unsigned char* hashData, __ssize_t dataLen, char* hashString){
    for(int i = 0; i < dataLen; ++i){
        //write the hex into hashString, one byte at a time (2 hex digits)
        snprintf(hashString+(2*i), 3, "%02X", hashData[i]);
    }
}

//operator overloads used for sorting entries in order to make searching the db efficient

//compare logins
//NOTE: recent updates have changed comparisons from username-username to userID-userID (!!)
bool operator<(const login& a, const login& b){
    return (a.userID < b.userID);
}
bool operator==(const login& a, const login& b){
    return (a.userID == b.userID);
}

//compare login with username
bool operator==(const login& a, const string& b){
    return (a.user == b);
}

//login with userID
bool operator<(const login& a, const int& b){
    return (a.userID < b);
}
bool operator==(const login& a, const int& b){
    return (a.userID == b);
}

//compare cookies with userID
bool operator==(const cookie& a, const cookie& b){
    return (a.userID == b.userID);
}
bool operator<(const cookie& a, const cookie& b){
    return (a.userID < b.userID);
}

//compare cookie with token
bool operator==(const cookie& a, const unsigned int b){
    return a.token == b;
}
bool operator<(const cookie& a, const unsigned int b){
    return a.token < b;
}