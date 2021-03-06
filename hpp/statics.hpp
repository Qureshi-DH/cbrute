#include <string>

#ifndef _PRJ_STATICS
#define _PRJ_STATICS

namespace Statics {

    enum ENC_TYPES {
        MD5 = 1,
        BLOWFISH = 2,
        SHA256 = 5,
        SHA512 = 6
    };

    enum NODE_STATUS {
        UNDEFINED = 0,
        INITIALZED = 1,
        SEARCHING = 2,
        COMPLETED = 3,
        TERMINATED = 4
    };

#ifdef DEV
    constexpr unsigned char alphabet_size = 10;
    constexpr char* alphabet = (char*)"1234567890";
#elif defined(PRO)
    constexpr unsigned char alphabet_size = 36;
    constexpr char* alphabet = (char*)"1234567890abcdefghijklmnopqrstuvwxyz";
#else
    constexpr unsigned char alphabet_size = 26;
    constexpr char* alphabet = (char*)"abcdefghijklmnopqrstuvwxyz";
#endif

    constexpr int buffer_size = 1024;

    constexpr unsigned char min_password_length = 1;
    constexpr unsigned char max_password_length = 8;

    constexpr char shadow_split = ':';
    constexpr char hash_t_split = '$';

    constexpr char* empty_string = (char*)"";
    constexpr char* shadow_file = (char*)"/etc/shadow";

}
#endif