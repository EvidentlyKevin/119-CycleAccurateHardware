// File: include/config_manager.h

#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>

struct Config {
    // Cache Settings
    std::string cache_policy;      // "lru" or "fifo"
    size_t block_size;             // Size in bytes (power of two)
    size_t cache_size;             // Total cache size in bytes
    int n_ways;                    // Associativity
    int miss_penalty;             // Miss penalty in cycles

    // Write Policy
    std::string write_policy;      // "write_back" or "write_through"

    // Simulation Settings
    std::string storage_file;      // Path to storage file
    std::string requests_file;     // Path to requests file
    int num_cycles;                // Number of simulation cycles
    int verbose;                   // Verbosity level (0: Silent, 1: Verbose, 2: Very Verbose)
    std::string output_file;       // Path to output log file

    // Constructor to initialize default values
    Config()
        : cache_policy("lru"),
          block_size(16),
          cache_size(1024),
          n_ways(4),
          miss_penalty(4),
          write_policy("write_back"),
          storage_file("input/storage.txt"),
          requests_file("input/requests.txt"),
          num_cycles(100),
          verbose(1),
          output_file("output_log.txt") {}
};

class ConfigManager {
public:
    ConfigManager();
    bool parse(int argc, char* argv[]);  // Overloaded to handle command-line arguments
    Config getConfig() const;

private:
    Config config;
    void setDefaults();      // Sets default configuration parameters
    bool validateConfig();   // Validates the configuration parameters
    bool parseArguments(int argc, char* argv[]); // Parses command-line arguments
};

#endif // CONFIG_MANAGER_H
