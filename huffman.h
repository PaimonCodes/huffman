#ifndef HUFFMAN_H
#define HUFFMAN_H


/*
    Dr. Huffman's algorithm as implemented in C++.
*/

#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <set>
#include <memory>
#include <map>
#include <bitset>
#include <vector>

namespace paimon
{
    class huffman;

    class node
    {
        public:
            node(int sum_frequency);
            node(char character, int frequency);
        private:
            int freq_;
            std::unique_ptr<char> char_;
            std::shared_ptr<node> left_;
            std::shared_ptr<node> right_;
            std::shared_ptr<node> parent_;

            friend huffman;
    };

    class huffman
    {
        public:
            static huffman compress(std::string input_file);

            void decompress_file(std::string compressed_file);

            void lookup_table_show();

        private:
            // create set of subtrees with their frequencies sorted as key
            void create_tree_collection(std::string input_file, std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);

            // create Huffman tree
            void huffman_tree(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);

            // fetch two subtrees, add their frequencies, set parent and children
            // remove children from set of subtrees, and reinsert parent into set
            std::set<std::pair<int, std::shared_ptr<paimon::node>>>::iterator fetch_two(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);

            // set parent-children relationship
            void set_kids(std::shared_ptr<node> parent, std::shared_ptr<node> left_child, std::shared_ptr<node> right_child);

            void create_lookup_table();

            // recursively go down left and right nodes while assigning bit codes
            void checkup_node(std::shared_ptr<node> curr_node, std::vector<bool> code);

            // check if current node is a leaf node
            bool is_leaf(std::shared_ptr<node> curr_node);

            // Huffman encodes the input file
            void create_compressed(std::string input_file);

            // pack bits into bytes and write to ofstream
            void fetch_and_flush(std::unordered_map<char, std::vector<bool>>::iterator itr, char* cur_byte,
                                 int* bit_count, std::ofstream* output);

            // decode bytes and end if EOF encountered
            void decode(std::string::iterator itr, std::vector<bool>* code, std::ifstream* input, std::ofstream* output);

            // root of Huffman tree
            std::shared_ptr<node> root_;

            // for character quick finds when encoding
            std::unordered_map<char, std::vector<bool>> char_table;

            // for decoding
            std::map<std::vector<bool>, char> lookup_table;

    };
}

#endif // HUFFMAN_H
