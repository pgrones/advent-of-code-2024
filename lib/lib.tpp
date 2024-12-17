#include "lib.h"

using std::cout;
using std::vector;

template <typename T, typename U>
vector<U> lib::map(const vector<T> &collection, U (*func)(T, int)) {
    vector<U> result;
    result.reserve(collection.size());

    for (size_t i = 0; i < collection.size(); i++) {
        result.push_back(func(collection[i], i));
    }

    return result;
}

template <typename T, size_t size>
void lib::print_collection(const T (&collection)[size]) {
    for (auto item : collection) {
        cout << item << ", ";
    }

    cout << "\b\b \n";  // remove the last comma
}

template <typename T>
void lib::print_collection(const std::vector<T> &collection) {
    for (auto item : collection) {
        cout << item << ", ";
    }

    cout << "\b\b \n";  // remove the last comma
}