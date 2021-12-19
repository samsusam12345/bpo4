#include <vector>
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <windows.h>
#include "RC5.h"


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
    setlocale(LC_ALL, "Russian");
    std::vector<unsigned char> data(10240);
    const char* fileName = argc > 1 ? argv[1] : "bpo4.exe";
    std::ifstream ifn(fileName, std::ios::binary);
    if (!ifn)
    {
        printf("Couldn't open file %s\n", fileName);
        exit(-1);
        }
    ifn.seekg(0, std::ios::end);
    int fileSize = ifn.tellg();
    data.resize(fileSize);
    ifn.seekg(0, std::ios::beg);
    ifn.read((char*)&data[0], fileSize);

    int start_of_function = seek_label(data, 0x11, 0) + 5;
    int end_of_function = seek_label(data, 0x22, start_of_function);
    int length_of_function = end_of_function - start_of_function;

    for (int i = 0; i < length_of_function; i += 2 * sizeof(DWORD))
    {
        DWORD buffer[2] = { *(DWORD*)&data + start_of_function + i, *(DWORD*)&data + start_of_function + i * sizeof(DWORD)};
        RC5_encrypt(buffer);
        for (int j = 0; j < 8; j++)
        {
            unsigned char byte = *(unsigned char*)&buffer + j;
            data[start_of_function + j] = byte;
        }
    }

    std::ofstream ofn(fileName, std::fstream::binary);
    ofn.write(reinterpret_cast<const char*>(&data[0]), data.size() * sizeof(char));
    printf("%s encrypted.\n", fileName);
    return 0;

}

