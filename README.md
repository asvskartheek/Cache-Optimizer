# Cache-Optimizer
We intend to implement already existing algorithms for cache optimization using ML techniques. </br>
There are two ways in which cache blocks are replaced:</br>
1. Eviction 
2. Prefetching

# Cache Eviction
## 1. Driving Cache Replacement with ML-based LeCAR - Giuseppe Vietri et. al
Proposed a general framework called **LeCaR** that uses the ML technique of regret minimization to answer the question in the affirmative. Paper shows that the LeCaR framework outperforms **ARC** using only two fundamental eviction policies, LRU and LFU, by more than 18x when the cache size is small relative to the size of the working set.
### [Implementation](./lecar.c)
Implemented in C with the following parameters:<br/>
Cache Size = 12<br/>
Size of History = 4<br/>
Learning Rate = 0.45<br/>
Discount Rate = 0.64
