#include "main.hpp"
#include "sockets.hpp"

using namespace std;

//this is where we load the users.txt data into
list<login> myLogins;
//list of all cookies, which are removed when they get accessed after expiration. These do not get saved to the harddrive on exit().
list<cookie> myCookies;

int main(int argc, char** argv){
    
    setupServer();

    return 0;
}