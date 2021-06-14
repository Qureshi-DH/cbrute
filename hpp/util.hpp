#include <string>
#include <vector>
#include <sstream>
#include <cstring>

#ifndef _PRJ_UTIL
#define _PRJ_UTIL

namespace Utility {

    // python string split implemnetion 
    std::vector<std::string>* split(const std::string& string, char split_char) {
        std::vector<std::string>* list = new std::vector<std::string>;
        unsigned prev = 0;
        for (unsigned i = 0;i < string.length(); ++i) {
            if (string[i] == split_char) {
                (*list).push_back(std::string(&string[prev], &string[i]));
                prev = i + 1;
            }
            else if (i == (string.length() - 1)) {
                (*list).push_back(std::string(&string[prev], &string[i + 1]));
            }
        }
        return list;
    }

    // standard progress logging function
    void log_progress(const int& rank, const int& length, const int& progress) {
        std::cout << "Rank: "
            << rank
            << " | Length to crack: "
            << length
            << " | Progress: "
            << (progress == -1 ? progress : progress)
            << "%" << std::endl;
    }

    // coverts anything to string using c++ basic stream
    template<class t>
    std::string to_string(const t& data) {
        std::stringstream data_stream;
        data_stream << data;
        return data_stream.str();
    }

    // -----------------------------------------------------------
    // -----------N E T W O R K   S E R I A L I Z E R S-----------
    // -----------------------------------------------------------

    template<class t>
    inline void write_data(std::stringstream& buffer, const t& data) {
        buffer.write((char*)&data, sizeof(t));
    }

    template<class t>
    inline void read_data(std::istringstream& buffer, t& data) {
        buffer.read((char*)&data, sizeof(t));
    }

    template<>
    inline void write_data(std::stringstream& buffer, const std::string& str) {
        std::size_t size = str.length();
        buffer.write((char*)&size, sizeof(std::size_t));
        buffer.write(&str[0], size);
    }

    template<>
    inline void read_data(std::istringstream& buffer, std::string& str) {
        std::size_t size;
        buffer.read((char*)&size, sizeof(std::size_t));
        str.resize(size);
        buffer.read(&str[0], size);
    }

}

#endif