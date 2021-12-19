#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>


int seek_label(std::vector<unsigned char> data, int label, int offset)
{
    for (unsigned int i = offset; i < data.size() - 5; i++)
        if (data[i] == 0xb8
            && data[i + 1] == label
            && data[i + 2] == label
            && data[i + 3] == label
            && data[i + 4] == label
            )
            return i;
    return -1;
}

int main(int argc, char **argv)
{
    std::vector<unsigned char> data(10240);
    std::ifstream ifn(argc > 0 ? argv[1] : "bpo4.exe", std::ios::binary);
    ifn.seekg(0, std::ios::end);
    int fileSize = ifn.tellg();
    data.resize(fileSize);
    ifn.seekg(0, std::ios::beg);
    ifn.read((char*)&data[0], fileSize);

    int start_of_function = seek_label(data, 0x11, 0);
    int end_of_function = seek_label(data, 0x22, start_of_function);
    int length_of_function = end_of_function - start_of_function;
    for (int i = 0; i < length_of_function; i++)
        data[start_of_function + i]

}

