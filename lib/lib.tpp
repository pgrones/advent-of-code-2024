#include "lib.h"

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