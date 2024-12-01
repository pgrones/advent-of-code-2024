#ifndef LIB_H
#define LIB_H

#include <chrono>
#include <string>
#include <iostream>
#include <fstream>
#include <functional>

inline void read_file(const char* who, const std::function<void(std::string, int)>& lambda)
{
    const std::string file_name = std::string("input-") + who + std::string(".txt");
    std::ifstream input("day01/" + file_name);

    if (input.is_open())
    {
        int i = 0;
        std::string line;

        while (getline(input, line))
        {
            lambda(line, i++);
        }

        input.close();
    }
    else
    {
        throw std::runtime_error("Unable to open file");
    }
}

class timer
{
    std::chrono::time_point<std::chrono::steady_clock> start_;

public:
    void start()
    {
        start_ = std::chrono::high_resolution_clock::now();
    }

    void end() const
    {
        const std::chrono::duration<double> elapsed = std::chrono::high_resolution_clock::now() - start_;
        std::cout << "Elapsed time: " << elapsed.count() << " s\n";
    }
};

#endif
