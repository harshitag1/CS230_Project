#ifndef CACHE_H
#define CACHE_H

#include "memory_class.h"
#define LIRS_SIZE LLC_WAY*LLC_SET*2

// PAGE
extern uint32_t PAGE_TABLE_LATENCY, SWAP_LATENCY;


#define INC_POLICY 3
#define INCLUSIVE 1
#define EXCLUSIVE 2
#define NON_INCLUSIVE 3

// BEST OFFSET CONSTANTS
#define NUM_OFFSETS 46
#define ROUND_MAX 100
#define SCORE_MAX 31
#define BAD_SCORE 10
#define DELAY 0

// CACHE TYPE
#define IS_ITLB 0
#define IS_DTLB 1
#define IS_STLB 2
#define IS_L1I  3
#define IS_L1D  4
#define IS_L2C  5
#define IS_LLC  6

// INSTRUCTION TLB
#define ITLB_SET 16
#define ITLB_WAY 4
#define ITLB_RQ_SIZE 16
#define ITLB_WQ_SIZE 16
#define ITLB_PQ_SIZE 0
#define ITLB_MSHR_SIZE 8
#define ITLB_LATENCY 1

// DATA TLB
#define DTLB_SET 16
#define DTLB_WAY 4
#define DTLB_RQ_SIZE 16
#define DTLB_WQ_SIZE 16
#define DTLB_PQ_SIZE 0
#define DTLB_MSHR_SIZE 8
#define DTLB_LATENCY 1

// SECOND LEVEL TLB
#define STLB_SET 128
#define STLB_WAY 12
#define STLB_RQ_SIZE 32
#define STLB_WQ_SIZE 32
#define STLB_PQ_SIZE 0
#define STLB_MSHR_SIZE 16
#define STLB_LATENCY 8

// L1 INSTRUCTION CACHE
#define L1I_SET 64
#define L1I_WAY 8
#define L1I_RQ_SIZE 64
#define L1I_WQ_SIZE 64
#define L1I_PQ_SIZE 32
#define L1I_MSHR_SIZE 8/2
#define L1I_LATENCY 4

// L1 DATA CACHE
#define L1D_SET 64
#define L1D_WAY 12
#define L1D_RQ_SIZE 64
#define L1D_WQ_SIZE 64
#define L1D_PQ_SIZE 8
#define L1D_MSHR_SIZE 16/2
#define L1D_LATENCY 5

// L2 CACHE
#define L2C_SET 1024
#define L2C_WAY 8
#define L2C_RQ_SIZE 32
#define L2C_WQ_SIZE 32
#define L2C_PQ_SIZE 16
#define L2C_MSHR_SIZE 32/2
#define L2C_LATENCY 10  // 4/5 (L1I or L1D) + 10 = 14/15 cycles

// LAST LEVEL CACHE
#define LLC_SET NUM_CPUS*2048
#define LLC_WAY 32
#define LLC_RQ_SIZE NUM_CPUS*L2C_MSHR_SIZE //48
#define LLC_WQ_SIZE NUM_CPUS*L2C_MSHR_SIZE //48
#define LLC_PQ_SIZE NUM_CPUS*32
#define LLC_MSHR_SIZE NUM_CPUS*64/2
#define LLC_LATENCY 20  // 4/5 (L1I or L1D) + 10 + 20 = 34/35 cycles


// #define NUM_BANKS 2
// #define ENTRIES_PER_BANK 64
// #define TAG_SIZE 12

// typedef struct {
//     uint16_t tags[ENTRIES_PER_BANK];
// } RR_BANK;

// typedef struct {
//     RR_BANK banks[NUM_BANKS];
// } RECENT_REQUESTS_TABLE;

class CACHE : public MEMORY {
public:
uint32_t cpu;
const string NAME;
const uint32_t NUM_SET, NUM_WAY, NUM_LINE, WQ_SIZE, RQ_SIZE, PQ_SIZE, MSHR_SIZE;
uint32_t LATENCY;
BLOCK **block;
// O3_CPU* o3;
// UNCORE* uncore;
int fill_level;
uint32_t MAX_READ, MAX_FILL;
uint32_t reads_available_this_cycle;
uint8_t cache_type;
// int offset = 1;     // the prefetch offset
// RECENT_REQUESTS_TABLE rr_table;
// int offsets[NUM_OFFSETS] = {1, 2, 3, 4, 5, 6, 8, 9, 10, 12, 15, 16, 18, 20, 24, 25, 27, 30, 32, 36, 40, 45, 48, -1, -2, -3, -4, -5, -6, -8, -9, -10, -12, -15, -16, -18, -20, -24, -25, -27, -30, -32, -36, -40, -45, -48};
// int scores[NUM_OFFSETS];
// int round = 1;
// int test_offset = 0;
// bool prefetch_on = true;
// uint16_t delay_queue[15];
CACHE *prev_level, *next_level_d[NUM_CPUS], *next_level_i[NUM_CPUS];

uint64_t addresses[LIRS_SIZE];
uint64_t lirs[LIRS_SIZE];
uint8_t present[LIRS_SIZE];

// prefetch stats
uint64_t pf_requested,
pf_issued,
pf_useful,
pf_useless,
pf_fill;

// queues
PACKET_QUEUE WQ{NAME + "_WQ", WQ_SIZE}, // write queue
RQ{NAME + "_RQ", RQ_SIZE}, // read queue
PQ{NAME + "_PQ", PQ_SIZE}, // prefetch queue
MSHR{NAME + "_MSHR", MSHR_SIZE}, // MSHR
PROCESSED{NAME + "_PROCESSED", ROB_SIZE}; // processed queue

uint64_t sim_access[NUM_CPUS][NUM_TYPES],
sim_hit[NUM_CPUS][NUM_TYPES],
sim_miss[NUM_CPUS][NUM_TYPES],
roi_access[NUM_CPUS][NUM_TYPES],
roi_hit[NUM_CPUS][NUM_TYPES],
roi_miss[NUM_CPUS][NUM_TYPES];

uint64_t total_miss_latency;

// constructor
CACHE(string v1, uint32_t v2, int v3, uint32_t v4, uint32_t v5, uint32_t v6, uint32_t v7, uint32_t v8)
: NAME(v1), NUM_SET(v2), NUM_WAY(v3), NUM_LINE(v4), WQ_SIZE(v5), RQ_SIZE(v6), PQ_SIZE(v7), MSHR_SIZE(v8) {

LATENCY = 0;

// cache block
block = new BLOCK* [NUM_SET];
for (uint32_t i=0; i<NUM_SET; i++) {
block[i] = new BLOCK[NUM_WAY];

for (uint32_t j=0; j<NUM_WAY; j++) {
block[i][j].lru = j;
}
}

for (uint32_t i=0; i<NUM_CPUS; i++) {
upper_level_icache[i] = NULL;
upper_level_dcache[i] = NULL;

for (uint32_t j=0; j<NUM_TYPES; j++) {
sim_access[i][j] = 0;
sim_hit[i][j] = 0;
sim_miss[i][j] = 0;
roi_access[i][j] = 0;
roi_hit[i][j] = 0;
roi_miss[i][j] = 0;
}
}

total_miss_latency = 0;

lower_level = NULL;
extra_interface = NULL;
fill_level = -1;
MAX_READ = 1;
MAX_FILL = 1;

pf_requested = 0;
pf_issued = 0;
pf_useful = 0;
pf_useless = 0;
pf_fill = 0;
// memset(&rr_table, 0, sizeof(rr_table));

// refresh_scores();
// for(int i=0;i<15;i++){
//     delay_queue[i] = 0;
// }

for(int i=0;i<LIRS_SIZE;i++){
addresses[i] = 0;
}
for(int i=0;i<LIRS_SIZE;i++){
lirs[i] = INF;
}
for(int i=0;i<LIRS_SIZE;i++){
present[i] = 0;
}

};

// destructor
~CACHE() {
for (uint32_t i=0; i<NUM_SET; i++)
delete[] block[i];
delete[] block;
};
void exclusive_read_hit(uint32_t set, uint32_t way, uint8_t cache_type, uint64_t instr_id, uint32_t cpu_index);
void wb_next_level(CACHE *c, uint32_t set, uint32_t way, uint64_t instr_id, uint32_t cpu_index);
void exclusive_fill(uint32_t set, uint32_t way, uint8_t cache_type, uint64_t instr_id, uint32_t cpu_index);
void invalidate_lower(uint32_t set, uint32_t way, uint8_t cache_type, uint64_t instr_id, uint32_t cpu_index);
void wb_dram(CACHE *c, uint32_t set, uint32_t way, uint64_t instr_id, uint32_t cpu_index);
void wb_prev_level(CACHE *c, uint32_t set, uint32_t way, uint64_t instr_id, uint32_t cpu_index);


int  add_rq(PACKET *packet),
add_wq(PACKET *packet),
add_pq(PACKET *packet);

void return_data(PACKET *packet),
operate(),
increment_WQ_FULL(uint64_t address);

uint32_t get_occupancy(uint8_t queue_type, uint64_t address),
get_size(uint8_t queue_type, uint64_t address);

int  check_hit(PACKET *packet),
invalidate_entry(uint64_t inval_addr),
check_mshr(PACKET *packet),
prefetch_line(uint64_t ip, uint64_t base_addr, uint64_t pf_addr, int prefetch_fill_level, uint32_t prefetch_metadata),
kpc_prefetch_line(uint64_t base_addr, uint64_t pf_addr, int prefetch_fill_level, int delta, int depth, int signature, int confidence, uint32_t prefetch_metadata);

void handle_fill(),
handle_writeback(),
handle_read(),
handle_prefetch();

uint32_t fifo_evict(uint32_t set);
uint32_t lfu_evict(uint32_t set);

void add_mshr(PACKET *packet),
update_fill_cycle(),
llc_initialize_replacement(),
update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit),
llc_update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit),
lru_update(uint32_t set, uint32_t way),
lfu_update(uint32_t set, uint32_t way),
fifo_update(uint32_t set, uint32_t way),
fill_cache(uint32_t set, uint32_t way, PACKET *packet),
replacement_final_stats(),
llc_replacement_final_stats(),
//prefetcher_initialize(),
l1d_prefetcher_initialize(),
l2c_prefetcher_initialize(),
llc_prefetcher_initialize(),
prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type),
l1d_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type),
prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr),
l1d_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in),
//prefetcher_final_stats(),
l1d_prefetcher_final_stats(),
l2c_prefetcher_final_stats(),
llc_prefetcher_final_stats();
void (*l1i_prefetcher_cache_operate)(uint32_t, uint64_t, uint8_t, uint8_t);
void (*l1i_prefetcher_cache_fill)(uint32_t, uint64_t, uint32_t, uint32_t, uint8_t, uint64_t);

uint32_t l2c_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in),
llc_prefetcher_operate(uint64_t addr, uint64_t ip, uint8_t cache_hit, uint8_t type, uint32_t metadata_in),
l2c_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in),
llc_prefetcher_cache_fill(uint64_t addr, uint32_t set, uint32_t way, uint8_t prefetch, uint64_t evicted_addr, uint32_t metadata_in);

uint32_t get_set(uint64_t address),
get_way(uint64_t address, uint32_t set),
find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
llc_find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
fifo_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
lfu_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
lirs_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type),
lfru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type);
};

#endif


// BO prefetcher code:

// void refresh_scores(){
//     for(int i = 0;i<NUM_OFFSETS;i++){
//         scores[i] = 0;
//     }
//     prefetch_on = true;
// }

// // Function to check if a tag exists in the RR table
// bool rr_table_lookup(uint64_t tag) {
//     uint16_t tag_hash_bank1 = hash_tag_bank1(tag);
//     uint16_t tag_hash_bank2 = hash_tag_bank2(tag);

//     bool found = false;

//     // Check bank 1
//     RR_BANK* bank1 = &rr_table.banks[0];
//     for (int i = 0; i < ENTRIES_PER_BANK; i++) {
//         if (bank1->tags[i] == tag_hash_bank1) {
//             found = true;
//             break;
//         }
//     }

//     if(!found){
//         return false;
//     }

//     // Check bank 2
//     RR_BANK* bank2 = &rr_table.banks[1];
//     for (int i = 0; i < ENTRIES_PER_BANK; i++) {
//         if (bank2->tags[i] == tag_hash_bank2) {
//             return true;
//         }
//     }

//     return false;
// }

// // Function to insert a tag into the RR table
// void rr_table_insert(uint64_t tag) {
//     uint16_t tag_hash_bank1 = hash_tag_bank1(tag);
//     uint16_t tag_hash_bank2 = hash_tag_bank2(tag);
//     bool found = false;

//     // Insert into bank 1
//     RR_BANK* bank1 = &rr_table.banks[0];
//     for (int i = 0; i < ENTRIES_PER_BANK; i++) {
//         if (bank1->tags[i] == 0) {
//             bank1->tags[i] = tag_hash_bank1;
//             found = true;
//             break;
//         }
//     }
//     if(!found){
//         // Replace oldest entry in bank 1
//         for (int i = 0; i < ENTRIES_PER_BANK - 1; i++) {
//             bank1->tags[i] = bank1->tags[i+1];
//         }
//         bank1->tags[ENTRIES_PER_BANK - 1] = tag_hash_bank1;
//     }

//     //insert into delay queue for bank 1
//     for(int i=0;i<DELAY;i++){
//         delay_queue[i+1] = delay_queue[i];
//     }
//     delay_queue[0] = tag_hash_bank2;
//     rr2_insert();
// }

// void rr2_insert(){
//     if(delay_queue[DELAY]==0){
//         return;
//     }
//     uint16_t tag_hash_bank2 = delay_queue[DELAY];
//     bool found = false;
//     // Insert into bank 2
//     RR_BANK* bank2 = &rr_table.banks[1];
//     for (int i = 0; i < ENTRIES_PER_BANK; i++) {
//         if (bank2->tags[i] == 0) {
//             bank2->tags[i] = tag_hash_bank2;
//             found = true;
//             break;
//         }
//     }
//     if(!found){
//         // Replace oldest entry in bank 2
//         for (int i = 0; i < ENTRIES_PER_BANK - 1; i++) {
//             bank2->tags[i] = bank2->tags[i+1];
//         }
//         bank2->tags[ENTRIES_PER_BANK - 1] = tag_hash_bank2;
//     }
// }

// Change this to mean offsetN and not all offsets
// void update_scores(uint64_t addr){
//     uint64_t base_addr;
//     base_addr = ((addr >> LOG2_BLOCK_SIZE) - offsets[test_offset]) << LOG2_BLOCK_SIZE;
//     if(rr_table_lookup(base_addr)){
//         scores[test_offset]++;
//         if(scores[test_offset]==SCORE_MAX){
//             offset = offsets[test_offset];
//             //start new phase
//             round = 0;
//             test_offset = 0;
//             refresh_scores();
//             return;
//         }
//     }
//     test_offset++;
//     if(test_offset==NUM_OFFSETS){
//         test_offset = 0;
//         round++;    //new round
//         if(round==ROUND_MAX){
//             //start new phase
//             int max = 0;
//             for(int i=0;i<NUM_OFFSETS;i++){
//                 if(scores[i]>scores[max]){
//                     max = i;
//                 }
//             }
// 	refresh_scores();
// 	round = 0;
//             if(scores[max]<=BAD_SCORE){
//                 //turn off prefetching
//                 prefetch_on = false;
//             }
//             else{
//                 offset = offsets[max];
//             }
//             return;
//         }
//     }

// }

// // functions
// // Define two hash functions to map 64-bit integers to 12-bit integers
// uint16_t hash_tag_bank1(uint64_t tag) {
//     return tag % 4096; // 2^12
// }

// uint16_t hash_tag_bank2(uint64_t tag) {
//     return (tag >> 32) % 4096; // Use upper 32 bits for bank 2 hash
// };

