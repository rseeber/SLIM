#include "main.hpp"

using namespace std;

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
    
    cout << "Printing login for " << l.user << ":\n";
    printUser(l, 0);

    exit();
    return 0;
}