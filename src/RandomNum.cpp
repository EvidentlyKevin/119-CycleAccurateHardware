#include <iostream>
#include <fstream>
#include <random>
#include <string>
#include <sstream>
#include <vector>
#include <direct.h> // For _mkdir on Windows

// Generates a vector file with random integers
void generate_vector_file(const std::string& directory, int file_number, int num_batches, int samples_per_batch, int vector_size) {
    // Concatenate the directory path and filename
    std::string filename = directory + "\\0801008n" + std::to_string(file_number);

    // Create the output file
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error creating file: " << filename << std::endl;
        return;
    }

    // Create a random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(-127, 127);

    for (int batch = 0; batch < num_batches; ++batch) {
        for (int sample = 0; sample < samples_per_batch; ++sample) {
            std::vector<int> vector(vector_size);
            for (int& num : vector) {
                num = dist(gen); // Fill vector with random numbers
            }

            // Convert vector to a comma-separated string
            std::ostringstream oss;
            for (size_t i = 0; i < vector.size(); ++i) {
                if (i > 0) oss << ',';
                oss << vector[i];
            }

            file << oss.str() << '\n'; // Write the vector to the file
        }

        if (batch < num_batches - 1) {
            file << "#\n"; // Add batch separator
        }
    }
}

// Generates multiple vector files
void generate_multiple_files(const std::string& directory, int num_files, int num_batches, int samples_per_batch, int vector_size) {
    // Check if the directory exists, and if not, create it
    if (_mkdir(directory.c_str()) == -1) {
        if (errno != EEXIST) {
            std::cerr << "Error creating directory: " << directory << std::endl;
            return;
        }
    }

    for (int i = 0; i < num_files; ++i) {
        generate_vector_file(directory, i, num_batches, samples_per_batch, vector_size);
        std::cout << "Generated file: " << directory << "\\0801008n" << i << std::endl;
    }
}

void preview_file(const std::string& filename, std::size_t preview_size = 500) {
    std::ifstream file(filename);
    if (!file) {
        std::cerr << "Error opening file: " << filename << std::endl;
        return;
    }

    std::string content;
    content.assign(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());

    if (content.size() > preview_size) {
        content = content.substr(0, preview_size) + "...";
    }
    std::cout << content << std::endl;
}

int main() {
    std::string directory = "C:\\Users\\Henry\\C++Code\\RandomNumOutput(inputA)";  // Specify the directory where files should be saved
    int num_files = 5;
    int num_batches = 8;
    int samples_per_batch = 1;
    int vector_size = 8;

    generate_multiple_files(directory, num_files, num_batches, samples_per_batch, vector_size);

    std::cout << "\nAll files have been generated. Here's a preview of the first file:\n";
    preview_file(directory + "\\0801008n0");

    return 0;
}
