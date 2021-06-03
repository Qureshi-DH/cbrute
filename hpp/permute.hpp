#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <crypt.h>

#include "record.hpp"

void printAllKLengthRec(char set[], std::string prefix, int n, int k, Record& record)
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
    // from set and recursively
    // call for k equals to k-1
    for (int i = 0; i < n; i++)
    {

        std::string newPrefix;

        // Next character of input added
        newPrefix = prefix + set[i];

        // k is decreased, because
        // we have added a new character
        printAllKLengthRec(set, newPrefix, n, k - 1, record);


        if (record.password != Statics::empty_string) {
            break;
        }
    }

}

void printAllKLength(char set[], int k, int n, Record& record)
{
    printAllKLengthRec(set, "", n, k, record);
}