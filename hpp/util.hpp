#include <string>
#include <vector>

#ifndef _PRJ_UTIL
#define _PRJ_UTIL

namespace Utility {

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
}

#endif