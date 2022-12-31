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
            void create_tree_collection(std::string input_file, std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);

            void huffman_tree(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);
            
            std::set<std::pair<int, std::shared_ptr<paimon::node>>>::iterator fetch_two(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection);

            void set_kids(std::shared_ptr<node> parent, std::shared_ptr<node> left_child, std::shared_ptr<node> right_child);

            void create_lookup_table();

            void checkup_node(std::shared_ptr<node> curr_node, std::vector<bool> code);

            bool is_leaf(std::shared_ptr<node> curr_node);

            void create_compressed(std::string input_file);

            std::shared_ptr<node> root_; 

            std::unordered_map<char, std::vector<bool>> char_table;
            
            std::map<std::vector<bool>, char> lookup_table;

    }; 
}   