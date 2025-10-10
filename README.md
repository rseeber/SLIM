# Simple LogIn Manager (SLIM)

This project is a C++ library which can be used to facilitate cryptographically secure management of login details from users. The library allows developers to easily create and maintain a dynamic database of users, storing their passwords as a salted SHA256 hash. It also provides full support for logging users in and handling temporary login tokens, such as cookies, which are common in web applications.

The management of these temporary login tokens is fully managed by the library, including the automatic revocation of the tokens after some arbitrary time interval.

The login database (which contains usernames and password hashes) can be stored to a file on the disk, and then re-loaded into memory between server downtimes. The same can be done seperately for the login tokens, if desired.

# Major Contributions

The main point of the SLIM library is to streamline the process of a developer working on any system which requires a simple login system. The hope is that this library will provide a simple enough interface, that they will use this library instead of simply storing all logins in a plaintext, insecure database. The goal was to create the system in a modular way that followed as closely as possible with the principles of the UNIX Design Philosophy of modularity and simplicity.

Listed below are some of the major functions of the SLIM library, and which demonstrate the ways that this project contributes to the existing work in the field.

## Interface Functions

- `initDB()`: loads the database of passwords into memory. All future read and write operations on the database will be done to the version in memory. Changes will only be synced back to the disk when calling `saveDB()`.
- `addUser()`: creates a new user in the database. This function will automatically handle salting and hashing of the password by using the SHA256 hash function, and the output will be stored in the database along with its salt. The plaintext password will of course not be saved to the database.
- `loginAsUser()`: will check if the provided username and password are correct. If the password matches the provided user, it will return a 1, otherwise it returns 0. Furthermore, when it returns a 1, the provided pointer `*cook` will be assigned with a cookie associated with that user, which can later be used for performing actions as the user without providing the password again.
- `editPasswd()` allows you to edit the password stored on the database for the user. Simply enter the new password as a parameter, and it will do all the hashing and salting for you.
- `saveDB()`: saves the database of passwords back down to the disk, overwriting any previous versions. This function should be called at program termination, as well as intermitently during extended runtime in order to prevent data loss if the program terminates abruptly.

## Internal Functions

- `hashPasswd_generateSalt()` and `hashPasswd()`: which generates 32 bytes of cryptographically random noise to use as salt, then hashing the provided password with the random salt in the SHA256 function provided by the OpenSSL library. It then converts that hash into an ASCII representation of the hash, which can then be stored in the text database. These functions are called by both `addUser()` and `loginAsUser()` when handling the password.
- `findUserByName()` which iterates through the database and puts the `user` object corresponding to the provided username at the location pointed to by `*l`, if one exists. This function is called within both `editPasswd()` and `loginAsUser()` in order to find the correct user.

# Example Usage

This project comes with an example file `main.cpp` to demonstrate how one can use the library. To compile this example, run `make build`. The resulting executable can be found in `bin/main`. If you run with the `-i` flag, it will allow you to create multiple logins, otherwise it will just create a default user "River". Either way, on termination it will save the login database to the file `users.txt` in `data/`. You will see that the password is securely stored as a SHA256 salted hash, and that if multiple users use the same password, their resulting hashes will all be unique due to the salt.

While the program is running, it keeps track of valid temporary login tokens (cookies). If you pass in the `-l` (as in "login") flag instead of `-i`, you can handle active logins. In this mode, it will load up all the login cookies from `data/cookies.txt`. You can log a new user in by selecting that option and entering the credentials. Then, the program will add that user to the cookie database. Finally, it will save the cookies back to the `cookies.txt` file, to be loaded again next time it's called. Users can be logged out early by selecting the "logout" option instead of "log in" at runtime.

# Using this library in your projects

## Method 1: Importing the object files (reccomended)
The first way to use this library in your C programs is to import compiled object files. You can find these under the "Releases" page. The object file comes with an associated pgp signature. You can find my pgp key on [my website](https://riverseeber.net/about). The key file can either be downloaded, or use the fingerprint to get it from a keyserver.

After you've verified the files, simply import them somewhere suitable into your project. You should be putting both the `login.o` AND the `login.hpp` file into the project. You will then place an include line in any programs which utilize the library, like so `#include "login.hpp"`.

Finally, when you compile your program, you will need to link your program's code with the library object file. To do this in g++, run `g++ <your source file(s)> login.o -lcrypto`. Note that it is also necessary that you link to the crypto library when compiling (`-lcrypto`), as this is the library used for password salt and hashing.


## Method 2: Compiling the library yourself
Although the object files are provided pre-compiled, it is possible for you to compile the object files yourself. To do so, aquire a copy of all the source code files for this library. From there, pass the `-c` flag to gcc to compile the object file: `g++ -c login.cpp -o bin/login.o`. After that, use your resulting object file the same way you do in Method 1 to compile your project.

# Roadmap

The plan for this project is to make it be sufficiently hardened so that it can be used in confidence to handle logins for low- to medium-profile targets, such as simple web blogs, or other small applications which don't handle financial data or significant secrets. It also will need to be fully featured, having all necessary functionality implemented. Below is listed an unordered list of tasks need doing still.

- ~~Add logout function for revoking tokens early~~ (done)
- ~~Create randomized userIDs, allow users to edit username and email.~~ (done)
- ~~Make userID the central reference for users. Totally phase out usernames from backend usage.~~ (done)
- Clean up documentation
- Create man pages for interface functions
- Delete abandoned or deprecated functions
- ~~Remove non-interface functions from `login.hpp` (determine which can be classed as that)~~ (done)
- ~~Expand example program to demonstrate how to use the login tokens to verify logins.~~ (done)
    + ~~Create functions for matching users to tokens and vice versa.~~ (done)
- Create additional options for hashing methods, usage of pepper, etc
- Create detailed documentation on how to use each interface function
    + Create general reccomendations on how programs should be written to avoid problems (ex: writing to the disk periodically, not just when terminating the program)
- ~~Apply an open-source, copyleft license (LGPL?)~~ (done)
