# Simple LogIn Manager (SLIM)

This project is a C++ library which can be used to facilitate cryptographically secure management of login details from users. The library allows developers to easily create and maintain a dynamic database of users, storing their passwords as a salted SHA256 hash. It also provides full support for logging users in and handling temporary login tokens, such as cookies, which are common in web applications.

The management of these temporary login tokens is fully managed by the library, including the automatic revocation of the tokens after some arbitrary time interval. (still partially in development)

The login database (which contains usernames and password hashes) can be stored to a file on the disk, and then re-loaded into memory between server downtimes. The same can be done seperately for the login tokens, if desired.

# Example Usage

This project comes with an example file `main.cpp` to demonstrate how one can use the library. To compile this example, run `make build`. The resulting executable can be found in `bin/main`. If you run with the `-i` flag, it will allow you to create multiple logins, otherwise it will just create a default user "River". Either way, on termination it will save the login database to the file `users.txt` in `data/`. You will see that the password is securely stored as a SHA256 salted hash, and that if multiple users use the same password, their resulting hashes will all be unique due to the salt.

While the program is running, it keeps track of valid temporary login tokens (cookies). This collection is discarded after the program ends, forcing users to login again. Theoretically, a user could use this temporary token to validate their identity in place of their credentials during the duration of the token.

# Using this library in your projects

## Method 1: Importing the object files (reccomended)
The first way to use this library in your C programs is to import compiled object files. You can find these under the "Releases" page. The object file comes with an associated pgp signature. You can find my pgp key on [my website](https://rseeber.github.io/about.html). The key file can either be downloaded, or use the fingerprint to get it from a keyserver.

After you've verified the files, simply import them somewhere into your project somewhere suitable. You should be putting both the `login.o` AND the `login.hpp` file into the project. You will then place an include line in any programs which utilize the library, such as `#include "login.hpp"`.

Finally, when you compile your program, you will need to link your program's code with the library object file. To do this in g++, run `g++ <your source file(s)> login.o -lcrypto`. Note that it is also necessary that you link to the crypto library when compiling (`-lcrypto`), as this is the library used for password salt and hashing.


## Method 2: Compiling the library yourself
Although the object files are provided pre-compiled, it is possible for you to compile the object files yourself. To do so, aquire a copy of the `lib/` directory in this repo. From there, pass the `-c` flag to gcc to compile to object files: `g++ -c lib/login.cpp -o bin/login.o`. After that, use your resulting object file the same way you do in Method 1 to compile your project.

# Roadmap

The plan for this project is to make it be sufficiently hardened so that it can be used in confidence to handle logins for low- to medium-profile targets, such as simple web blogs, or other small applications which don't handle financial data or significant secrets. It also will need to be fully featured, having all necessary functionality implemented. Below is listed an unordered list of tasks need doing still.

- ~~Add logout function for revoking tokens early~~ (done)
- ~~Create randomized userIDs, allow users to edit username and email.~~ (done)
- ~~Make userID the central reference for users~~. Totally phase out usernames from backend usage.
- Clean up documentation
- Create man pages for interface functions
- Delete abandoned or deprecated functions
- ~~Remove non-interface functions from `login.hpp` (determine which can be classed as that)~~
- Expand example program to demonstrate how to use the login tokens to verify logins.
    + ~~Create functions for matching users to tokens and vice versa.~~ (done)
- Create additional options for hashing methods, usage of pepper, etc
- Create detailed documentation on how to use each interface function
    + Create general reccomendations on how programs should be written to avoid problems (ex: writing to the disk periodically, not just when terminating the program)
- Apply an open-source, copyleft license (LGPL?)