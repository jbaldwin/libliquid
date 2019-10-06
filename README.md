# libliquid

This library is an attempt to write an HTTP/1.1 stateful parser for high performance and maximizing throughput.

The library will use SIMD instructions if available, for my benefit this was done to see what kind of a performance increase 
an expensive paresr like HTTP can gain by uses these types of instructions.
