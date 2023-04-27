# ./cache_size.sh 64 1024 2048 1
# sed -i 's/,3,/,1,/g' test.sh #>temp.sh 
# mv temp.sh  test.sh
# ./runner.sh
./cache_size.sh 64 1024 2048 2
sed -i 's/,1,/,2,/g' test.sh #>temp.sh 
# mv temp.sh  test.sh
./runner.sh
./cache_size.sh 64 1024 2048 3
sed -i 's/,2,/,3,/g' test.sh #>temp.sh 
# mv temp.sh test.sh
./runner.sh
