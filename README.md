# Cache-Optimizer
Repository of the research (previously done and Our Contribution) on optimization of cache usage using Machine Learning Algorithms. There are 2 basic ways through which cache usage can be maximised:
1. Cache Eviction
2. Pre-Fetching
# Cache Eviction
## 1. Driving Cache Replacement with ML-based LeCAR - Giuseppe Vietri et. al
Proposed a general framework called **LeCaR** that uses the ML technique of regret minimization to answer the question in the affirmative. Paper shows that the LeCaR framework outperforms **ARC** using only two fundamental eviction policies, LRU and LFU, by more than 18x when the cache size is small relative to the size of the working set.
### [Implementation](./lecar.c)
Implemented in C with the following parameters:<br/>
Cache Size = 12<br/>
Size of History = 4<br/>
Learning Rate = 0.45<br/>
Discount Rate = 0.64
