# l1d,l2c,llc number of sets will be changed with this script
# also used to control whether it is inclusive, exclusive or non-inclusive

if [[ $4 != 1 && $4 != 2 && $4 != 3 ]]; then
    echo "Inclusive/Exclusive/Non-Inclusive for 1,2,3 only"
    exit 1
fi

while read line; do
  if [[ $line == *"#define L1D_SET"* ]]; then
    echo "#define L1D_SET $1"
  else
    echo "$line"
  fi
done < inc/cache.h > inc/cache2.h

rm inc/cache.h


while read line; do
  if [[ $line == *"#define L2C_SET"* ]]; then
    echo "#define L2C_SET $2"
  else
    echo "$line"
  fi
done < inc/cache2.h > inc/cache3.h

rm inc/cache2.h


while read line; do
  if [[ $line == *"#define LLC_SET NUM_CPUS"* ]]; then
    echo "#define LLC_SET NUM_CPUS*$3"
  else
    echo "$line"
  fi
done < inc/cache3.h > inc/cache4.h

rm inc/cache3.h

while read line; do
  if [[ $line == *"#define INC_POLICY"* ]]; then
    echo "#define INC_POLICY $4"
  else
    echo "$line"
  fi
done < inc/cache4.h > inc/cache.h

rm inc/cache4.h


