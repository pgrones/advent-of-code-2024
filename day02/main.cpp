#include <bits/stdc++.h>

#include <iostream>
#include <sstream>
#include <string>

#include "../lib/lib.h"

using namespace std;

const int M = 8;

void copyArray(int a[], int b[], const int n, const int skip = -1) {
    int a_i = 0;
    int b_i = 0;

    while (a_i < n) {
        if (a_i == skip) {
            a_i++;
            continue;
        }

        b[b_i] = a[a_i];
        a_i++;
        b_i++;
    }
}

int isSafe(int levels[]) {
    bool isIncreasing;

    for (int j = 0; j < (M - 1); j++) {
        int first = levels[j];
        int second = levels[j + 1];

        if (second <= 0)
            return -1;

        if (j == 0) {
            isIncreasing = first < second;
        }

        if (!isIncreasing) {
            first = -first;
            second = -second;
        }

        if (!(second > first && second - first <= 3)) {
            return j;
        }
    }

    return -1;
}

int main(int argc, char *argv[]) {
    lib::timer timer;
    const int N = 1000;

    timer.start();

    int reports[N][M] = {0};

    const char *delimiter = " ";
    lib::read_file(argv[1], [&reports, &delimiter](const string &line, const int i) -> void {
        istringstream stream(line);
        string token = "";

        int j = 0;
        while (getline(stream, token, *delimiter)) {
            reports[i][j++] = stoi(token);
        } });

    int safeReports = 0;
    for (int i = 0; i < N; i++) {
        safeReports += isSafe(reports[i]) == -1;
    }

    timer.stop();

    std::cout << "Result: " << safeReports << "\n";

    // PART II
    timer.start();

    safeReports = 0;
    for (int i = 0; i < N; i++) {
        int levels[M] = {0};
        copyArray(reports[i], levels, M);

        int res = isSafe(levels);

        if (res == -1) {
            safeReports++;
            continue;
        }

        int buf[M] = {0};
        copyArray(reports[i], buf, M, res);
        if (isSafe(buf) == -1) {
            safeReports++;
            continue;
        }

        buf[M] = {0};
        copyArray(reports[i], buf, M, res + 1);
        if (isSafe(buf) == -1) {
            safeReports++;
            continue;
        }

        if (res == 1) {
            buf[M] = {0};
            copyArray(reports[i], buf, M, res - 1);
            if (isSafe(buf) == -1) {
                safeReports++;
                continue;
            }
        }
    }

    timer.stop();

    std::cout << "Result: " << safeReports;

    return 0;
}