// How many bytes of salt to append to the password plaintext before hashing
#define SALT_LEN 16

// How many bytes of hashtext to generate for each password
#define HASH_LEN 32

// How many seconds should a login token (cookie) be valid for before expiring?
#define COOKIE_EXPIRY_LEN_SECONDS 600   //10 minutes = 60 seconds * 10 = 600 seconds