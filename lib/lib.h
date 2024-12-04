#ifndef LIB_H
#define LIB_H

#include <chrono>
#include <string>
#include <fstream>
#include <functional>
#include <iostream>
#include <vector>

namespace lib
{
    void read_file(const char *who, const std::function<void(std::string, int)> &lambda);

    std::vector<std::string> split(const std::string s, const std::string delimiter);

    template <typename T, typename U>
    std::vector<U> map(const std::vector<T> &collection, U (*func)(T, int));

    class timer
    {
    private:
        std::chrono::time_point<std::chrono::high_resolution_clock> start_;

    public:
        void start();
        void end() const;
    };

}

#include "lib.tpp"

#endif
