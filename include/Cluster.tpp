
#include "Cluster.h"


template<typename T>
Cluster<T>::Cluster(int size) : SIZE(size) {
    array.resize(SIZE);
    for (int i = 0; i < 1; ++i) {
        array[i].reserve(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            array[i].push_back(std::make_unique<TPU<T>>(i, j));
        }
    }

    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (j > 0) {
                array[i][j]->setLeftPort(&array[i][j - 1]->getRPort());
            }
        }
    }
}

template<typename T>
void Cluster<T>::setParametersForTPUs() {
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "set parameters for TPU[" << i << "][" << j << "]" << endl;
            array[i][j]->setparameters();
        }
    }
}

template<typename T>
void Cluster<T>::runTPUS() {
    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "output for TPU[" << i << "][" << j << "]" << endl;
            array[i][j]->run();
        }
    }
}




/*template<typename T>
void Cluster<T>::showbanks() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < 1; ++j) {
            std::cout << "show banks for TPU[" << i << "][" << j << "]" << endl;
            array[i][j]->display();
        }
    }
}*/




template<typename T>
void Cluster<T>::setActivationFunction(int activation) {
    activationFunction = activation;
}

template<typename T>
int Cluster<T>::getActivationFunction() const {
    return activationFunction;
}



// This sends row 0 to TPU[0][0] and row 1 to TPU[0][1] and so on
template<typename T>
void Cluster<T>::sendDataToTPUs(const std::vector<T>& data) {
    int idx = 0; // index for TPU
    // Do this intelligently instead of sending the same data to all TPUs

    for (int i = 0; i < 1; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            if (idx < data.size()) { // Check if index is within bounds
                array[i][j]->TPU<T>::sendData(data[idx]);// Send data to TPU
                ++idx;
             }
             else{
                break; // Break if index exceeds data size

             } 
                
            }
            
        }
    }


template<typename T>
int Cluster<T>::getSize() const {
    return SIZE;
}

template<typename T>
void Cluster<T>::setTPUData(int i, int j, const std::vector<T>& data) {
    if (i < SIZE && j < SIZE) {
        array[i][j]->TPU<T>::sendData(data);  // sendData must exist in TPU<T>
    } else {
        std::cerr << " setTPUData: Index out of bounds!\n";
    }
}
