// File: lib/config_manager.cpp

#include "config_manager.h"
#include <iostream>
#include <cmath>
#include <cstring>

// Constructor
ConfigManager::ConfigManager() {
    setDefaults();
}

// Sets default configuration parameters
void ConfigManager::setDefaults() {
    config.cache_policy = "lru";
    config.block_size = 4;
    config.cache_size = 1024;
    config.n_ways = 4;
    config.miss_penalty = 4;
    config.write_policy = "write_back";
    config.storage_file = "input/storage.txt";
    config.requests_file = "input/requests.txt";
    config.num_cycles = 40;
    config.verbose = 1;
    config.output_file = "output_log.txt";
}

bool ConfigManager::validateConfig() {
    // Assert that block_size is a power of two
    if (config.block_size == 0 || (config.block_size & (config.block_size - 1)) != 0) {
        std::cerr << "Error: block_size must be a power of two.\n";
        return false;
    }

    // Assert that cache_size is divisible by (block_size * n_ways)
    if (config.cache_size == 0 || (config.cache_size % (config.block_size * config.n_ways)) != 0) {
        std::cerr << "Error: cache_size must be divisible by (block_size * n_ways).\n";
        return false;
    }

    // Ensure write_policy is valid
    if (config.write_policy != "write_back" && config.write_policy != "write_through") {
        std::cerr << "Error: write_policy must be 'write_back' or 'write_through'.\n";
        return false;
    }

    // Ensure cache_policy is valid
    if (config.cache_policy != "lru" && config.cache_policy != "fifo") {
        std::cerr << "Error: cache_policy must be 'lru' or 'fifo'.\n";
        return false;
    }

    // Validate verbosity level
    if (config.verbose < 0 || config.verbose > 2) {
        std::cerr << "Error: verbose must be 0, 1, or 2.\n";
        return false;
    }

    return true;
}

// Parses command-line arguments to override defaults
bool ConfigManager::parseArguments(int argc, char* argv[]) {
    for (int i = 1; i < argc; ++i) {
        if (strcmp(argv[i], "--cache_policy") == 0 && i + 1 < argc) {
            config.cache_policy = argv[++i];
        }
        else if (strcmp(argv[i], "--block_size") == 0 && i + 1 < argc) {
            config.block_size = std::stoul(argv[++i]);
        }
        else if (strcmp(argv[i], "--cache_size") == 0 && i + 1 < argc) {
            config.cache_size = std::stoul(argv[++i]);
        }
        else if (strcmp(argv[i], "--n_ways") == 0 && i + 1 < argc) {
            config.n_ways = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--miss_penalty") == 0 && i + 1 < argc) {
            config.miss_penalty = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--write_policy") == 0 && i + 1 < argc) {
            config.write_policy = argv[++i];
        }
        else if (strcmp(argv[i], "--storage_file") == 0 && i + 1 < argc) {
            config.storage_file = argv[++i];
        }
        else if (strcmp(argv[i], "--requests_file") == 0 && i + 1 < argc) {
            config.requests_file = argv[++i];
        }
        else if (strcmp(argv[i], "--num_cycles") == 0 && i + 1 < argc) {
            config.num_cycles = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--verbose") == 0 && i + 1 < argc) {
            config.verbose = std::stoi(argv[++i]);
        }
        else if (strcmp(argv[i], "--output_file") == 0 && i + 1 < argc) {
            config.output_file = argv[++i];
        }
        else if (strcmp(argv[i], "--help") == 0) {
            std::cout << "Usage: cache_simulator [options]\n"
                      << "Options:\n"
                      << "  --cache_policy [lru|fifo]\n"
                      << "  --block_size [size in bytes]\n"
                      << "  --cache_size [size in bytes]\n"
                      << "  --n_ways [associativity]\n"
                      << "  --miss_penalty [cycles]\n"
                      //<< "  --write_policy [write_back|write_through]\n"
                      << "  --storage_file [path]\n"
                      << "  --requests_file [path]\n"
                      << "  --num_cycles [number]\n"
                      << "  --verbose [0|1|2]\n"
                      << "  --output_file [path]\n"
                      << "  --help\n";
            return false; // Exit program after printing help
        }
        else {
            std::cerr << "Unknown or incomplete argument: " << argv[i] << "\n";
            return false;
        }
    }

    return true;
}

// Initializes the config with default or modified values
bool ConfigManager::parse(int argc, char* argv[]) {
    // Set defaults
    setDefaults();

    // Parse command-line arguments to override defaults
    if (!parseArguments(argc, argv)) {
        return false;
    }

    // Validate the current configuration
    if (!validateConfig()) {
        return false;
    }

    return true;
}

// Returns the current configuration
Config ConfigManager::getConfig() const {
    return config;
}
