# ./cache_size.sh 64 1024 2048 1
./build_champsim.sh bimodal no no no no lru 1
./run_champsim.sh bimodal-no-no-no-no-lru-1core 30 30 bfs-3.trace.gz
ipc=$(cat results_10M/bfs-3.trace.gz-bimodal-no-no-no-no-lru-1core.txt | grep "CPU 0 cumulative IPC:" | cut -d ":" -f 2 | cut -d " " -f 2)
l1d=$(cat results_10M/bfs-3.trace.gz-bimodal-no-no-no-no-lru-1core.txt | grep "L1D TOTAL")
l2c=$(cat results_10M/bfs-3.trace.gz-bimodal-no-no-no-no-lru-1core.txt | grep "L2C TOTAL")
llc=$(cat results_10M/bfs-3.trace.gz-bimodal-no-no-no-no-lru-1core.txt | grep "LLC TOTAL")

echo lru,no,1,bfs-3.trace.gz,${ipc} >> policy_comparison.txt
echo -----------------------,${l1d} >> policy_comparison.txt
echo -----------------------,${l2c} >> policy_comparison.txt
echo -----------------------,${llc} >> policy_comparison.txt

