#include "huffman.h"
#include <algorithm>
#include <iterator>


int main(int argc, char** argv)
{
    // takes in text file and deletes old file
    paimon::huffman job = paimon::huffman::compress(argv[1]);
    job.lookup_table_show();

    // creates compressed file
    //job.compressed_release("compressed.bin");

    // prints coded binary sequence
    //job.compressed_show();

    // takes in compressed file and
    // decompress into an output file
    //job.decompress_file("compressed.txt", "uncompressed.txt");

    // takes in compressed file
    // and decodes into terminal
    //job.decompress_show("compressed.txt")


}