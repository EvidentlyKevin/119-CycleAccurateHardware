#ifndef SYSTOLIC_ARRAY_H
#define SYSTOLIC_ARRAY_H

#include <array>

class Systolic_Array {
public:
    std::array<std::array<int, 8>, 8> array; // Example dimensions, adjust as needed

    int read(int i, int j) const {
        return array[i][j];
    }
};

#endif
