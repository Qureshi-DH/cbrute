// #include "../hpp/shadow.hpp"
#include <iostream>
#include <ctime>
#include <mpi.h>
#include <cstring>
#include <cstdio>
#include <cmath>

#define DEV

#include "../hpp/record.hpp"
#include "../hpp/permute.hpp"
#include "../hpp/statics.hpp"
#include "../hpp/node.hpp"

Node node;

int main(int argc, char* argv[], char* envp[]) {

    Record record;
    int my_rank, number_of_processes;

    unsigned long long start_time = time(NULL);

    MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &number_of_processes);
    MPI_Comm_rank(MPI_COMM_WORLD, &my_rank);

    node.update_status(my_rank, 0, Statics::NODE_STATUS::INITIALZED);

    if (!my_rank) {

        if (number_of_processes == 1) {
            std::cout << "Requires atleast 1 slave process. Terminating..." << std::endl;
            std::exit(0);
        }

        if (argc <= 1) {
            std::cout << "Missing Username. Terminating..." << std::endl;
            std::exit(0);
        }

        std::string username(argv[1]);
        if (!record.read_shadow(username)) {
            std::cout << "User Not Found. Terminating..." << std::endl;
            std::exit(0);
        }

        // Serialize and Send to Other Nodes
        std::string serialized = record.serialize();
        for (int i = 1; i < number_of_processes; ++i)
        {
            MPI_Send(serialized.c_str(), serialized.length(), MPI_BYTE, i, 1, MPI_COMM_WORLD);
        }

        // Send the pwd length to start bruteforce
        int i = 1,
            send_unit[2],
            length = Statics::min_password_length,
            distribution_unit = ceil((double)(Statics::max_password_length - Statics::min_password_length + 1) / (number_of_processes - 1));

        for (; i < number_of_processes && length <= Statics::max_password_length; ++i, length += distribution_unit)
        {
            send_unit[0] = length;
            send_unit[1] = length + distribution_unit - 1;

            // take care of one corner case when distribution unit causes the pwd_length to exceed max_pass_length
            send_unit[1] = ((send_unit[1] <= Statics::max_password_length) ? send_unit[1] : Statics::max_password_length);
            MPI_Send((void*)&send_unit, 2, MPI_INT, i, 2, MPI_COMM_WORLD);
        }

        while (i < number_of_processes) {
            send_unit[0] = send_unit[1] = 0;
            MPI_Send((void*)&send_unit, 2, MPI_INT, i++, 2, MPI_COMM_WORLD);
        }

    }


    if (my_rank) {

        // Receive Record Information
        char buffer[Statics::buffer_size];
        MPI_Recv(buffer, Statics::buffer_size, MPI_BYTE, 0, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

        record.deserialize(buffer);

        int recv_unit[2];
        MPI_Recv(&recv_unit, 2, MPI_INT, 0, 2, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
        if (recv_unit[0] && recv_unit[1]) {

            node.start_length = recv_unit[0];
            node.end_length = recv_unit[1];

            MPI_Request receive_request;
            MPI_Status receive_status;
            int flag;
            MPI_Irecv(nullptr, 0, MPI_CHAR, 0, 4, MPI_COMM_WORLD, &receive_request);

#pragma omp for 
            for (int pwd_length = recv_unit[0]; pwd_length <= recv_unit[1]; ++pwd_length)
                initiate_brute_force(Statics::alphabet, pwd_length, Statics::alphabet_size, record, receive_request, receive_status, flag, node);

            if (record.password != Statics::empty_string) {
                MPI_Send(record.password.c_str(), record.password.length(), MPI_CHAR, 0, 3, MPI_COMM_WORLD);
            }
            else {
                MPI_Send(nullptr, 0, MPI_CHAR, 0, 4, MPI_COMM_WORLD);
            }
        }


    }
    else {

        // Await Confirmation
        MPI_Status status;
        unsigned affirmations_received;
        char password[Statics::max_password_length + 1];

        while (status.MPI_TAG != 3 || affirmations_received < number_of_processes - 1) {
            MPI_Recv(password, Statics::max_password_length + 1, MPI_CHAR, MPI_ANY_SOURCE, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            ++affirmations_received;
        }

        for (int i = 1; i < number_of_processes; ++i)
        {
            if (i != status.MPI_SOURCE)
                MPI_Send(nullptr, 0, MPI_INT, i, 4, MPI_COMM_WORLD);
        }

        if (status.MPI_TAG == 3) {
            std::cout << "Password: "
                << password
                << " cracked in "
                << (time(NULL) - start_time)
                << " seconds."
                << std::endl;
        }
        else {
            std::cout << "Failed to crack password after "
                << (time(NULL) - start_time)
                << " seconds."
                << std::endl;
        }

    }

    MPI_Finalize();
    return 0;
}