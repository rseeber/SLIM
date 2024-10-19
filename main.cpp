#include <iostream>
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
//hashes the passwd with the salt, storing it at the buffer pointed to by buf
int hashPasswd(string passwd, string salt, string* buf);


int addUser(string user, string email, string passwd){
    
}