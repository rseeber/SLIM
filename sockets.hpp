#include <iostream>
#include <netinet/in.h>
//#include <stdio.h>
#include <stdlib.h>
//#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#define PORT 8080
#define HEADER 8
#define MAX_LEN 1024

using namespace std;

int Socket(int domain, int type, int protocol);
int Setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen);
int Bind(int socket, const struct sockaddr* addr, socklen_t len);
int Listen(int fd, int cnt);
int Accept(int socket, struct sockaddr* addr, socklen_t* len);
int Read(int fd, char* buf, size_t max_bytes);


int setupServer(){
    int sockfd, connfd;
    int opt = 1;
    struct sockaddr_in addr;

    cout << "Socket()...\n";
    sockfd = Socket(AF_INET, SOCK_STREAM, 0);

    Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt));

    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    cout << "Bind()...\n";
    Bind(sockfd, (struct sockaddr*)&addr, sizeof(addr));

    cout << "Listen()...\n";
    Listen(sockfd, 5);

    char buf[MAX_LEN];

    while(1){
        cout << "Accept()...\n";
        //blocking function
        if(connfd = Accept(sockfd, nullptr, nullptr) < 0){
            continue;
        }

        while(1){
            cout << "Read()...\n";
            Read(connfd, buf, MAX_LEN);
            cout << "\n[CLIENT]: " << buf << endl;
            break;
        }

        close(connfd);
        break;
    }
    return 0;
}

int Socket(int domain, int type, int protocol){
    int n;
    if((n = socket(domain, type, protocol) ) < 0){
        perror("Socket() error:");
        exit(n);    
    }
    return n;
}

int Setsockopt(int fd, int level, int optname, const void* optval, socklen_t optlen){
    int n;
    if(n = setsockopt(fd, level, optname, optval, optlen) < 0){
        perror("Setsockopt() error: ");
        exit(n);
    }
    return n;
}

int Bind(int socket, const struct sockaddr* addr, socklen_t len){
    int n;
    if(n = bind(socket, addr, len) < 0){
        perror("Bind() error: ");
        //close(socket);
        //exit(n);
    }
    return n;
}

int Listen(int fd, int cnt){
    int n;
    if(n = listen(fd, cnt) < 0){
        perror("Listen() error: ");
        exit(n);
    }
    return n;
}

int Accept(int socket, struct sockaddr* addr, socklen_t* len){
    int n;
    if((n = accept(socket, addr, len) ) < 0){
        perror("Accept() error  ");
        return -1;
        //since accept is called in a loop, we don't exit the program on error, 
        //since we get another chance to retry next time
    }
    return n;
}

//reads a 4 byte header, which indicates the number of bytes to be read in the body. It then
//reads up to that amount, or max_bytes (the smaller of the two) from the body. 
//Writes only the body to buf.
//returns the number of bytes read from the header and body combined, or -1 on error
int Read(int fd, char* buf, size_t max_bytes){
    int msgLen, m, n, cnt;

    //first read the 4 byte header to determine the length of the message body
    if(m = read(fd, &msgLen, sizeof(msgLen)) < 0){
        perror("Read() header error: ");
        return m;
    }

    //use the smaller number
    if(msgLen > max_bytes){
        msgLen = max_bytes;
    }

    //read msgLen bytes
    cnt = 0;
    while(n < msgLen){
        if(n = read(fd, buf, msgLen - cnt) < 0){
            perror("Read() body error: ");
            return n;
        }
        //print what we read
        for(int i = 0; i < n; ++i){
            printf("%c", buf[i+cnt]);
        }
        cnt += n;
    }
    return cnt + m;
}
