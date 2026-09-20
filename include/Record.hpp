#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>

struct Record {
    int id;
    std::string key;
    double value;

    Record(int r_id, std::string r_key, double r_val)
        : id(r_id), key(std::move(r_key)), value(r_val) {}
};

#endif // RECORD_HPP