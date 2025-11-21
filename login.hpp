/*
 * Project: Simple LogIn Manager (SLIM) Library
 * Author: River Seeber
 * Date: May 2025
 * Version: 1.1.0 development
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



#include <string>
#include <list>

using namespace std;

struct login {
    int userID; //new
    string user;
    string passHash;
    string salt;
};

struct cookie {
    int userID;
    string user;    //Do not use username for verification. userID's are the only unique ID.
    unsigned int token;
    time_t expiry;
};

//returns a list of logins corresponding to valid cookies in our cookie DB
list<login> getLoggedInUsers();

// wrapper function for getLoggedInUsers(), to simply print 
// username and email of each user, with a newline seperating each user.
string getLoggedInUsers_string();

//creates a new user in the database with the given info, unless that username is already taken. Returns 0 on sucess, or -1 on error.
int addUser(string user, string passwd);

//changes the password for the user with the given userID. Generates new salt as well.
// NOTE: the server should verify the user before running this function
int editPasswd(int userID, string newPass);

//changes the username of the user with the given userID.
// NOTE: the server should verify the user before running this function
int editUsername(int userID, string newUsername);

//login, setting 'cook' as a temporary login cookie for the user, returning 0 on success, or -1 on error.
int loginAsUser(string user, string passwd, cookie* cook);

//revokes a cookie token early. Returns the userID of the logged out user on success, or -1 on error.
int logout(unsigned int token);

//wrapper for logout, which find the token by getting the userID from 
//the login db, then getting the token from the cookei db.
int logout(string username);

//searches database for an unexpired cookie with the given token value. Returns the associated userID, or -1 on failure.
int validateToken(unsigned int token);

//removes a user associated with a given userID from the login database. returns 0 on success, or -1 if no such user exists.
int deleteUser(int userID);

//load the database into ram as an array of structs
void initDB();

//load the cookie database into ram as an array of structs
void initCookieDB();

//saves the database back into users.txt
void saveDB();

//saves the database back into cookies.txt
void saveCookieDB();

//literally just print the whole database
void printDB();

// same as printDB(), but it prints cookies, not users
void printCookieDB();

// wrapper for printUser, except that it prints a given index number, useful for outputting a database.
void printUser_i(login l, int i);

//prints the username, email, password hash, and salt. All entries tab spaced, and ended with a newline.
void printUser(login l);

//wrapper for printCookie() that prints given index as well
void printCookie_i(cookie c, int i);

//prints info for a cookie: userID, username, token, and expiry timestamp
void printCookie(cookie c);

//returns the current username for a given userID
string getUsername(int userID);

//returns the login for user on the login pointed to by log
int findUserByName(string user, login* log);

//finds a user from the users database, and puts the iterator at the location pointed to by *it. 
// Returns 0 on success or -1 on error.
int findUserByID(int userID, login *l);

//finds a cookie from the cookie database, and puts the iterator at the location pointed to by *it. 
// Returns 0 on success or -1 on error.
int findCookieByUserID(int userID, cookie *c);