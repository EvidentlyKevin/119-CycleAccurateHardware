#include <iostream>

template<typename T>
class MACUnit {
public:
    
   MACUnit() : accumulator(0){}

   void cycle(T a, T b){
    accumulator += a*b;
   }

T read_accumulator() const{
    return accumulator;
}

void clear_accumulator(){
    accumulator = 0;
}

private:
    T accumulator;  // Stores the accumulated result
};

int main() {
    MACUnit<double> mac_unit;

    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0
    mac_unit.cycle(1.5, 2.0);  // 1.5 * 2.0 = 3.0
    mac_unit.cycle(2.5, 4.0);  // 2.5 * 4.0 = 10.0

    std::cout << "Accumulated value: " << mac_unit.read_accumulator() << std::endl;

    return 0;
}
