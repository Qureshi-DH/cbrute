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
    unsigned start_length;
    unsigned end_length;
    unsigned progress;
    Statics::NODE_STATUS status;

    Node() : rank(0),
        start_length(0),
        end_length(0),
        progress(0),
        status(Statics::NODE_STATUS::UNDEFINED) {}

    void update_status(unsigned rank, unsigned progress, Statics::NODE_STATUS status) {
        this->rank = rank;
        this->progress = progress;
        this->status = status;
    }

    void log_status(bool lf_cr = false) {
        bool diff_length = (start_length != end_length);
        std::cout << "Rank: "
            << this->rank
            << " | Length to crack: "
            << this->start_length
            << (diff_length ? " - " : "")
            << (diff_length ? Utility::to_string(this->end_length) : "")
            << " | Progress: "
            << this->progress
            << "% | Status Code: "
            << this->status
            << (lf_cr ? "\n" : "");
    }
};

#endif