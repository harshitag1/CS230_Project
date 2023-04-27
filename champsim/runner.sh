chmod 777 test.sh
./test.sh
sed -i 's/lru/lfu/g' test.sh #>temp.sh 
# mv temp.sh test.sh
chmod 777 test.sh
./test.sh
sed -i 's/lfu/fifo/g' test.sh #>temp.sh 
# mv temp.sh test.sh
chmod 777 test.sh
./test.sh
sed -i 's/fifo/random/g' test.sh #>temp.sh 
# mv temp.sh test.sh
chmod 777 test.sh
./test.sh
sed -i 's/random/ship/g' test.sh #>temp.sh 
# mv temp.sh test.sh
chmod 777 test.sh
./test.sh
sed -i 's/ship/lru/g' test.sh 

