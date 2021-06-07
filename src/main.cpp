// #include "../hpp/shadow.hpp"
#include <iostream>
#include <ctime>

#define DEV

#include "../hpp/record.hpp"
#include "../hpp/permute.hpp"


int main(int argc, char* argv[], char* envp[]) {

    const unsigned long long start_time = time(NULL);

    if (argc <= 1) {
        std::cout << "Missing Username. Terminating..." << std::endl;
        return 0;
    }

    int length = 4;
    Record record;
    std::string username(argv[1]);

    if (!record.read_shadow(username)) {
        std::cout << "User Not Found. Terminating..." << std::endl;
        return 0;
    }

    initiate_brute_force(Statics::alphabet, length, Statics::alphabet_size, record);

    if (record.password != Statics::empty_string) {
        std::cout << "Password: "
            << record.password
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
    return 0;
}