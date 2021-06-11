#include <string>
#include <iostream>
#include <fstream>
#include <regex>
#include <sstream>

#include "./statics.hpp"
#include "./util.hpp"

#ifndef _PRJ_RECORD
#define _PRJ_RECORD

class Record {
public:
    std::string         name;
    std::string         password;
    std::string         salt;
    std::string         hash;
    std::string         salt_str;
    Statics::ENC_TYPES  enc_type;

    Record() : name(Statics::empty_string),
        password(Statics::empty_string),
        salt(Statics::empty_string),
        hash(Statics::empty_string),
        salt_str(Statics::empty_string),
        enc_type(Statics::ENC_TYPES::SHA512) {}

    bool read_shadow(const std::string& username) {
        std::ifstream shadow_file;
        shadow_file.open(Statics::shadow_file);

        if (!shadow_file.is_open()) {
            std::cout << "Sudo Access Ungranted. Terminating..." << std::endl;
            std::exit(0);
        }

        std::string current_line;
        while (std::getline(shadow_file, current_line)) {
            if (current_line.find(username) != std::string::npos) {
                auto tokens = Utility::split(current_line, Statics::shadow_split);
                auto hash_t = Utility::split((*tokens)[1], Statics::hash_t_split);

                this->name = (*tokens)[0];
                this->salt = (*hash_t)[2];
                this->hash = (*hash_t)[3];
                this->salt_str = "$6$" + this->salt + "$";
                this->enc_type = (Statics::ENC_TYPES)(std::atoi(((*hash_t)[1]).c_str()));

                shadow_file.close();
                return true;
            }
        }

        shadow_file.close();
        return false;
    }

    std::string serialize() {
        std::stringstream buffer_stream;

        Utility::write_data(buffer_stream, this->enc_type);
        Utility::write_data(buffer_stream, this->hash);
        Utility::write_data(buffer_stream, this->name);
        Utility::write_data(buffer_stream, this->salt);
        Utility::write_data(buffer_stream, this->salt_str);

        return buffer_stream.str();
    }

    void deserialize(const char buffer[]) {
        std::istringstream buffer_stream(std::string(buffer, Statics::buffer_size));

        Utility::read_data(buffer_stream, this->enc_type);
        Utility::read_data(buffer_stream, this->hash);
        Utility::read_data(buffer_stream, this->name);
        Utility::read_data(buffer_stream, this->salt);
        Utility::read_data(buffer_stream, this->salt_str);
    }
};

#endif