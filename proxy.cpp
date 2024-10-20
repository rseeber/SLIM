#include <iostream>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <fstream>

using namespace std;

#define API_MAX_LEN 100

int main(int argc, char** argv){

    char* call = "/tmp/PMS-fifo_CALL";
    char* response = "/tmp/PMS-fifo_RESPONSE";
    
    int fdCall, fdRes;

    if(fdRes = open(call, O_WRONLY) < 0){
        cout << "error on open 1\n";
    }
    if (fdCall = open(response, O_RDONLY) < 0){
        cout << "error on open 2\n";
    }

    char bufIn[API_MAX_LEN];
    string bufOut;

    ifstream fin(response);


    cout << "entering loop...\n";
    while(1){
        bufOut.assign("Hello, World\n");
        cout << "wrote n bytes: " << write(fdCall, bufOut.c_str(), bufOut.length()) << endl;



        cout << "read n bytes: " << read(fdRes, bufIn, API_MAX_LEN) << endl;
        break;
    }

}