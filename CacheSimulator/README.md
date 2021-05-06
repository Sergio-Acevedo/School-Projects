## Cache Simulator

Given a series of memory read/write commands, the desired address, cache/blocksize, and the desired mapping method, the program outputs the number of memory read/writes, and cache hits/misses. It will then perform the same operation again, with prefetching the next address after a cache miss. Program follows only FIFO removal implementation. The input format is in a txt file as follows: <br />
Address: Read/Write TargetAddress<br />
#EOF<br />

Execution of the program abides by the following format:<br />
./cachesim.exe Cachesize Mappingmethod fifo Blocksize txtfile<br />
Sample execution:<br />
./cachesim.exe 512 assoc:4 fifo 8 trace2.txt<br />
The three support mapping methods are direct mapping("direct"), n-way associative mapping("assoc:n" where n is a power of 2), and fully associative("assoc").