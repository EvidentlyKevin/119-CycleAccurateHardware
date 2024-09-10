#include <iostream>
#include <fstream>
#include <sstream>
#include <deque>
#include <queue>
#include <cmath>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

template<typename T>
class channel{
public:
    
    channel(size_t capacity): capacity(capacity), is_full(is_full){} // constructor to initialize the channel with a given capacity

    // function to push an element into the channel
    void channel_push(T& element){

        // check if the buffer is full
        if (buffer.size() >= capacity){
            // buffer is full, do not add the element
            // cout << "Buffer full..." << endl;
            return;
        }

        // add the element to the buffer
        buffer.push_back(element);

        // update the is_full flag
        if (buffer.size() == capacity){
            is_full = true;
        }
        else{
            is_full = false;
        }
        return;
    }

    // pop an element from the channel
    T channel_pop(){
        if (buffer.size() == 0){ // check if the buffer is empty
            // cout << "Buffer empty..." << endl;
            return T(); // buffer is empty, return default value of T
        }
        
        T ret_val = buffer.front(); // get the front element of the buffer
        buffer.pop_front(); // remove the front element from the buffer
        is_full = false; // buffer is not full after popping an element
        return ret_val;
    }

    // check if the channel is full
    bool channel_full(){
        return is_full;
    }

    // check if the channel is empty
    bool channel_empty(){
        return buffer.empty();
    }

    // get the current size of the channel
    size_t channel_size(){
        return buffer.size(); 
    }

private:
    deque<T> buffer; // store the elements in the channel
    size_t capacity; // maximum capacity of the channel
    bool is_full; // indicate if the channel is full
};

struct read_request{
    int32_t address; // address to read from
    int32_t id; // unique identifier for the read request
};

struct read_ack{
    bool read_success; // indicates if the read was successful
    int32_t address; // address that was read from
    int32_t data; // data read from the address
};

// create memory channels
// bandwidth (B) = buffer_size*2*4 ( = buffer_size*number of ints in struct * 4B for each word)
int buffer_size = 16;
channel<read_request> read_request_channel(buffer_size), read_sink_channel(buffer_size); 
channel<read_ack> read_ack_channel(buffer_size), sink_ack_channel(buffer_size);

// performance counters
// 0: number of instructions/memory ops/requests processed
// 1: number of misses
// 2: number of hits
// 3: number of stall cycles
vector<int> perf_vector(4, 0); 

//////////////////////////////

class load_store_unit{
public:
    // constructor initializes the load/store unit with a given queue size
    load_store_unit(size_t ls_queue_size): ls_queue_size(ls_queue_size){}

    // initialize load requests from file
    void init_(string input_fname){
        ifstream input_file(input_fname); // open input file

        string line;
        while(getline(input_file, line)){ // read lines from the file
            istringstream iss(line);
            int first;
            if (iss >> first){ // extract element from line
                request_queue.push(first); // save
            }
        }
        input_file.close();
    }

    // cycle behavior
    int cycle(){

        // check if requests available
        if (request_queue.empty()){
            return -1;     
        }

        
        if (load_queue.size() >= ls_queue_size){
            // cout << "Load queue full!" << endl;
        }
        else{ // add request to load queue
            load_queue.push(request_queue.front());
            request_queue.pop();
        }

        // offload read acknowledgment signals (future support for processor)
        if (!read_ack_channel.channel_empty()){
            read_ack_channel.channel_pop();
        }
        
        // stalled
        if (read_request_channel.channel_full()){
            // cout << "can't push to channel" << endl;
            return 1;
        }

        
        read_request req1 = {load_queue.front(), 0}; // create new request from the front of the load queue
        read_request_channel.channel_push(req1); // push read request to the channel
        
        // cout << "Buffer is not full..." << endl;
        load_queue.pop(); // this request is going to the cache, LSU doesn't need to keep it
        // cout << "Load popped" << endl;

        return 0;
    }

private:
    size_t ls_queue_size; // maximum size of the load/store queue
    queue<int> load_queue, request_queue; // queues for load operations and all memory access operations
    // queue<pair<int, int>> store_queue, data_queue; // (future support)
};

///////////////////////////////////////////////

class cache{
public:
    const int miss_penalty = 4; // penalty for a cache miss
    
    // constructor, size parameters are in words
    cache(size_t block_size, size_t cache_size, int n_ways) : block_size(block_size), cache_size(cache_size), n_ways(n_ways), stall_rem(stall_rem){}

    int block_offset_bits, index_bits, tag_bits, set_index_mask, tag_mask;

    // initializes the cache configuration and storage map from a file
    void init_(string input_fname){

        int n_sets = cache_size/(n_ways*block_size); // calculate number of sets

        block_offset_bits = get_bits(block_size*4); // block offset bits
        index_bits = get_bits(n_sets); // index bits
        tag_bits = 32 - (block_offset_bits + index_bits); // tag bits

        set_index_mask = (1 << index_bits) - 1; // set index mask

        // track number of free lines in each set
        // needed for line eviction and replacement in SA caches
        for (int i = 0; i < n_sets; i++){
            free_lines_map[i] = n_ways; // n-way set associativity
            set_fifo_map[i] = queue<int>(); // initialize FIFO map for each set
        }

        stall_rem = 0;

        // open the storage input file and populate the storage map with key-value pairs
        ifstream input_file(input_fname);
        string line;
        while(getline(input_file, line)){
            istringstream iss(line);
            int first, second;
            if (iss >> first >> second){
                storage_map[first] = second;
            }
        }

        input_file.close();
    }

    // return the number of bits needed to represent the given value
    int get_bits(int val){
        return log2(val);
    }

    // check if the cache is full
    bool is_full(){
        return cache_map.size() >= cache_size;
    }

    // extract the set index from the given address
    int get_set_index(int addr){
        return (addr >> block_offset_bits) & set_index_mask;
    }

    // extract the tag from the given address
    int get_tag(int addr){
        return addr >> (block_offset_bits + index_bits);
    }

    // moves the specified address to the back of the LRU queue for its set
    void move_to_back_lru(int addr){
        int set_index = get_set_index(addr); // get the set index of the address
        int addr_tag = get_tag(addr); // get the tag of the address
        // cout << "LRU" << endl;
        // cout << addr_tag << endl;

        queue<int> tmp_queue; // (future support) optimize using deque
        // process the FIFO queue for the set for maintaing LRU order
        while (!set_fifo_map[set_index].empty()){
            int current_addr = set_fifo_map[set_index].front();
            int current_addr_tag = get_tag(current_addr);
            set_fifo_map[set_index].pop();

            // all addresses except the address being accessed (hit or miss)
            if (current_addr_tag != addr_tag){
                tmp_queue.push(current_addr);
            }
        }

        // the address being accessed is the MOST recently used
        // hence it is at the end of the queue
        // at the front of the queue si the address LEASE recently used
        tmp_queue.push(addr);
        set_fifo_map[set_index] = tmp_queue;
        // cout << set_fifo_map[set_index].size() << endl;
    }

    // behavior during a single cycle
    void cycle(){
        if (read_request_channel.channel_empty()) { // check if there are read requests
            cout << "Cache starting up..." << endl;
            return;
        }

        if (stall_rem > 0){ // check memory stalls
            stall_rem--;
            // cout << "Memory stalled for " << stall_rem << " more cycles..." << endl;
            return;
        }

        read_request req1 = read_request_channel.channel_pop(); // get read request from bus

        if (cache_map.find(req1.address) != cache_map.end()){ // hit
            // cout << "HIT! @ " << req1.address << " Val: "<< cache_map[req1.address] << endl;
            read_ack ack1 = {true, req1.address, cache_map[req1.address]}; // send read acknowledgement (future support)
            read_ack_channel.channel_push(ack1);
            perf_vector[0]++; // increment total accesses
            perf_vector[2]++; // increment hit count
            move_to_back_lru(req1.address); // update LRU
        }
        else{ // miss
            int set_index = get_set_index(req1.address);
            // cout << "MISS @ " << req1.address << " Set Index: " << set_index << endl;
            stall_rem += miss_penalty; // add miss penalty to stall count
            perf_vector[3] += miss_penalty; // update number of cycles stalled due to memory

            // comment this line out if you want only hits to count as "use" of a cache line
            move_to_back_lru(req1.address); // update LRU

            // line eviction from set
            if (free_lines_map[set_index] == 0){
                int words_evicted = 0;
                int addr = set_fifo_map[set_index].front(); // least recently used line from set
                while (words_evicted < block_size){ // all words in the cache line
                    cache_map.erase(addr); // evict the address from the cache
                    addr += 4;
                    words_evicted++;
                }
                set_fifo_map[set_index].pop(); // remove the LRU line from queue
                free_lines_map[set_index]++; // one more line is now available in the set
            }

            // fetch line from lower levels of memory hierarchy
            // and place in cache set if free lines are available in set
            if (free_lines_map[set_index] > 0){
                int words_fetched = 0;
                int addr = req1.address;
                while (words_fetched < block_size){
                    cache_map[addr] = storage_map[addr]; // fetch data from storage
                    addr += 4;
                    words_fetched++;
                }
                free_lines_map[set_index]--; // one less line is now available in the set
                set_fifo_map[set_index].push(addr); // most recently used line (new)
            }
            

            read_ack ack1 = {false, req1.address, req1.id};
            read_ack_channel.channel_push(ack1);

            perf_vector[0]++; // increment total accesses
            perf_vector[1]++; // increment miss count
        }
    }

private:
    size_t block_size, cache_size;
    int n_ways;
    int stall_rem = 0;
    queue<pair<int, int>> data_queue;
    unordered_map<int, int> storage_map, cache_map, free_lines_map;
    // unordered_map<int, bool> cache_valid_map; // valid bits (future support)
    unordered_map<int, queue<int>> set_fifo_map;
};

int main(int argc, char* argv[]){

    // argument parsing, processing and checking
    if (argc == 2 && (string(argv[1]) == "-h" || string(argv[1]) == "--help")){
        cout << "The input desgin and testing parameters are as follows: \n\n";
        cout << "Cache size: \t\t -c, --cache_size (design parameter)\n";
        cout << "Block size: \t\t -b, --block_size (design parameter)\n";
        cout << "Set associativity: \t -a, --n_ways (design parameter)\n";
        cout << "LSU queue sizes: \t -q, --ls_queue_size (design parameter)\n";
        cout << "Path to storage file: \t -s, --storage_file_path (testing parameter)\n";
        cout << "Path to requests file: \t -r, --request_file_path (testing parameter)\n";
        cout << "\n*** All design parameters should be powers of 2 ***" << endl;
        return 1;
    }
    else if (argc != 13){
        cout << "Please pass all the parameters..." << endl;
        return -1;
    }

    int cache_size, block_size, n_ways, ls_queue_size;
    string storage_file_path, request_file_path;

    for (int i = 1; i < argc; i++){
        if (string(argv[i]) == "-a" || string(argv[i]) == "--n_ways"){
            cout << "Set associativity: " << argv[i+1] << endl;
            n_ways = stoi(argv[i+1]);
            i++;
        }
        else if (string(argv[i]) == "-b" || string(argv[i]) == "--block_size"){
            cout << "Block size: " << argv[i+1] << endl;
            block_size = stoi(argv[i+1]);
            i++;
        }
        else if (string(argv[i]) == "-c" || string(argv[i]) == "--cache_size"){
            cout << "Cache size: " << argv[i+1] << endl;
            cache_size = stoi(argv[i+1]);
            i++;
        }
        else if (string(argv[i]) == "-q" || string(argv[i]) == "--ls_queue_size"){
            cout << "Load/store queue size: " << argv[i+1] << endl;
            ls_queue_size = stoi(argv[i+1]);
            i++;
        }
        else if (string(argv[i]) == "-s" || string(argv[i]) == "--storage_file_path"){
            cout << "Storage file: " << argv[i+1] << endl;
            storage_file_path = argv[i+1];
            i++;
        }
        else if (string(argv[i]) == "-r" || string(argv[i]) == "--request_file_path"){
            cout << "Request file: " << argv[i+1] << endl;
            request_file_path = argv[i+1];
            i++;
        }
        else{
            cout << "Unknown argument.. Exit" << endl;
            return -1;
        }
    }

    cout << "\nStarting simulation..." << endl;
    
    // initialize cache
    cache l1(block_size, cache_size, n_ways);
    l1.init_("tests/"+storage_file_path);
    cout << "Storage initialized... " << endl;

    // initialize LSU
    load_store_unit lsu1(ls_queue_size);
    lsu1.init_("tests/"+request_file_path);
    cout << "Requests initialized... " << endl;

    cout << "Starting cycles...\n" << endl;

    int n_cycles = 16384;
    int count = 1;

    while (n_cycles >= 0){
    //    cout << "Cycle#: " << count << endl;
       l1.cycle();
       lsu1.cycle();
       n_cycles--;
       count++;
    }
    cout << "Simulation finished: " << count-1 << " cycles simulated..." << endl;
    cout << "\n\n\n" << "Summary\n#########" << endl;
    cout << "Memory accesses: " << perf_vector[0] << endl;
    cout << "Misses: " << perf_vector[1] << endl;
    cout << "Hits: " << perf_vector[2] << endl;
    cout << "Hit rate: " << (float)perf_vector[2]/(float)perf_vector[0] << endl;
    cout << "Memory stall cycles: " << perf_vector[3] << endl;
    cout << "Stall cycle%: " << 100*((float)perf_vector[3]/(float)(count-1)) << endl;
    cout << "CPI: " << (float)count/(float)perf_vector[0] << endl;
    
    return 0;
}