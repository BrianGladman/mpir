# 64 bit proxy DLL for fat MSVC builds of MPIR

This solution generates a stub proxy DLL for the MPIR library. When applications link to MPIR, they will call this stub proxy DLL instead. The stub proxy DLL analyzes the CPU model it is running on, and then uses LoadLibrary to load the appropriate optimized version of MPIR.dll. It patches all of the MPIR function entrypoints and JMPs to the appropriate function in the optimized version of MPIR.

The basis for the 64 bit stub DLL was automatically generated from the 64 bit GC version of MPIR.dll using the tool below:

https://www.codeproject.com/Articles/1179147/ProxiFy-Automatic-Proxy-DLL-Generation
