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

    // base case --- one permutation generated
    if (k == 0)
    {
        // generate and compare hash with hash from shadow
        std::string hashed = crypt(prefix.c_str(), record.salt_str.c_str());
        if (hashed == (record.salt_str + record.hash)) {
            record.password = prefix;
        }
        return;
    }

    // append all letters from the 'alphabet' sequentially
    for (int i = 0; i < alphabet_size; i++)
    {
        // listen to 'found' signal to terminate search
        MPI_Test(&receive_request, &flag, &receive_status);
        if (record.password != Statics::empty_string || flag) {
            return;
        }

        std::string new_prefix;
        new_prefix = prefix + alphabet[i];

        if (new_prefix.length() == 1) {
            int progress = progress = (int)(((float)(i + 1) / alphabet_size) * 100);
            Utility::log_progress(rank, k + 1, progress);
        }

        // k count decreased after adding new character
        permute(alphabet, new_prefix, k - 1, alphabet_size, record, receive_request, receive_status, flag, rank);
    }

}

void initiate_brute_force(
    char alphabet[],
    int length,
    int alphabet_size,
    Record& record,
    MPI_Request& receive_request,
    MPI_Status& receive_status,
    int& flag,
    int& rank)
{
    permute(alphabet, std::string(""), length, alphabet_size, record, receive_request, receive_status, flag, rank);
}