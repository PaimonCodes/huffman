huffman.o: huffman.cpp huffman.h
	g++ -std=c++14 -c huffman.cpp
compress: main.cxx huffman.o
	g++ -std=c++14 main.cxx huffman.o -o compress.exe 

.PHONY: clean
clean:
	rm -f *.exe
	rm -f *.o
	rm -f *.bin