#include "Cluster.h"

template<typename T>
Cluster<T>::Cluster(int size) : SIZE(size) {
    array.resize(SIZE);
    for (int i = 0; i < SIZE; ++i) {
        array[i].resize(SIZE);
        for (int j = 0; j < SIZE; ++j) {
            array[i][j] = std::make_unique<TPU<T>>(i, j);
        }
    }
}

template<typename T>
void Cluster<T>::setParametersForTPUs() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "set parameters for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->setparameters();
        }
    }
}

template<typename T>
void Cluster<T>::runAllTPUs() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "output for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->run(persistentMem);  // Pass persistentMem
        }
    }
}

template<typename T>
void Cluster<T>::showbanks() {
    for (int i = 0; i < SIZE; ++i) {
        for (int j = 0; j < SIZE; ++j) {
            std::cout << "show banks for TPU[" << i << "][" << j << "]" << std::endl;
            array[i][j]->display(persistentMem);  // Pass persistentMem
        }
    }
}

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
void Cluster<T>::sendDataToTPUs(const std::vector<std::vector<T>>& data) {
    for (int i = 0; i < SIZE && i < data.size(); ++i) {
        if (data[i].size() < SIZE) {
            std::cerr << "Error: Row " << i << " in data has fewer columns than expected.\n";
            continue;
        }
        for (int j = 0; j < SIZE; ++j) {
            array[i][j]->sendData(data[i], persistentMem);  // Pass persistentMem
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
        array[i][j]->sendData(data, persistentMem);  // Pass the entire vector
    } else {
        std::cerr << "setTPUData: Index out of bounds!\n";
    }
}

template<typename T>
void Cluster<T>::loadFromJson(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }

    json j;
    file >> j;
    file.close();

    if (!j.is_array()) {
        throw std::runtime_error("Error: JSON data is not an array.");
    }

    persistentMem.resize(j.size()); // Resize to match the JSON data size

    int idx = 0;
    for (const auto& row : j) {
        if (idx >= persistentMem.size()) break;
        auto values = row.get<std::vector<T>>();
        if (values.size() != SIZE) { // Ensure row size matches expected size
            throw std::runtime_error("Error: Row size mismatch in JSON data.");
        }
        persistentMem[idx] = values;
        idx++;
    }
}