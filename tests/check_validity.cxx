#include "../include/huffman.h"
#include <assert.h>

void check_validity(std::string original, std::string uncompressed)
{
    std::ifstream a(original);
    std::ifstream b(uncompressed);

    char key_a;
    char key_b;

    while(a >> std::noskipws >> key_a && 
          b >> std::noskipws >> key_b)
    {
        assert(key_a == key_b);
    }

    std::cout << "Test Successful" << std::endl;
}

int main(int argc, char** argv)
{
    // compress input file
    paimon::huffman job = paimon::huffman::compress(argv[1]);
    
    // show lookup table
    job.lookup_table_show();

    // decompress and create uncompressed file
    job.decompress_file("data/data.bin");

    check_validity(argv[1], "data/uncompressed.txt");
}