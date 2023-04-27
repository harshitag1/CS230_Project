#!/bin/bash
cd inc
sed -i "/LLC_WAY/{s/.*/#define LLC_WAY 1/;:a;n;ba}" cache.h
cd ..
./build_champsim.sh bimodal no no no no lru 1
./run_champsim.sh bimodal-no-no-no-no-lru-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no lfu 1
./run_champsim.sh bimodal-no-no-no-no-lfu-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no random 1
./run_champsim.sh bimodal-no-no-no-no-random-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no fifo 1
./run_champsim.sh bimodal-no-no-no-no-fifo-1core 20 30 482.xz
mv results_30M ../../res1way/482

cd inc
sed -i "/LLC_WAY/{s/.*/#define LLC_WAY 4/;:a;n;ba}" cache.h
cd ..
./build_champsim.sh bimodal no no no no lru 1
./run_champsim.sh bimodal-no-no-no-no-lru-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no lfu 1
./run_champsim.sh bimodal-no-no-no-no-lfu-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no random 1
./run_champsim.sh bimodal-no-no-no-no-random-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no fifo 1
./run_champsim.sh bimodal-no-no-no-no-fifo-1core 20 30 482.xz
mv results_30M ../../res4way/482

cd inc
sed -i "/LLC_WAY/{s/.*/#define LLC_WAY 16/;:a;n;ba}" cache.h
cd ..
./build_champsim.sh bimodal no no no no lru 1
./run_champsim.sh bimodal-no-no-no-no-lru-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no lfu 1
./run_champsim.sh bimodal-no-no-no-no-lfu-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no random 1
./run_champsim.sh bimodal-no-no-no-no-random-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no fifo 1
./run_champsim.sh bimodal-no-no-no-no-fifo-1core 20 30 482.xz
mv results_30M ../../res16way/482

cd inc
sed -i "/LLC_WAY/{s/.*/#define LLC_WAY 32/;:a;n;ba}" cache.h
cd ..
./build_champsim.sh bimodal no no no no lru 1
./run_champsim.sh bimodal-no-no-no-no-lru-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no lfu 1
./run_champsim.sh bimodal-no-no-no-no-lfu-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no random 1
./run_champsim.sh bimodal-no-no-no-no-random-1core 20 30 482.xz
./build_champsim.sh bimodal no no no no fifo 1
./run_champsim.sh bimodal-no-no-no-no-fifo-1core 20 30 482.xz
mv results_30M ../../res32way/482
