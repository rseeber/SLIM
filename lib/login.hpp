#include <string>
#include <list>

using namespace std;

struct login {
    unsigned int userID; //new
    string user;
    string email;
    string passHash;
    string salt;
};

struct cookie {
    unsigned int userID;
    string user;    //depricated, phase out
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

//load the database into ram as an array of structs
int initDB();

//saves the database back into users.txt
void saveDB();

