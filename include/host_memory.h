#include "globals.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <array>
#include <vector>
#include <string>

class hostMemory {
public:
    void init();  
    void getData(double,int,int);  
    void saveToFile(const std::string& filename,int cycle);  

private:
    std::array<std::array<double, 16>, 16> memory{};  // 16 rows, default 64 cols
    size_t cols = 16;  // Adjustable column size
};
