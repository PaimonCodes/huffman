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
    std::set<std::pair<int, std::shared_ptr<paimon::node>>> tree_collection;
    hfman.create_tree_collection(input_file, &tree_collection);

    // create tree
    hfman.huffman_tree(&tree_collection);

    // create lookup table
    hfman.create_lookup_table();

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
    // Greedy algorithm - https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/handouts/220%20Huffman%20Encoding.pdf

    auto itr = tree_collection->begin();
    while (itr != tree_collection->end())
    {
        if (std::next(itr, 1) != tree_collection->end())
        {
            itr = fetch_two(tree_collection);
        }
        else
        {
            root_ = itr->second;
            itr = tree_collection->erase(itr);
        }
    }

    auto itr_f = tree_collection->begin();
    root_ = itr_f->second;
    
    /*
    for (auto itr1 = tree_collection->begin(); itr1 != tree_collection->end(); itr1++)
    {
        std::cout << itr1->first << ", ";
    }
    std::cout << std::endl;
    std::cout << root_->freq_ << std::endl;
*/
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
    std::cout << root_->freq_ << std::endl;
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
        std::cout << *(curr_node->char_) << std::endl;
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
        for (auto bit_itr = itr->first.begin(); bit_itr != itr->first.end(); bit_itr++)
        {
            std::cout << *bit_itr;
        }
        std::cout << " - " << itr->second << std::endl;
    }
}