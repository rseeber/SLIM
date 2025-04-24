#include <string>
#include <list>

using namespace std;

struct login {
    int userID; //new
    string user;
    string email;
    string passHash;
    string salt;
};

struct cookie {
    int userID;
    //string user;    //depricated, phase out
    unsigned int token;
    time_t expiry;
};

// === user space functions ===

//creates a new user in the database with the given info, unless that username is already taken. Returns 0 on sucess, or -1 on error.
int addUser(string user, string email, string passwd);

//changes the password for the user with the given userID. Generates new salt as well.
// NOTE: the server should verify the user before running this function
int editPasswd(int userID, string newPass);

//changes the username of the user with the given userID.
// NOTE: the server should verify the user before running this function
int editUsername(int userID, string newUsername);

//changes the email for the user with the given userID.
// NOTE: the server should verify the user before running this function
int editEmail(int userID, string newEmail);

//login, setting 'cook' as a temporary login cookie for the user, returning 0 on success, or -1 on error.
int loginAsUser(string user, string passwd, cookie* cook);

//revokes a cookie token early. Returns the userID of the logged out user on success, or -1 on error.
int logout(unsigned int token);

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