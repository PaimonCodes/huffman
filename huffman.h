#include <iostream>
#include <string>
#include <unordered_map>
#include <fstream>
#include <set>
#include <memory>
#include <deque>
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
            static void decompress_file(std::string compressed_file,
                                        std::string uncompressed_file);
            static void decompress_show(std::string compressed_file);

            void compressed_release(std::string output_file);
            void compressed_show();

            void lookup_table_show();

        private:
            struct compare 
            {
                bool operator ()(const std::pair<char, int>& a, const std::pair<char, int>& b) const
                {
                    return a.second < b.second;
                }
            };

            void create_frequency_table(std::string input_file, std::set<std::pair<char, int>, compare>* frequency_table);

            void create_tree(const std::set<std::pair<char, int>, compare>& frequency_table);

            // fetch two nodes and return parent
            std::shared_ptr<paimon::node> fetch_two(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                                const std::set<std::pair<char, int>, compare>& frequency_table);

            // fetch one node and new parent
            std::shared_ptr<paimon::node> fetch_one(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                                const std::set<std::pair<char, int>, compare>& frequency_table,
                                                std::shared_ptr<node> parent);

            // set parent-chldren relation
            void set_kids(std::shared_ptr<node> parent, std::shared_ptr<node> left, std::shared_ptr<paimon::node> right);

            // update roots if it exists, if not, set parent to be root
            void update_root(const std::shared_ptr<node>& parent);

            // creates lookup table of character frequencies
            void create_lookup_table();

            // Recursively check down left and right children of parent while assigning bits
            void checkup_node(std::shared_ptr<paimon::node> sub_node, std::vector<bool> code);

            // Assign the new binary representation if the node is a character
            void set_code(std::shared_ptr<node> sub_node, std::vector<bool>* code);

            void create_compressed(std::string input_file);

            // Root of huffman tree
            std::shared_ptr<node> root_;

            // char_table used for finding characters' binary representation during encoding
            std::unordered_map<char, std::vector<bool>> char_table;

            // lookup table used for decoding binary stream
            std::map<std::vector<bool>, char> lookup_table;
    }; 
}   