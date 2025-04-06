# LOGIN SERVER

This is a program designed to allow users to signup and confirm login authentication using cryptographically secure methods (salting and hashing passwords, using secure hash algorithms, etc).

## Modules

The program is broken up into several modules, seperated by header files.

### Main.hpp - The login header

This header includes all the functions for hashing and storing passwords, as well as verifying login information - giving a login token on success, or printing an error on failure. This module also keeps track of all login tokens (cookies) for users who are logged in, as well as an associated expiry timestamp 10 minutes from the initial login.

### Sockets.hpp - Network functions

This header attempts to create network functionality, allowing the program to act as an open node on the network which client agents can connect to. Currently still in development. Current thinking is to ditch this effort in the name of UNIX design principles, instead seeking to make this fully functioning in it's own right, and create an external means for connecting it to the internet (for instance, by importing it as a library to an existing web server).

## Compile and run
To avoid compilation errors, you will need to pass the `-lcrypto` and `-lssl` flags to `g++`. Full command then would be:

`g++ main.cpp login.cpp -lssl -lcrypto -o bin/main`
then run `bin/main`.

### What it means

After it has run, you will see a new file `users.txt` in the `data` folder, which is a tab seperated database of the users that were created in `main()`. It includes username, email, password hash, and password salt. You can see then that the password plaintext is not saved or managed by the database, but users are still able to login. The passwords are hashed using SHA256 with salt appended to the password.

# Developing with this library

