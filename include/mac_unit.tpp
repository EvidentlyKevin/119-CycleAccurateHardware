// mac_unit.tpp

template<typename T>
void MACUnit<T>::cycle(Systolic_Array<T>& systolic_array) {
    // Reset clk if it exceeds the max state
    if (clk > MAX_CLK_STATE) {
        clk = 0;
    }

    switch (clk) {
        case 0:
            if (!fetchInputs(systolic_array)) {
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
            sendOutputs();
            clk = 0; // Reset for next operation
            break;
        default:
            clk = 0;
            break;
    }
}

template<typename T>
bool MACUnit<T>::fetchInputs(Systolic_Array<T>& systolic_array) {
    // Fetch activation 'a'
    if (rowID == 0) {
        if (!inputA.pop(a)) {
            return false; // Activation not ready
        }
    } else {
        if (!systolic_array(rowID - 1, colID)->downOut.pop(a)) {
            return false; // Activation not ready
        }
    }

    // Fetch partial sum 'b'
    if (colID == 0) {
        b = 0; // First column has no incoming partial sum
    } else {
        if (!systolic_array(rowID, colID - 1)->rightOut.pop(b)) {
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
void MACUnit<T>::sendOutputs() {
    // Send partial sum to the right
    rightOut.push(accumulator);

    // Send activation downwards
    downOut.push(a);
}
