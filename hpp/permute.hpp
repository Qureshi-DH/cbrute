#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <unistd.h>
#include <crypt.h>
#include <mpi.h>

#include "record.hpp"
#include "node.hpp"

void permute(char alphabet[], std::string prefix, int k, int alphabet_size, Record& record, MPI_Request& receive_request,
    MPI_Status& receive_status,
    int& flag, int& rank)
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
    for (int i = 0; i < alphabet_size; i++)
    {

        MPI_Test(&receive_request, &flag, &receive_status);
        if (record.password != Statics::empty_string || flag) {
            return;
        }

        std::string new_prefix;

        // Next character of input added
        new_prefix = prefix + alphabet[i];

        if (new_prefix.length() == 1) {
            int progress = progress = (int)(((float)(i + 1) / alphabet_size) * 100);
            Utility::log_progress(rank, k + 1, progress);
        }


        // k is decreased, because
        // we have added a new character
        permute(alphabet, new_prefix, k - 1, alphabet_size, record, receive_request, receive_status, flag, rank);



    }

}

void initiate_brute_force(char alphabet[], int length, int alphabet_size, Record& record, MPI_Request& receive_request,
    MPI_Status& receive_status,
    int& flag,
    int& rank)
{
    permute(alphabet, std::string(""), length, alphabet_size, record, receive_request, receive_status, flag, rank);
}