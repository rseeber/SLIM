# LOGIN SERVER

This is a program designed to allow users to signup and confirm login authentication using cryptographically secure methods (salting and hashing passwords, using secure hash algorithms, etc).

## login.hpp and login.cpp - The login header

This header includes all the functions for hashing and storing passwords, as well as verifying login information - giving a login token on success, or printing an error on failure. This module also keeps track of all login tokens (cookies) for users who are logged in, as well as an associated expiry timestamp 10 minutes from the initial login.

Following standard conventions, login.hpp includes function prototypes, and the login.cpp function should be compiled together with your program in order to use it's function implimentations.

# Purpose
The point of this project is to serve as a library which can be dropped into any project that needs secure login capabilities. The project is a prototype of using the library to accomplish that end. Thus, developers should only import the `lib/` folder into their project. My `main.cpp` file is meant only to demonstrate how and why to use the library.

# Compile and run
To compile using my prototype `main.cpp` project, simply call `g++ main.cpp lib/login.cpp -lssl -lcrypto -o bin/main`, then run `bin/main` to execute.

# Developing with this library
If you're a developer, simply copy `lib/` into your project, and use the same compiler command, but substitute your own source code in place of my `main.cpp` file.


# Under the hood
After you have run my `main.cpp` program, you will see a new file `users.txt` in the `data/` folder, which is a tab seperated database of the users that were created. It includes username, email, password hash, and password salt. You can see then that the password plaintext is not saved or managed by the database, but users are still able to login. The passwords are hashed using SHA256 with salt appended to the password.

There's also currently a functional backend which manages login tokens (cookies) that expire after a set period of time. I'm still working on creating a library interface for this functionality. The database of tokens is not ever saved to the disk, as that could cause potential security flaws if cookies were allowed to stay in-tact between reboots.