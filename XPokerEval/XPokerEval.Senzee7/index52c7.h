// In Paul Senzee's original code, everything now contained in index52c7.cpp existed
// here in the header file, in fact, there was no index52c7.cpp (only the .h). I have
// created the .cpp file and moved all the code into it, exposing only the actual
// method which performs the conversion from a 52-bit card mask with any 7 bits set
// to a unique number between 0 and 133 million.


unsigned index52c7(unsigned __int64 x);