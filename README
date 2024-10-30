# LOGIN SERVER

This is a program designed to allow users to signup and confirm login authentication using cryptographically secure methods (salting and hashing passwords, using secure hash algorithms, etc).

## Modules

The program is broken up into several modules, seperated by header files.

### Main.hpp - The login header

This header includes all the functions for hashing and storing passwords, as well as verifying login information - giving a login token on success, or printing an error on failure. This module also keeps track of all login tokens (cookies) for users who are logged in, as well as an associated expiry timestamp 10 minutes from the initial login.

### Sockets.hpp - Network functions

This header attempts to create network functionality, allowing the program to act as an open node on the network which client agents can connect to. Currently still in development.