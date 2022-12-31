#include "huffman.h"

paimon::node::node(int sum_frequency) : char_(nullptr), freq_(sum_frequency),
                                  left_(nullptr), right_(nullptr), parent_(nullptr)
{
    // Constructor takes in sum frequency of left and right child
}

paimon::node::node(char character, int frequency) : char_(std::make_unique<char>(character)), freq_(frequency), 
                                              left_(nullptr), right_(nullptr), parent_(nullptr)
{
    // Constructor creates node with character and frequency
}

paimon::huffman paimon::huffman::compress(std::string input_file) 
{
    /* Create frequency table, huffman tree, and lookup table */
    
    huffman hfman;

    // create init table 
    std::set<std::pair<char, int>, compare> frequency_table;
    hfman.create_frequency_table(input_file, &frequency_table);

    // create tree
    hfman.create_tree(frequency_table);

    // create lookup table
    hfman.create_lookup_table();

    // create the compressed string
    hfman.create_compressed(input_file);

    return hfman;
}

void paimon::huffman::create_frequency_table(std::string input_file, 
                                             std::set<std::pair<char, int>, compare>* frequency_table) 
{
    // init
    std::unordered_map<char, int> quick_insert_search;
    char key;
    std::ifstream extract_char;
    extract_char.open(input_file);

    // extract every node
    // increment frequency if already present
    while (extract_char >> std::noskipws >> key)
    {
        std::unordered_map<char, int>::iterator itr = quick_insert_search.find(key);
        if (itr != quick_insert_search.end()) 
        {
            itr->second++;
        }
        else
        {
            quick_insert_search.insert({key, 1});
        }
    }

    quick_insert_search.insert({'\0', 0});

    // sort the frequencies in a custom set
    frequency_table->insert(quick_insert_search.begin(), quick_insert_search.end());
}

void paimon::huffman::create_tree(const std::set<std::pair<char, int>, compare>& frequency_table)
{
    // Greedy algorithm -  fetch two-one

    auto table_itr = frequency_table.begin();
    while (table_itr != frequency_table.end()) 
    {
        std::shared_ptr<node> parent;
        if (std::next(table_itr, 2) != frequency_table.end())
        {
            parent = fetch_two(table_itr, frequency_table);
            update_root(parent);
            root_ = fetch_one(table_itr, frequency_table, root_);
        }
        else if (std::next(table_itr, 1) != frequency_table.end())
        {
            parent = fetch_two(table_itr, frequency_table);
            update_root(parent);
        }
        else    
        {
            root_ = fetch_one(table_itr, frequency_table, root_); 
        }
    }
}

std::shared_ptr<paimon::node> paimon::huffman::fetch_two(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                    const std::set<std::pair<char, int>, compare>& frequency_table)
{
    std::shared_ptr<paimon::node> ch_left;
    std::shared_ptr<paimon::node> ch_right;

    // assign left and right children
    if (table_itr->first < std::next(table_itr, 1)->first)
    {
        ch_left = std::make_shared<paimon::node>(table_itr->first, table_itr->second);
        ch_right = std::make_shared<paimon::node>(std::next(table_itr, 1)->first, std::next(table_itr, 1)->second);
    }
    else
    {
        ch_left = std::make_shared<paimon::node>(std::next(table_itr, 1)->first, std::next(table_itr, 1)->second);
        ch_right = std::make_shared<paimon::node>(table_itr->first, table_itr->second);
    }

    std::shared_ptr<paimon::node> parent = std::make_shared<paimon::node>(ch_left->freq_ + ch_right->freq_);
    
    // set parent and children connection
    set_kids(parent, ch_left, ch_right);

    // update table_itr
    std::advance(table_itr, 2);

    return parent;
}

std::shared_ptr<paimon::node> paimon::huffman::fetch_one(std::set<std::pair<char, int>, compare>::iterator& table_itr,
                                const std::set<std::pair<char, int>, compare>& frequency_table,
                                std::shared_ptr<paimon::node> parent)
{
    // Fetch one and assign to right of the previous parent node

    std::shared_ptr<paimon::node> ch_node = std::make_shared<paimon::node>(table_itr->first, table_itr->second);
    if (parent != nullptr)
    {
        // Previous parent exists, assign right node, sum frequencies into new parent
        std::shared_ptr<node> new_parent = std::make_shared<paimon::node>(table_itr->second + parent->freq_);
        set_kids(new_parent, parent, ch_node);
        table_itr++;
        return new_parent;
    }
    else
    {
        // Previous parent does not exist yet
        // just return the single character node
        table_itr++;
        return ch_node;
    }
}

void paimon::huffman::set_kids(std::shared_ptr<paimon::node> parent, std::shared_ptr<paimon::node> left, std::shared_ptr<paimon::node> right)
{
    // Assign parent-children relation

    parent->left_ = left;
    parent->right_ = right;

    left->parent_ = parent;
    right->parent_ = parent;
}

void paimon::huffman::update_root(const std::shared_ptr<node>& parent)
{
    if (root_ != nullptr)
    {
        // Sum the old root and the current parent into new root
        std::shared_ptr<paimon::node> new_parent;
        new_parent = std::make_shared<node>(root_->freq_ + parent->freq_);

        set_kids(new_parent, root_, parent);
        root_ = new_parent;
    }
    else
    {
        // Set parent to be the root
        root_ = parent;
    }
}

void paimon::huffman::create_lookup_table()
{
    std::shared_ptr<paimon::node> start_node = root_;
    std::vector<bool> code_bit;
    checkup_node(start_node, code_bit);

    for (auto itr = char_table.begin(); itr != char_table.end(); itr++)
    {
        lookup_table.insert({itr->second, itr->first});
    }
}

void paimon::huffman::checkup_node(std::shared_ptr<paimon::node> sub_node, std::vector<bool> code)
{
    if (sub_node == nullptr)
    {
        return;
    }
    else
    {
        // Recursively go down all left nodes
        code.push_back(0);
        checkup_node(sub_node->left_, code);
        set_code(sub_node, &code);

        // Recursively go down all right nodes
        code.push_back(1);
        checkup_node(sub_node->right_, code);
        set_code(sub_node, &code);
    }
}


void paimon::huffman::set_code(std::shared_ptr<paimon::node> sub_node, std::vector<bool>* code)
{
        // Only insert into lookup table when character is not recorded yet
        // Pop back is necessary for removing false predicted branches during recursion

        code->pop_back();
        if (sub_node->char_ != nullptr)
        {
            char_table.emplace(std::make_pair(*(sub_node->char_), *code));
        }
}

void paimon::huffman::lookup_table_show()
{
    // Print lookup table

    for (auto itr = lookup_table.begin(); itr != lookup_table.end(); itr++)
    {
        for (auto bool_itr = itr->first.begin(); bool_itr != itr->first.end(); bool_itr++)
        {
            std::cout << *bool_itr;
        }
        std::cout << " - " << itr->second << std::endl;
    }
}

void paimon::huffman::create_compressed(std::string input_file)
{
    std::ifstream input;
    input.open(input_file);
    char key;

    int bit_count = 0;
    char cur_byte;
    std::ofstream output;
    output.open("data.bin", std::ios::binary);

    while (input >> std::noskipws >> key)
    {
        auto itr = char_table.find(key);
        for (auto bit_itr = itr->second.begin(); bit_itr != itr->second.end(); bit_itr++)
        {
            if (bit_count == 8)
            {
                output.write(&cur_byte, sizeof(cur_byte));
                bit_count = 0;
                cur_byte = 0;
            }

            cur_byte <<= 1;
            cur_byte |= *bit_itr;
            bit_count++;
        }
    }
    
    if (bit_count > 0)
    {
        // pad the last byte with zeroes
        cur_byte <<= 8 - bit_count;
        output.write(&cur_byte, sizeof(cur_byte));
    }

    output.close();
}