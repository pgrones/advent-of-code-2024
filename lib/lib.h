#ifndef LIB_H
#define LIB_H

#include <chrono>
#include <fstream>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

namespace lib {

/* Reads a file in the same folder based on `who` and applies `lambda` to each line */
void read_file(const char *who, const std::function<void(std::string, int)> &lambda);

/* Splits a string on each occurrence of `delimiter`. If the `delimiter` is `""` then a vector of single chars is returned */
std::vector<std::string> split(const std::string s, const std::string delimiter);

/* Converts a char to string */
std::string ctos(const char c);

/* Prints an array to the console */
template <typename T, int size>
void print_collection(const std::array<T, size> &collection);

/* Prints a vector to the console */
template <typename T>
void print_collection(const std::vector<T> &collection);

/* Calls `func` on each element of the vector and returns the transformed vector */
template <typename T, typename U>
std::vector<U> map(const std::vector<T> &collection, U (*func)(T, int));

/* Encapsulates a timer  */
class timer {
   private:
    std::chrono::time_point<std::chrono::high_resolution_clock> start_;

   public:
    /* Start the timer */
    void start();
    /* Stop the timer and print the elapsed time */
    void stop() const;
};

}  // namespace lib

#include "lib.tpp"

#endif
