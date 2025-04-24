// File include/mac_unit.tpp
#include  "../include/activation.h"
#include "../include/mac_unit.h"

template<typename T>
MACUnit<T>::MACUnit(int row, int col)
    : rowID(row), colID(col), accumulator(0), a(0), b(0), w(0), clk(0),
      rightOut(CHANNEL_CAPACITY), downOut(CHANNEL_CAPACITY), inputA(CHANNEL_CAPACITY) {}


// Set input channels
template<typename T>
void MACUnit<T>::setUpIn(channelM<T>* upChannel) {
    upIn = upChannel;
}
template<typename T>
void MACUnit<T>::setLeftIn(channelM<T>* leftChannel) {
    leftIn = leftChannel;
}
// Get output channels
template<typename T>
channelM<T>& MACUnit<T>::getRightOut() {
    return rightOut;
}
template<typename T>
channelM<T>& MACUnit<T>::getDownOut() {
    return downOut;
}

template<typename T>
void MACUnit<T>::setWeight(T weight) {
    w = weight;
   // std::cout << "MAC[" << rowID << "][" << colID << "] weight set to " << w << std::endl; //debugging
}

template<typename T>
void MACUnit<T>::setInputActivation(T activation) {
    if (rowID == 0) {
        inputA.push(activation);
    }
}

template<typename T>
void MACUnit<T>::cycle(int act) {
    // Reset clk if it exceeds the max state
    if (clk > MAX_CLK_STATE) {
        clk = 0;
    }

    switch (clk) {
        case 0:
            if (!fetchInputs()) {
                // Inputs not ready, wait
                return;
            }
            clk++;
            break;
        case 1:
            computeMAC();
            clk++;
            break;
        case 2:
            sendOutputs(act);
            clk = 0; // Reset for next operation
            break;
        default:
            clk = 0;
            break;
    }
}

// Modify the fetchInputs method to use the input channels
template<typename T>
bool MACUnit<T>::fetchInputs() {
    // Fetch activation 'a'
    if (rowID == 0) {
        if (!inputA.pop(a)) {
            return false; // Activation not ready
        }
    } else {
        if (!upIn || !upIn->pop(a)) {
            return false; // Activation not ready
        }
    }

    // Fetch partial sum 'b'
    if (colID == 0) {
        b = 0; // No incoming partial sum
    } else {
        if (!leftIn || !leftIn->pop(b)) { //remove !leftIn before ||
            return false; // Partial sum not ready
        }
    }
    return true; // Inputs fetched successfully
}

template<typename T>
void MACUnit<T>::computeMAC() {
    accumulator = (a * w) + b;
}

template<typename T>
void MACUnit<T>::sendOutputs(int act) {
    Activation activation;
    T result = accumulator; // Result is the accumulator for now
    // Apply activation
    //Case statement to determine the activation function
    switch (act) {
        case 0:
            rightOut.push(static_cast<T>(activation.relu(accumulator)));
            break;
        case 1:
            rightOut.push(static_cast<T>(activation.sigmoid(accumulator)));
            break;
        case 2:
            rightOut.push(static_cast<T>(activation.tanh(accumulator)));
            break;
        case 3:
            rightOut.push(static_cast<T>(activation.gelu(accumulator)));
            break;
        default:
           // rightOut.push(accumulator);
            break;

    }
    

    // Send result to the right
    rightOut.push(result);
    if (false) {
        std::cout << "MAC[" << rowID << "][" << colID << "] sent result " << result << " to rightOut" << std::endl;
    }

    // Send activation downward
    downOut.push(a);
    // Debugging: Print the activation sent downward
    if (false) {
        std::cout << "MAC[" << rowID << "][" << colID << "] sent activation " << a << " to downOut\n";
    }
}


template<typename T>
T MACUnit<T>::readAccumulator() const {
    return accumulator;
}

template<typename T>
T MACUnit<T>::getLastActivation() const {
    return a;
}