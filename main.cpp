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
using namespace std;

#define SALT_LEN 16
#define HASH_LEN 32
#define API_MAX_LEN 100
#define COOKIE_EXPIRY_LEN_SECONDS 600   //10 minutes = 60 seconds * 10 = 600 seconds

struct login {
    string user;
    string email;
    string passHash;
    string salt;
};

struct cookie {
    string user;
    int token;
    int expiry;
};

//creates a new user in the database
int addUser(string user, string email, string passwd);
//change passwd
int editUser(string user, string oldPass, string newPass);
//login, returinging a temporary login cookie for the user
string loginAsUser(string user, string passwd);
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



//this is where we load the users.txt data into
list<login> myLogins;
//list of all cookies, which are removed when they get accessed after expiration. These do not get saved to the harddrive on exit().
list<cookie> myCookies;

int main(int argc, char** argv){
    initDB();

    //fifoSocket();
    addUser("Donna", "d.donna@email.com", "badPassword1!");

    login l;
    if(findUser("Donna", &l) < 0){
        cout << "uh-oh\n";
    }
    
    cout << "Printing login for " << l.user << endl;
    printUser(l, 0);

    exit();
    return 0;
}

int fifoSocket(){
    char* call = "/tmp/PMS-fifo_CALL";
    char* response = "/tmp/PMS-fifo_RESPONSE";

    string bufIn;
    string bufOut;

    cout << "creating fifo named pipes\n";
    /* create the FIFO (named pipe) */
    mkfifo(call, 0666);
    mkfifo(response, 0666);

    cout << "calling ./proxy\n";
    //system("./proxy");

    ifstream fin(call);
    ofstream fout(response);
    string line;

    cout << "starting while loop\n";
    while(1){

        fin >> bufIn;
        cout << "read '" << bufIn << "' from proxy\n";
        
        bufOut.assign("You just said '"+(string)bufIn+"'\n");
        cout << "writing '" << bufOut << "' to proxy\n";
        fout << bufOut;

        break;
    }

    cout << "The Conversation:\n\n";
    cout << "[PROXY]: " << bufIn << endl;
    cout << "[MAIN]: " << bufOut << endl;
}

void exit(){
    saveDB();
}

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

//loads the database into ram as a list of login structs
int initDB(){
    string line;
    ifstream ifs("users.txt");
    //save the current line of the document to `line`
    for(int i = 0; ifs.good(); ++i){
        login l;
        //now break up the line into valid pieces to save as each part of the login
        ifs >> l.user >> l.email >> l.passHash >> l.salt;

        //don't save empty values to myLogins
        if(l.user == ""){
            continue;
        }

        //append l to myLogins
        myLogins.push_back(l);
    }
    ifs.close();

    return 0;
}

void saveDB(){
    ofstream ofs("users.txt");
    for(login l : myLogins){
        //write the data from the list of structs into the text file
        ofs << l.user << "\t" << l.email << "\t" << l.passHash << "\t" << l.salt;
        
        //only print a newline if we're NOT on the last entry
        if(myLogins.back().user != l.user){   //compare the usernames
            ofs << endl;
        }
    }
    ofs.close();
}


//creates a new user, with the given info, unless that username is already taken. Returns -1 in that case, or 0 if everything worked.
int addUser(string user, string email, string passwd){
    login l;
    l.user = user;

    //first check that this user does not already exist in the system
    if(binary_search(myLogins.begin(), myLogins.end(), l)){
        cout << "error: cannot addUser '" << user << "', as this name is already in use!\n";
        return -1;
    }

    //otherwise, proceed
    l.email = email;
    hashPasswd(passwd, &l.salt, &l.passHash);
    //append l to myLogins
    myLogins.push_back(l);
    //sort myLogins
    myLogins.sort();
    return 0;
}

int loginAsUser(string user, string passwd, cookie* cook){
    //find the login entry for the desired user
    login l;
    if(findUser(user, &l) < 0){
        return -1;
    }

    //check that the passwords match
    string hash;
    hashPasswd(passwd, l.salt, &hash);
    if(hash != l.passHash){
        cout << "error: couldn't login. Either passwords do not match, or database is corrupted. Aborting.\n";
        return -1;
    }
    cout << "Login successful, generating random login token/cookie...\n";
    //return a unique, random cookie value
    return generateCookie(user, cook);
}

//generates a cookie, setting it at the pointer cook, as well as REGISTERING IT IN THE COOKIE DATABASE
int generateCookie(string user, cookie* cook){
    
    int token;
    if(RAND_bytes((unsigned char*)(&token), sizeof(int)) < 0){
        cout << "error: couldn't generate random data\n";
        return -1;
    }
    int expiry = time(nullptr) + COOKIE_EXPIRY_LEN_SECONDS;

    //create the cookie
    cookie c;
    c.user = user;
    c.token = token;
    c.expiry = expiry;
    //check to see if we already have this user in our cookie database
    list<cookie>::iterator it = find(myCookies.begin(), myCookies.end(), c);
    //if the user already has a registered cookie, delete the old one, and use the new one
    if(it != myCookies.end()){
        *it = c;
    }
    //otherwise, add a new cookie entry to the cookie database
    myCookies.push_back(c);
    sort(myCookies.begin(), myCookies.end());

    //assign the value of cook to be our newly generated cookie entry
    *cook = c;
    return 0;
}

//goes through the database, and returns the login referring to the provided user
int findUser(string user, login* log){
    login l;
    l.user = user;
    list<login>::iterator it = find(myLogins.begin(), myLogins.end(), l);
    if(it == myLogins.end()){
        cout << "error: couldn't find specified user";
        return -1;
    }
    //assign log with the value stored at it
    *log = *it;
    return 0;
}

//Hashes a password that you already have the salt for
int hashPasswd(string passwd, string salt, string* buf){
    //append salt to the password before hashing
    string saltedPasswd = passwd + salt;
    char* hash = (char*)malloc(33); //32 bytes + null terminator

    //hash the passwd+salt combo
    SHA256((unsigned char*)saltedPasswd.c_str(), saltedPasswd.size(), (unsigned char*)hash);


    char* hashHex = (char*)malloc(HASH_LEN*2 + 1);  //hex text for the hash output
    toHex((unsigned char*)hash, HASH_LEN, hashHex);

    buf->assign(hashHex);   //save the hash as hex text
    return 0;
}

//Hashes a password that you do not have the salt for, and assignes the salt to the pointer you pass in
int hashPasswd(string passwd, string* salt, string* buf){
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

//converts Hexidecimal text back into binary data
void toBinary(const char* hex, size_t N, unsigned char* data){

}


bool operator<(const login& first, const login& second){
    return (first.user < second.user) ? true : false;
}

bool operator==(const login& a, const login& b){
    return (a.user == b.user) ? true : false;
}

bool operator==(const login& a, const string& b){
    return (a.user == b) ? true : false;
}

bool operator==(const cookie& a, const cookie& b){
    return (a.user == b.user) ? true : false;
}

bool operator<(const cookie& a, const cookie& b){
    return (a.user < b.user) ? true : false;
}