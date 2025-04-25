#include "login.hpp"

// === internal functions ===

//generates a unique cookie, saving it to the database, or overwriting any old cookie for that user.
int generateCookie(int userID, cookie* cook);

//checks if both cookies have identical values in all fields
bool cookiesEqual(cookie c1, cookie c2);

//write the requested data to the buffer pointed to by buf
int getUserData(string user, string cookie, int dataID, string* buf);



//returns the current username for a given userID
string getUsername(int userID);





//prints the specified login
void printUser(login l, int i);



//converts binary data into the cleaner hexidecimal format
void toHex(unsigned char* hashData, __ssize_t dataLen, char* hashString);

//converts Hexidecimal text back into binary data
void toBinary(const char* hex, size_t N, unsigned char* data);



//TODO: change this. We should NOT have function overloads that perform
// substantially different behaviors (notably, one takes IN salt, and one OUTPUTS it to a pointer location)

//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);

//hashes passwd, generating a new salt stored at *salt. Hash is stored at *buf.
int hashPasswd(string passwd, string* salt, string* buf);

void exit();