#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

#include "./statics.hpp"
#include "./util.hpp"

#ifndef _PRJ_NODE
#define _PRJ_NODE

class Node {
public:
    unsigned rank;
    unsigned length;
    unsigned progress;
    Statics::NODE_STATUS status;

    Node() : rank(0),
        length(0),
        progress(0),
        status(Statics::NODE_STATUS::UNDEFINED) {}

    void update_status(unsigned rank, unsigned progress, Statics::NODE_STATUS status) {
        this->rank = rank;
        this->progress = progress;
        this->status = status;
    }

    void log_status(bool lf_cr = false) {
        std::cout << "Rank: "
            << this->rank
            << " | Length to crack: "
            << this->length
            << " | Progress: "
            << this->progress
            << "% | Status Code: "
            << this->status
            << (lf_cr ? "\n" : "");
    }
};

#endif