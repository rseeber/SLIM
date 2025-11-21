/*
 * Project: Simple LogIn Manager (SLIM) Library
 * Author: River Seeber
 * Date: May 2025
 * Version: 1.1.0
 */

/*
 *  COPYRIGHT 2025 River Seeber.
 *
 *  This file is part of SLIM (Simple LogIn Manager).
 *
 *  SLIM is free software: you can redistribute it and/or modify it under the terms of the GNU Lesser General Public License as published by the Free Software Foundation, either version 3 of the License, or (at your option) any later version.
 *
 *  SLIM is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public License along with SLIM. If not, see <https://www.gnu.org/licenses/>. 
 */


// How many bytes of salt to append to the password plaintext before hashing
#define SALT_LEN 16

// How many bytes of hashtext to generate for each password
#define HASH_LEN 32

// How many seconds should a login token (cookie) be valid for before expiring?
#define COOKIE_EXPIRY_LEN_SECONDS 600   //10 minutes = 60 seconds * 10 = 600 seconds