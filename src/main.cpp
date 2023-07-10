#include <stdexcept>
#include <iostream>
#include <stdio.h>
#include <string.h>


int main(int argc,char *argv[]) {

    if(argc < 2)
		throw std::runtime_error("ERROR: Argumentos inválidos.\n");

    const char* input = argv[1];


    printf("Finished Huffman encoding/decoding execution.\n");
}