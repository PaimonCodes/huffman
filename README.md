# Huffman

<p align="center">
    <img src="https://ieeecs-media.computer.org/wp-media/2018/04/11204517/davidhuffman-wm-e1523479537419.jpg"> 
</p>

Developed by Dr. David A. Huffman in 1952, Huffman encoding is one of the oldest lossless compression algorithm to be developed. Although surpassed by modern
compression algorithms such as LZ77 and LZ78, Huffman's encoding opened the way for the development of more efficient compression algorithms that we
may have taken for granted nowadays. This library reinvisions Huffman's algorithm in C++.

## Overview
https://user-images.githubusercontent.com/104661175/210291487-78a44589-ba2c-4aa4-b3e1-7fc54efeccb7.mp4

## Tree
A set of subtree initially stores single nodes of characters with each of a single subtree. Two trees with root nodes of minimum frequencies are fetched and their frequencies are
summed into a new parent node. The two trees becomes the subtree of the new parent node. 
The two trees are removed from the set of trees and the new parent node is reinserted into the set. A new pair of trees with minimum root node frequencies
are fetched again. The cycle repeats until there is only one remaining tree in the set. This remaining tree is the complete Huffman Tree.

## Pseudo EOF
In practical terms, it is impossible to know where the end of file is when decoding a Huffman compressed file. Therefore, a pseudo eof of zero-byte '/0'
is added to be encoded in the tree.

## Encoding
The file is read character by character. The character is matched from a character table and its corresponding bit codes is written on a bin file.
One of the important things to note is that it is not allowed to write bit by bit in a file. Therefore, all bits must be first packed into bytes before
being flushed to the output file.

## Decoding
C++'s std::bitset comes in handy when reading byte per byte of the compressed file. These bytes are read and converted to a string, where each bit can
then be pushed into a temporary vector of bool that can be used to match to the lookup table. Although it can be argued that decoding can be done by
traversing the tree, in practical terms, compressors don't generally store a whole tree. It is more ideal to store a lookup table itself. Furthermore,
lookup tables allow for a future project with the compressed file storing the table in it.

#### Read more:
https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/handouts/220%20Huffman%20Encoding.pdf







