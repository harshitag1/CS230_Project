# CS230_Project

Team ASAP

1. Harshit Agarwal - 210020054

2. Aadish Sethiya - 210100001

3. Premankur Chakraborty - 210050123

## NOTE
Please note that our team was not aware of the fact that submission was through a github repository. Hence, we did not regularly use github to keep up with all of our work. We only found out about this today morning and hence, we added all of our files in a series of a few commits in about an hour. 

Kindly do not penalize us for this - we have been doing the project over the last week, we have not rushed through all of it in the last 2 hours (as it might look like from the repo commit history). We used whatsapp and email to keep track of files and did everything locally on our own systems, without the use of github.

## Links

Link to our presentation - https://docs.google.com/presentation/d/14QwFbxAjonSrqkHlFMMptGp8ygxQ8QqWTBZOyCMvZlM/edit?usp=sharing

Link to our video - https://youtu.be/tIVXPxivN58

## Implementation
We have implemented Inclusive and Exclusive cache inclusion policies over the default Non-Inclusive policy. Also we tested our code for different cache sizes by varying the number of sets. They can be checked using the bash script 'cache_size.sh'. 

```bash
./cache_size.sh [L1D_SETS] [L2C_SETS] [LLC_SETS] [POLICY]
```
Where policy can be initialised with 1 for inclusive, 2 for exclusive and 3 for non-inclusive. After running this bash script, run the champsim as usual and it will work according to the specifications.

## Optimizations:
1. Optimal implementations of inclusive, exclusive and non-inclusive caches
2. New replacement policies like LIRS.
3. Optimizing LFU to get LFU with minimum threshold and decay. 
4. Creating LFRU to further improve LFU and using the above 2 techniques of LFRU as well.
