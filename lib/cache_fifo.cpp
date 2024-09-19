#include "cache_fifo.h"
#include "request_structs.h"
#include "channel.h"
#include <iostream>
#include <fstream>
#include <sstream>

cache_fifo::cache_fifo(size_t block_size, size_t cache_size, int n_ways)
    : stall_rem(0) {
    this->block_size = block_size;
    this->cache_size = cache_size;
    this->n_ways = n_ways;
}

void cache_fifo::init_(const std::string& input_fname) {
    int n_sets = cache_size / (n_ways * block_size);

    block_offset_bits = get_bits(block_size*4);
    index_bits = get_bits(n_sets);
    tag_bits = 32 - (block_offset_bits + index_bits);

    set_index_mask = (1 << index_bits) - 1;
    
    for (int i = 0; i < n_sets; i++){
        free_lines_map[i] = n_ways;
        set_fifo_map[i] = queue<int>();
    }

    stall_rem = 0;

    ifstream input_file(input_fname);

    string line;
    while(getline(input_file, line)) {
        istringstream iss(line);
        int first, second;
        if (iss >> first >> second){
            storage_map[first] = second;
        }
    }

    input_file.close();

    // std::cout << stall_rem << endl;
    
    // std::cout << storage_map.size() << endl;
    // std::cout << data_queue.size() << endl;
}

void cache_fifo::cycle() {
        if (read_request_channel.channel_empty()) {
            std::cout << "Cache starting up..." << std::endl;
            return;
        }

        if (stall_rem > 0){
            stall_rem--;
            std::cout << "Memory stalled for " << stall_rem << " more cycles..." << std::endl;
            return;
        }

        read_request req1 = read_request_channel.channel_pop();

        if (cache_map.find(req1.address) != cache_map.end()){ // hit
            cout << "HIT! @ " << req1.address << " Val: "<< cache_map[req1.address] << endl;
            read_ack ack1 = {true, req1.address, req1.id};
            read_ack_channel.channel_push(ack1);
            perf_vector[0]++;
            perf_vector[2]++;
        }
        else{ // miss
            int set_index = (req1.address >> block_offset_bits) & set_index_mask;
            std::cout << "MISS @ " << req1.address << " Set Index: " << set_index << std::endl;
            stall_rem += miss_penalty;

            // line eviction
            if (free_lines_map[set_index] == 0){
                int words_evicted = 0;
                int addr = set_fifo_map[set_index].front();
                while (words_evicted < block_size){
                    cache_map.erase(addr);
                    addr += 4;
                    words_evicted++;
                }
                set_fifo_map[set_index].pop();
                free_lines_map[set_index]++;
            }

            if (free_lines_map[set_index] > 0){
                int words_fetched = 0;
                int addr = req1.address;
                while (words_fetched < block_size){
                    cache_map[addr] = storage_map[addr];
                    addr += 4;
                    words_fetched++;
                }
                free_lines_map[set_index]--;
                set_fifo_map[set_index].push(addr);
            }
            

            read_ack ack1 = {false, req1.address, req1.id};
            read_ack_channel.channel_push(ack1);

            perf_vector[0]++;
            perf_vector[1]++;
        }

        // cout << stall_rem << endl;    
}
