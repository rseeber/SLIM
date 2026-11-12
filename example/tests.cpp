/*
 * TODO: Need to add a test for every single function
 * that is inside of login.hpp
*/

#include "../login.hpp"
#include <math.h>

string randomUsername(int);
char randomVowel();
char randomConsonant();
char randomLetter(char[], int);

int main(int argc, char** argv){
    
    //initDB();
    
    string passwd = "password123";
    // Make 500 logins
    int login_cnt = pow(10, 3);
    printf("Creating %d logins...\n", login_cnt);
    for (size_t i = 0; i < login_cnt; i++)
    {
        if(i % 100 == 0){
            printf("Login %d\n", i);
        }
        string user = randomUsername(i);

        addUser(user, passwd);
    }
    
    printf("Done!\n");
    
    string names[] = {"alice", "bob", "charlie", "donna"};
    for(int i = 0; i < 4; ++i){
        addUser(names[i], passwd);
    }

    string user = "alice";
    login l;

    printf("Finding user %s\n\n", user.c_str());
    if(findUserByName(user, &l) < 0){
        printf("couldn't find user!\n");
    }
    else{
        printUser(l);
    }

    printf("Saving DB...\n");
    saveDB();
    printf("Done!\n");

    return 0;
}

string randomUsername(int num){
    // len is inside [5, 9]
    int len = 5 + (rand() % 5);
    bool vowel = false;
    char output[len+2];

    for(int i = 0; i < len; ++i){
        char c = randomConsonant();
        if(vowel){
            c = randomVowel();
        }
        output[i] = c;
        // toggle vowel
        vowel = !vowel;
    }
    output[len] = 0;
    string strout(output);
    return strout + to_string(num);
}

char randomVowel(){
    char arr[] = "aeiou";
    int len = 5;
    return randomLetter(arr, len);
}

char randomConsonant(){
    char arr[] = "bcdfghjklmnpqrstvwxyz";
    int len = 26 - 5;
    return randomLetter(arr, len);
}

char randomLetter(char arr[], int len){
    int r = rand() % len;
    return arr[r];

}