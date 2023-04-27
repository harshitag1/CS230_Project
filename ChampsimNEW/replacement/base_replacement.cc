#include "cache.h"
#include "time.h"


uint32_t CACHE::find_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    // baseline LRU replacement policy for other caches 
    return lru_victim(cpu, instr_id, set, current_set, ip, full_addr, type); 
}

void CACHE::update_replacement_state(uint32_t cpu, uint32_t set, uint32_t way, uint64_t full_addr, uint64_t ip, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    if (type == WRITEBACK) {
        if (hit) // wrietback hit does not update LRU state
            return;
    }

    return lru_update(set, way);
}

uint32_t CACHE::fifo_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{  
    uint32_t way = 0;
    uint64_t max_fifo = block[set][0].fifo;

    // fill invalid line first
    for (way=0; way<NUM_WAY; way++) {
        if (block[set][way].valid == false) {
            DP ( if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " fifo: " << block[set][way].fifo << endl; });

            break;
        }
    }

    // FIFO victim
    if (way == NUM_WAY) {
        for (int i=0; i<NUM_WAY; i++) {
            if (block[set][i].fifo > max_fifo) {
                max_fifo = block[set][i].fifo;
                way = i;
            }
        }
        DP ( if (warmup_complete[cpu]) {
        cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
        cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
        cout << dec << " fifo: " << block[set][way].fifo << endl; });
    }

    if (way == NUM_WAY) {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}

// uint32_t CACHE::lfu_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
// {
//     uint32_t way = 0;
//     uint32_t min_lfu = block[set][0].lfu;

//     // fill invalid line first
//     for (way = 0; way < NUM_WAY; way++) {
//         if (block[set][way].valid == false) {
//             DP ( if (warmup_complete[cpu]) {
//             cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
//             cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//             cout << dec << " lfu: " << block[set][way].lfu << endl; });

//             break;
//         }
//     }
//     // LFU victim
//     if (way == NUM_WAY) {
//         for (int i = 0; i < NUM_WAY; i++) {
//             if (block[set][i].lfu < min_lfu) {
//                 min_lfu = block[set][i].lfu;
//                 way = i;
//             }
//         }
//         DP ( if (warmup_complete[cpu]) {
//         cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
//         cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//         cout << dec << " lru: " << block[set][way].lru << endl; });
//     }

//     if (way == NUM_WAY) {
//         cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
//         assert(0);
//     }

//     return way;
// }


uint32_t CACHE::lru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type)
{
    uint32_t way = 0;

    // fill invalid line first
    for (way=0; way<NUM_WAY; way++) {
        if (block[set][way].valid == false) {

            DP ( if (warmup_complete[cpu]) {
            cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
            cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
            cout << dec << " lru: " << block[set][way].lru << endl; });

            break;
        }
    }

    // LRU victim
    if (way == NUM_WAY) {
        for (way=0; way<NUM_WAY; way++) {
            if (block[set][way].lru == NUM_WAY-1) {

                DP ( if (warmup_complete[cpu]) {
                cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
                cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
                cout << dec << " lru: " << block[set][way].lru << endl; });

                break;
            }
        }
    }

    if (way == NUM_WAY) {
        cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
        assert(0);
    }

    return way;
}



// uint32_t CACHE::lfru_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type){
//     // baseline LRU
//     uint32_t way = 0;

//     // fill invalid line first
//     for (way=0; way<NUM_WAY; way++) {
//         if (block[set][way].valid == false) {

//             DP ( if (warmup_complete[cpu]) {
//             cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
//             cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//             cout << dec << " lru: " << block[set][way].lru << endl; });

//             break;
//         }
//     }

//     // LRU victim
//     if (way == NUM_WAY) {
//         uint64_t m = 0;

//         for ( uint32_t j = 0 ; j < LLC_WAY ; j++ ){
//             if ( freqs[set][j] <= freqs[set][m] ){
//                 m = j;
//             }
//         }
//         way = m;
//                 DP ( if (warmup_complete[cpu]) {
//                 cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
//                 cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//                 cout << dec << " lru: " << block[set][way].lru << endl; });
//     }

//     if (way == NUM_WAY) {
//         cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
//         assert(0);
//     }

//     return way;
    // uint32_t way = 0;
    // uint32_t min_lfu = block[set][0].lfu;

    // // fill invalid line first
    // for (way = 0; way < NUM_WAY; way++) {
    //     if (block[set][way].valid == false) {
    //         DP ( if (warmup_complete[cpu]) {
    //         cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
    //         cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
    //         cout << dec << " lfu: " << block[set][way].lfu << endl; });

    //         break;
    //     }
    // }
    // uint32_t min_lru = block[set][0].lru;

    // // LFU victim
    // if (way == NUM_WAY) {
    //     for (int i = 0; i < NUM_WAY; i++) {
    //         if (block[set][i].lfu < min_lfu) {
    //             min_lfu = block[set][i].lfu;
    //         }
    //     }
        
    //     for(int i=0; i<NUM_WAY; i++){
    //         if(block[set][i].lfu == min_lfu){
    //             if(block[set][i].lru < min_lru){
    //                 way = i;
    //                 min_lru = block[set][i].lru;
    //             }
    //         }
    //     }

    //     DP ( if (warmup_complete[cpu]) {
    //     cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
    //     cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
    //     cout << dec << " lru: " << block[set][way].lru << endl; });
    // }

    // if (way == NUM_WAY) {
    //     cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
    //     assert(0);
    // }

    // return way;
// }


// uint32_t CACHE::lirs_victim(uint32_t cpu, uint64_t instr_id, uint32_t set, const BLOCK *current_set, uint64_t ip, uint64_t full_addr, uint32_t type){
//     uint32_t way = 0;
//     uint64_t max_lirs = 0;

//     uint64_t addresses[LIRS_SIZE];
//     for(int i=0;i<LIRS_SIZE;i++){
//         addresses[i] = 0;
//     }
//     uint64_t lirs[LIRS_SIZE];
//     for(int i=0;i<LIRS_SIZE;i++){
//         lirs[i] = INF;
//     }
//     uint8_t present[LIRS_SIZE];
//     for(int i=0;i<LIRS_SIZE;i++){
//         present[i] = 0;
//     }

//     // fill invalid line first
//     for (way=0; way<NUM_WAY; way++) {
//         if (block[set][way].valid == false || block[set][way].lirs == INF) {
//             DP ( if (warmup_complete[cpu]) {
//             cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " invalid set: " << set << " way: " << way;
//             cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//             cout << dec << " lirs: " << block[set][way].lirs << endl; });

//             break;
//         }
//     }

//     // LIRS victim
//     if (way == NUM_WAY) {
//         int jndex = 0;
//         for (int i=0; i<NUM_WAY; i++) {
//             uint64_t addr = block[set][way].address;
//             for(int j=0;j<LIRS_SIZE;j++){
//                 if(addresses[j]==addr){
//                     jndex = j;
//                     break;
//                 }
//             }
//             if (lirs[jndex] > max_lirs) {
//                 max_lirs = lirs[jndex];
//                 way = i;
//             }
//         }
//         present[jndex] = 0;
//         DP ( if (warmup_complete[cpu]) {
//         cout << "[" << NAME << "] " << __func__ << " instr_id: " << instr_id << " replace set: " << set << " way: " << way;
//         cout << hex << " address: " << (full_addr>>LOG2_BLOCK_SIZE) << " victim address: " << block[set][way].address << " data: " << block[set][way].data;
//         cout << dec << " lirs: " << block[set][way].lirs << endl; });
//     }

//     if (way == NUM_WAY) {
//         cerr << "[" << NAME << "] " << __func__ << " no victim! set: " << set << endl;
//         assert(0);
//     }

//     return way;
// }

void CACHE::lru_update(uint32_t set, uint32_t way)
{
    // update lru replacement state
    for (uint32_t i=0; i<NUM_WAY; i++) {
        if (block[set][i].lru < block[set][way].lru) {
            block[set][i].lru++;
        }
    }
    block[set][way].lru = 0; // promote to the MRU position
    return;
}

void CACHE::replacement_final_stats()
{

}

#ifdef NO_CRC2_COMPILE
void InitReplacementState()
{
    
}

uint32_t GetVictimInSet (uint32_t cpu, uint32_t set, const BLOCK *current_set, uint64_t PC, uint64_t paddr, uint32_t type)
{
    return 0;
}

void UpdateReplacementState (uint32_t cpu, uint32_t set, uint32_t way, uint64_t paddr, uint64_t PC, uint64_t victim_addr, uint32_t type, uint8_t hit)
{
    
}

void PrintStats_Heartbeat()
{
    
}

void PrintStats()
{

}
#endif
