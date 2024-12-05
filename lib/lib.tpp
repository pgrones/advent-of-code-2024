#include "lib.h"

using std::vector;
using std::cout;

template <typename T, typename U>
vector<U> lib::map(const vector<T> &collection, U (*func)(T, int)) {
    vector<U> result;
    result.reserve(collection.size());

    for (size_t i = 0; i < collection.size(); i++) {
        result.push_back(func(collection[i], i));
    }

    return result;
}

template <typename T, int size>
void lib::print_collection(const std::array<T, size> &collection){
    for (auto item : collection){
        cout << item << ", ";
    }
    
    cout << "\n";
}

template <typename T>
void lib::print_collection(const std::vector<T> &collection){
    for (auto item : collection){
        cout << item << ", ";
    }

    cout << "\n";
}