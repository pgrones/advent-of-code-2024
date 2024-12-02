#ifndef LIB_H
#define LIB_H

#include <chrono>
#include <string>
#include <fstream>
#include <functional>
#include <iostream>

void read_file(const char *who, const std::function<void(std::string, int)> &lambda);

class timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;

public:
    void start();
    void end() const;
};

#endif
