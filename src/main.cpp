// File: src/main.cpp

#include "../include/cache_lru.h"
#include "../include/cache_fifo.h"
#include "../include/load_store_unit.h"
#include "../include/perf_counter.h"
#include "../include/globals.h"
#include <iostream>
#include <deque>
#include <array>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <filesystem>
#include "../include/channel.h"
#include "../include/channelM.h"
#include "../include/systolic_array.h"
#include "../include/mac_unit.h"

using namespace std;    

int main(int argc, char* argv[]) {


    /*Systolic_Array<double> systolic_array;
    systolic_array.Construct("C:\\Users\\Henry\\C++Code\\119-CycleAccurateHardware\\weight\\0801008n1");
    systolic_array.ConstructA("C:\\Users\\Henry\\C++Code\\119-CycleAccurateHardware\\0801008n1");

    int max_cycle = 256, cycle_count = 0, array_size = 8;



for (int i = 0; i < array_size; i++){
    systolic_array[0][i]->inputA.channel_push(systolic_array.A[0][i]);
}



while (cycle_count < max_cycle){
    for (int i = array_size-1; i > -1; i--) { 
        for (int j = array_size-1; j > -1; j--) {
            cout << "Clock: " << systolic_array[i][j]->clk << endl;
            cout << "" << endl;
            cout << "Performing MAC operation at position: " << i << ", " << j << endl;
            cout << "operand B: " << systolic_array[i][j]->b << endl;
            cout << "operand A: " << systolic_array[i][j]->a << endl;
            cout << "Weight: " << systolic_array[i][j]->w << endl;
                        cout << "" << endl;
            systolic_array[i][j]->cycle(systolic_array);
                        cout << "Accumulated value: " << systolic_array[i][j]->read_accumulator() << endl;
                                    cout << "" << endl;


        }
    }
cycle_count++;

}*/

    // Check for correct number of arguments
    if (argc != 6 && argc != 7) { // Allow 5 or 6 arguments + program name
        std::cerr << "Usage: " << argv[0] << " <cache_policy> <storage_file> <requests_file> <num_cycles> <verbose> [<output_file>]\n";
        std::cerr << "cache_policy: lru or fifo\n";
        std::cerr << "verbose: 1 for verbose, 0 for silent\n";
        std::cerr << "[<output_file>]: (Optional) Name of the output file. Defaults to 'outfile.txt' if not provided.\n";
        return 1;
    }

    // Parse command-line arguments
    std::string cache_policy = argv[1];
    std::string storage_file = argv[2];
    std::string requests_file = argv[3];
    int num_cycles;
    int verbose;

   try {
        num_cycles = std::stoi(argv[4]);
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: <num_cycles> must be an integer.\n";
        return 1;
    }

    try {
        verbose = std::stoi(argv[5]);
        if (verbose < 0 || verbose > 2) {
            std::cerr << "Error: <verbose> must be 0 (silent), 1 (verbose), or 2 (very verbose).\n";
            return 1;
        }
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: <verbose> must be an integer (0, 1, or 2).\n";
        return 1;
    }

    // Determine output file name
    std::string output_file;
    if (argc == 7) {
        output_file = argv[6];
    } else {
        output_file = "outfile.txt";
    }

    // Create the output directory if it doesn't exist
    std::filesystem::create_directory("../output");

    // Open the output file for redirection
    std::ofstream out_stream("../output/" + output_file);
    if (!out_stream.is_open()) {
        std::cerr << "Error: Could not open output file: " << output_file << "\n";
        return 1;
    }

    // Redirect std::cout and std::cerr to the output file
    std::streambuf* cout_buf = std::cout.rdbuf(); // Save original buffer
    std::streambuf* cerr_buf = std::cerr.rdbuf(); // Save original buffer
    std::cout.rdbuf(out_stream.rdbuf());
    std::cerr.rdbuf(out_stream.rdbuf());

    // Initialize performance counter
    perf_counter counter;

    // Create cache based on policy
    Cache* cache = nullptr;
    if (cache_policy == "lru") {
        cache = new cache_lru(4, 1024, 4, &counter, verbose); // block_size=4, cache_size=1024 bytes, n_ways=4
    } else if (cache_policy == "fifo") {
        cache = new cache_fifo(4, 1024, 4, &counter, verbose); // block_size=4, cache_size=1024 bytes, n_ways=4
    } else {
        std::cerr << "Invalid cache policy. Use 'lru' or 'fifo'.\n";
        // Restore original buffers before exiting
        std::cout.rdbuf(cout_buf);
        std::cerr.rdbuf(cerr_buf);
        return 1;
    }

    // Initialize cache
    cache->init_(storage_file);

    // Create Load Store Unit
    LoadStoreUnit lsu(8, verbose); // ls_queue_size=8, pass verbosity
    lsu.init_(requests_file);

    
    // Simulation loop
    for (int cycle = 0; cycle < num_cycles; ++cycle) {
        // Cache simulation cycle
        cache->cycle();
        lsu.cycle();

       

        // Optionally, synchronize or handle dependencies between cache and systolic array here
            return 0;

    }

    // Print performance summary
    counter.print_summary();

    // Clean up
    delete cache;

    // Restore original buffers
    std::cout.rdbuf(cout_buf);
    std::cerr.rdbuf(cerr_buf);

    // Close the output file
    out_stream.close();

    return 0;
}
