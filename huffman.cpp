#include "huffman.h"

paimon::node::node(int sum_frequency) : freq_(sum_frequency), char_(nullptr),
                                  left_(nullptr), right_(nullptr), parent_(nullptr)
{
    // Constructor takes in sum frequency of left and right child
}

paimon::node::node(char character, int frequency) : freq_(frequency), char_(std::make_unique<char>(character)),
                                              left_(nullptr), right_(nullptr), parent_(nullptr)
{
    // Constructor creates node with character and frequency
}

paimon::huffman paimon::huffman::compress(std::string input_file)
{

    /* Create frequency table, huffman tree, and lookup table */
    huffman hfman;

    // create init table
    std::set<std::pair<int, std::shared_ptr<paimon::node>>> tree_collection;
    hfman.create_tree_collection(input_file, &tree_collection);

    // create tree
    hfman.huffman_tree(&tree_collection);

    // create lookup table
    hfman.create_lookup_table();

    // compress
    hfman.create_compressed(input_file);

    return hfman;
}

void paimon::huffman::create_tree_collection(std::string input_file, std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection)
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

    for (auto itr = quick_insert_search.begin(); itr != quick_insert_search.end(); itr++)
    {
        std::shared_ptr<node> character_node = std::make_shared<node>(itr->first, itr->second);
        tree_collection->insert({character_node->freq_, character_node});
    }
}

void paimon::huffman::huffman_tree(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection)
{
    // Huffman Algorithm - https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/handouts/220%20Huffman%20Encoding.pdf

    auto itr = tree_collection->begin();
    while (tree_collection->size() > 1)
    {
        itr = fetch_two(tree_collection);
    }
    root_ = itr->second;
}

std::set<std::pair<int, std::shared_ptr<paimon::node>>>::iterator paimon::huffman::fetch_two(std::set<std::pair<int, std::shared_ptr<paimon::node>>>* tree_collection)
{
    auto itr = tree_collection->begin();
    std::shared_ptr<node> ch_left = itr->second;
    std::shared_ptr<node> ch_right = std::next(itr, 1)->second;

    for (int i = 0; i < 2; i++)
    {
        itr = tree_collection->erase(itr);
    }

    std::shared_ptr<node> parent = std::make_shared<node>(ch_left->freq_ + ch_right->freq_);
    set_kids(parent, ch_left, ch_right);
    tree_collection->insert({parent->freq_, parent});

    // reset itr after the reinsert
    itr = tree_collection->begin();

    return itr;
}

void paimon::huffman::set_kids(std::shared_ptr<paimon::node> parent, std::shared_ptr<paimon::node> left_child, std::shared_ptr<paimon::node> right_child)
{
    parent->left_ = left_child;
    parent->right_ = right_child;

    left_child->parent_ = parent;
    right_child->parent_ = parent;
}

void paimon::huffman::create_lookup_table()
{
    std::vector<bool> code;
    std::shared_ptr<node> curr_node = root_;
    checkup_node(curr_node, code);

    for (auto itr = char_table.begin(); itr != char_table.end(); itr++)
    {
        lookup_table.insert({itr->second, itr->first});
    }
}

void paimon::huffman::checkup_node(std::shared_ptr<paimon::node> curr_node, std::vector<bool> code)
{
    if (is_leaf(curr_node))
    {
        char_table.emplace(std::make_pair(*(curr_node->char_), code));
    }
    else
    {
        code.push_back(0);
        checkup_node(curr_node->left_, code);
        code.pop_back();
        code.push_back(1);
        checkup_node(curr_node->right_, code);
    }
}

bool paimon::huffman::is_leaf(std::shared_ptr<paimon::node> curr_node)
{
    return (curr_node->left_ == nullptr) && (curr_node->right_ == nullptr);
}

void paimon::huffman::lookup_table_show()
{
    for (auto itr = lookup_table.begin(); itr != lookup_table.end(); itr++)
    {
        // Print code
        for (auto bit_itr = itr->first.begin(); bit_itr != itr->first.end(); bit_itr++)
        {
            std::cout << *bit_itr;
        }
        // Print character
        if (itr->second == '\0')
        {
            std::cout << " - EOF" << std::endl;
        }
        else if (itr->second == ' ')
        {
            std::cout << " - SPACE" << std::endl;
        }
        else if (itr->second == '\n')
        {
            std::cout << " - NEWLINE" << std::endl;
        }
        else
        {
            std::cout << " - " << itr->second << std::endl;
        }
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
    output.open("data/data.bin", std::ios::binary);

    while (input >> std::noskipws >> key)
    {
        auto itr = char_table.find(key);
        fetch_and_flush(itr, &cur_byte, &bit_count, &output);
    }

    // if file not terminated with EOF, add itself
    if (key != '\0')
    {
        auto itr = char_table.find('\0');
        fetch_and_flush(itr, &cur_byte, &bit_count, &output);
    }

    if (bit_count > 0)
    {
        // pad the last byte with zeroes
        cur_byte <<= 8 - bit_count;
        output.write(&cur_byte, sizeof(cur_byte));
    }

    output.close();
}

void paimon::huffman::fetch_and_flush(std::unordered_map<char, std::vector<bool>>::iterator itr, char* cur_byte,
                                      int* bit_count, std::ofstream* output)
{
        for (auto bit_itr = itr->second.begin(); bit_itr != itr->second.end(); bit_itr++)
        {
            if (*bit_count == 8)
            {
                output->write(cur_byte, sizeof(*cur_byte));
                *bit_count = 0;
                *cur_byte = 0;
            }

            *cur_byte <<= 1;
            *cur_byte |= *bit_itr;
            (*bit_count)++;
        }
}

void paimon::huffman::decompress_file(std::string compressed_file)
{

    std::ifstream input(compressed_file);
    std::vector<unsigned char> stream(std::istreambuf_iterator<char>(input), {});

    std::ofstream output;
    output.open("data/uncompressed.txt");

    std::vector<bool> code;
    for (std::size_t i = 0; i < stream.size(); i++)
    {
        std::bitset<8> bits(stream[i]);
        std::string byte = bits.to_string();
        for (auto itr  = byte.begin(); itr != byte.end(); itr++)
        {
            decode(itr, &code, &input, &output);
        }
    }

}


void paimon::huffman::decode(std::string::iterator itr, std::vector<bool>* code, std::ifstream* input, std::ofstream* output)
{
    code->push_back(*itr - 48);
    auto look_itr = lookup_table.find(*code);
    if (look_itr != lookup_table.end())
    {
        if (look_itr->second == '\0')
        {
            output->close();
            input->close();
            return;
        }
        else
        {
            *output << look_itr->second;
            code->clear();
        }
    }
}
