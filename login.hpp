#include <string>

using namespace std;

struct login {
    string user;
    string email;
    string passHash;
    string salt;
};

struct cookie {
    string user;
    unsigned int token;
    int expiry;
};

//creates a new user in the database with the given info, unless that username is already taken. Returns 0 on sucess, or -1 on error.
int addUser(string user, string email, string passwd);
//change passwd
int editUser(string user, string oldPass, string newPass);
//login, setting 'cook' as a temporary login cookie for the user, returning 0 on success, or -1 on error.
int loginAsUser(string user, string passwd, cookie* cook);
//generates a unique cookie, saving it to the database, or overwriting any old cookie for that user.
int generateCookie(string user, cookie* cook);
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
//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);
//hashes passwd, generating a new salt stored at *salt. Hash is stored at *buf.
int hashPasswd(string passwd, string* salt, string* buf);

void exit();