# Huffman Tree Processing Tool
## Overview
The Huffman Tree Processing Tool (huffman.exe) is a command-line utility written in C that facilitates Huffman coding and decoding. The tool supports three main modes: Build Tree, Encode, and Decode.

## Features
### 1. Build Tree Mode
In this mode, the tool constructs a Huffman tree based on the weights of input characters. The user provides a file containing character weights, and the resulting model is saved to a specified output file. The tool outputs the size of the code table and the Weighted Path Length (WPL) on the screen.

**Usage:**

```bash
# format of input file
A 0.03
B 0.028
C 0.035
```

```bash
# order in terminal
./huffman.exe -m c -i input_file -o model_file
```

### 2. Encode Mode
The Encode mode allows users to encode text using a pre-built Huffman tree model. Users provide a text file in .txt format with a single row without a line break, the Huffman model file, and the tool outputs the encoded result to a specified file.

**Usage:**

```bash
./huffman.exe -m e -i input_file -h model_file -o output_file
```

### 3. Decode Mode
In Decode mode, the tool takes an encoded file and the Huffman code file, then decodes the input and outputs the result to a specified file in .txt format.

**Usage:**

```bash
./huffman.exe -m d -i input_file -h code_file -o output_file
```

## Usage Examples
### Build Tree Mode
```bash
./huffman.exe -m c -i weight.txt -o model.txt
```

### Encode Mode
```bash
./huffman.exe -m e -i origin.txt -h model.txt -o encode.txt
```

### Decode Mode
```bash
./huffman.exe -m d -i encode.txt -h model.txt -o decode.txt
```

## Dependencies
The tool is written in C and does not have external dependencies, orders should be input in terminal in the same direction.

## Compilation
To compile the tool, use the provided Makefile or any C compiler, such as GCC.

To compile your huffman.c program in linux environment with provided Makefile, open the terminal in the same directory and run:

```bash
make
```
This will create an executable named huffman in the same directory. If you want to clean up and remove the executable, you can run:
```bash
make clean
```
Make sure to adjust the CFLAGS variable if you have additional compilation flags you'd like to include.

## Contributing
Feel free to contribute by opening issues or submitting pull requests on the GitHub repository.

## License
This Huffman Tree Processing Tool is an open-source.
