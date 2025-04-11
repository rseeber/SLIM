# Login Server

This project is a source code C library which can be used to facilitate cryptographically secure management of login details from users. The library allows developers to easily create and maintain a dynamic database of users, storing their passwords as a salted SHA256 hash. It also provides full support for logging users in and handling temporary login tokens, such as cookies, which are common in web applications.

The management of these temporary login tokens is fully managed by the library, including the automatic revocation of the tokens after some arbitrary time interval. (still partially in development)

The login database (which contains usernames and password hashes) can be stored to a file on the disk, and then re-loaded into memory between server downtimes. The login tokens cannot be saved in such manner, due to security concerns.

# Example Usage

This project comes with an example file `main.cpp` to demonstrate how one can use the library. To compile this example, run `g++ main.cpp lib/login.cpp -lssl -lcrypto -o bin/main`, then run the resulting executable `bin/main`. If you run with the `-i` flag, it will allow you to create multiple logins, otherwise it will just create a default user "River". Either way, on termination it will save the login database to the file `users.txt` in `data/`. You will see that the password is securely stored as a SHA256 salted hash, and that if multiple users use the same password, their resulting hashes will all be unique due to the salt.

While the program is running, it keeps track of valid temporary login tokens (cookies). This collection is discarded after the program ends, forcing users to login again. Theoretically, a user could use this temporary token to validate their identity in place of their credentials during the duration of the token.

# Using this library in your projects

To use this library, simply copy the `lib/` folder and it's contents into your project. You are free then to use any of the functions defined in `lib/login.hpp`. To compile, simply use `g++ <source files> lib/login.cpp -lssl -lcrypto`, where `<source files>` is a list of all source files you wish to link together in the program that uses this library (oftentimes something like `main.cpp`). 

# Roadmap

The plan for this project is to make it be sufficiently hardened so that it can be used in confidence to handle logins for low- to medium-profile targets, such as simple web blogs, or other small applications which don't handle financial data or significant secrets. It also will need to be fully featured, having all necessary functionality implemented. Below is listed an unordered list of tasks need doing still.

- ~~Add logout function for revoking tokens early~~ (done)
- Create randomized userIDs, allow users to edit username and email (password changes are already implemented).
- Clean up documentation
- Delete abandoned or deprecated functions
- Remove non-interface functions from `login.hpp` (determine which can be classed as that)
- Expand example program to demonstrate how to use the login tokens to verify logins.
    + ~~Create functions for matching users to tokens and vice versa.~~ (done)
- Create additional options for hashing methods, usage of pepper, etc
- Create detailed documentation on how to use each interface function
    + Create general reccomendations on how programs should be written to avoid problems (ex: writing to the disk periodically, not just when terminating the program)