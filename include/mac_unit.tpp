// File include/mac_unit.tpp

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
}

template<typename T>
void MACUnit<T>::setInputActivation(T activation) {
    if (rowID == 0) {
        inputA.push(activation);
    }
}

template<typename T>
void MACUnit<T>::cycle() {
    // Reset clk if it exceeds the max state
    if (clk > MAX_CLK_STATE) {
        clk = 0;
    }

    switch (clk) {
        case 0:
            if (!fetchInputs(true)) {
                // Inputs not ready, wait
                return;
            }
            clk++;
            break;
        case 1:
            computeMAC(true);
            clk++;
            break;
        case 2:
            sendOutputs(true);
            clk = 0; // Reset for next operation
            break;
        default:
            clk = 0;
            break;
    }
}

// Modify the fetchInputs method to use the input channels
template<typename T>
bool MACUnit<T>::fetchInputs(bool debug) {
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
        if (!leftIn || !leftIn->pop(b)) {
            return false; // Partial sum not ready
        }
    }

    // Debugging: Print the fetched inputs
    if (false) {
        std::cout << "MAC[" << rowID << "][" << colID 
        << "] fetchInputs: a=" << a << ", b=" << b << "\n";
    }
    return true; // Inputs fetched successfully
}

template<typename T>
void MACUnit<T>::computeMAC(bool debug) {
    accumulator = (a * w) + b;

    // Debugging: Print the MAC operation
    if (false) {
        std::cout << "MAC[" << rowID << "][" << colID << "] computeMAC: a=" << a
                  << ", w=" << w << ", b=" << b << ", accumulator=" << accumulator << "\n";
    }
}

template<typename T>
void MACUnit<T>::sendOutputs(bool debug) {
    // Send partial sum to the right
    rightOut.push(accumulator);
    // Debugging: Print the partial sum sent to the right
    if (false) {
        std::cout << "MAC[" << rowID << "][" << colID << "] sent accumulator " << accumulator << " to rightOut\n";
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
