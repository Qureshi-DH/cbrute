#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

#include "record.hpp"

void permute(char alphabet[], std::string prefix, int k, int n, Record& record)
{

    // Base case: k is 0,
    // print prefix
    if (k == 0)
    {
        std::string hashed = crypt(prefix.c_str(), record.salt_str.c_str());
        if (hashed == (record.salt_str + record.hash)) {
            record.password = prefix;
        }
        return;
    }

    // One by one add all characters
    // from alphabet and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++)
    {

        std::string new_prefix;

        // Next character of input added
        new_prefix = prefix + alphabet[i];

        // k is decreased, because
        // we have added a new character
        permute(alphabet, new_prefix, k - 1, n, record);


        if (record.password != Statics::empty_string) {
            break;
        }
    }

}

void initiate_brute_force(char alphabet[], int length, int alphabet_size, Record& record)
{
    permute(alphabet, std::string(""), length, alphabet_size, record);
}