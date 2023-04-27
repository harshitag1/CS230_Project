./incpo.sh
sed -i 's/no no no no/no no ip_stride no/g' test.sh
sed -i 's/no-no-no-no/no-no-ip_stride-no/g' test.sh
sed -i 's/,no,/,st,/g' test.sh
./incpo.sh
sed -i 's/no no ip_stride no/no no no no/g' test.sh
sed -i 's/no-no-ip_stride-no/no-no-no-no/g' test.sh
sed -i 's/,st,/,no,/g' test.sh
