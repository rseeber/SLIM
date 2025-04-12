#include <string>

using namespace std;

struct login {
    int userID; //new
    string user;
    string email;
    string passHash;
    string salt;
};

struct cookie {
    string user;
    unsigned int token;
    time_t expiry;
};

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

//generates a unique cookie, saving it to the database, or overwriting any old cookie for that user.
int generateCookie(string user, cookie* cook);

//checks if both cookies have identical values in all fields
bool cookiesEqual(cookie c1, cookie c2);

//write the requested data to the buffer pointed to by buf
int getUserData(string user, string cookie, int dataID, string* buf);

//returns the login for user on the login pointed to by log
int findUser(string user, login* log);



//converts binary data into the cleaner hexidecimal format
void toHex(unsigned char* hashData, __ssize_t dataLen, char* hashString);

//converts Hexidecimal text back into binary data
void toBinary(const char* hex, size_t N, unsigned char* data);

//load the database into ram as an array of structs
int initDB();

//literally just print the whole database
void printDB();

//prints the specified login
void printUser(login l, int i);

//saves the database back into users.txt
void saveDB();

//UTILITY FUNCTIONS
//TODO: change this. We should NOT have function overloads that perform
// substantially different behaviors (notably, one takes IN salt, and one OUTPUTS it to a pointer location)

//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);

//hashes passwd, generating a new salt stored at *salt. Hash is stored at *buf.
int hashPasswd(string passwd, string* salt, string* buf);

void exit();