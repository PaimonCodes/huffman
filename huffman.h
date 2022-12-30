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
            char char_;
            int freq_;
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

            // fetch operations
            std::shared_ptr<paimon::node> fetch_two(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                                const std::set<std::pair<char, int>, compare>& frequency_table);
            std::shared_ptr<paimon::node> fetch_one(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                                const std::set<std::pair<char, int>, compare>& frequency_table,
                                                std::shared_ptr<node> parent);

            void set_kids(std::shared_ptr<node> parent, std::shared_ptr<node> left, std::shared_ptr<paimon::node> right);

            void update_root(const std::shared_ptr<node>& parent);
            void create_lookup_table();
            void checkup_node(std::shared_ptr<paimon::node> sub_node, std::vector<bool> code);

            void set_code(std::shared_ptr<node> sub_node, std::vector<bool>* code);
            void create_compressed(std::string input_file);

            std::shared_ptr<node> root_;
            std::unordered_map<char, std::vector<bool>> char_table;
            std::map<std::vector<bool>, char> lookup_table;
            
            
            
    }; 
}   