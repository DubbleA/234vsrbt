# Real-World Performance of Red-Black vs. 2-3-4 trees, especially considering cache performance?

[stack overflow link](https://stackoverflow.com/questions/56404500/real-world-performance-of-red-black-vs-2-3-4-trees-especially-considering-cach)

**Plan to benchmark on NYU Greene Supercomputing cluster**

### Step 1: Implement the Data Structure
- Red-Black Tree and 2-3-4 Tree Implementation: Impl both trees in C++ w/ proper memory layout. Ensure that nodes are cache-line aligned (maybe use alignas specifier in C++11)
- Uniform Interface: Design common interface for insertion, deletion, and search operations for uniformity between benchmark tests
- Create random input generator for uniform interface for live testing
- Setup cuda code

### Step 2: Port CUDA to AMD
- adapt CUDA code to HIP (Heterogeneous-compute Interface for Portability) or directly use OpenCL. Since AMD GPUs typically use ROCm
- optimize architecture: understand cache hierarchy & memory bandwidth of AMD M100 gpus. Aligning data structures (tree nodes) to cache lines and ensuring coalesced memory accesses can affect performance.
- Utilize ROCm's rocm-profiler or rocprof to analyze cache hits/misses and memory usage

### Step 3: Benchmark

### Step 4: Executing and Profiling

### Step 5: Analysis and Report