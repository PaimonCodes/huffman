# Huffman
Developed by Dr. David A. Huffman in 1952, Huffman encoding is one of the oldest lossless compression algorithm to be developed. Although surpassed by modern
compression algorithms such as LZ77 and LZ78, Huffman's encoding opened the way for the development of more efficient compression algorithms that we
may have taken for granted nowadays.

## Overview
https://user-images.githubusercontent.com/104661175/210291487-78a44589-ba2c-4aa4-b3e1-7fc54efeccb7.mp4

## Usages
1. Select a text file by `Browse` or typing the text's file path.

<p align="center">
  <img src="https://user-images.githubusercontent.com/104661175/211097680-e8ba43f8-65c8-4742-8416-73aa5b8a5705.png">
</p>

2. Compress. Compressing creates a `<huffman root>/data` directory with a `data.bin` file. Note that the last byte is padded with zeroes
because it is not possible to write into a file bit by bit.

<p align="center">
  <img src="https://user-images.githubusercontent.com/104661175/211127628-5ddf9142-f62e-40ca-8235-c62a007ff05a.png">
</p>

3. Decompress. Decompressing creates an `uncompressed.txt` file in the `<huffman root>/data` directory.

<p align="center">
  <img src="https://user-images.githubusercontent.com/104661175/211127692-6c9eb0d2-acb6-447b-95eb-f1766e6b9efd.png">
</p>

## Tools
Built with:
- C++
- QT5

----

### Read more:
https://web.stanford.edu/class/archive/cs/cs106b/cs106b.1126/handouts/220%20Huffman%20Encoding.pdf

## License
Licensed under [MIT](/license) license.

