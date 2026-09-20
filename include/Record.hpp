#ifndef RECORD_HPP
#define RECORD_HPP

#include <string>
#include <iostream>

struct Record {
    int id;
    std::string key;
    double value;

    Record(int r_id, std::string r_key, double r_val)
        : id(r_id), key(std::move(r_key)), value(r_val) {}

    void print() const {
        std::cout << "[Record] ID: " << id 
                  << " | Key: " << key 
                  << " | Value: " << value << std::endl;
    }
};

#endif // RECORD_HPP