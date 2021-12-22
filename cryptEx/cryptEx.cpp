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
    ifn.close();

    int start_of_function = seek_label(data, 0x11, 0) + 5;
    int end_of_function = seek_label(data, 0x22, start_of_function);
    int length_of_function = end_of_function - start_of_function;

    RC5_Init();

    for (int i = 0; i < length_of_function; i += 2 * sizeof(DWORD))
    {
        //3348140176    8115269
        DWORD buffer[2] = { *(DWORD*)&data[start_of_function + i], *(DWORD*)&data[start_of_function + i + sizeof(DWORD)]};
        RC5_encrypt(buffer);
        //2672322386    348134176
        std::memcpy(&data[start_of_function + i], &buffer, sizeof(buffer));

    }

    std::ofstream ofn(fileName, std::ios::binary);
    int s = data.size();
    ofn.write((char*)&data[0], data.size());
    printf("%s encrypted.\n", fileName);
    system("pause");
    /*
    std::vector<unsigned char> _data(10240);
    std::ifstream _ifn(fileName, std::ios::binary);
    if (!_ifn)
    {
        printf("Couldn't open file %s\n", fileName);
        exit(-1);
    }
    _ifn.seekg(0, std::ios::end);
    int _fileSize = _ifn.tellg();
    _data.resize(_fileSize);
    _ifn.seekg(0, std::ios::beg);
    _ifn.read((char*)&_data[0], _fileSize);
    _ifn.close();

    int _start_of_function = seek_label(_data, 0x11, 0) + 5;
    int _end_of_function = seek_label(_data, 0x22, _start_of_function);
    int _length_of_function = _end_of_function - _start_of_function;
    */
    return 0;

}

