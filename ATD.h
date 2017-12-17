//
// Created by denis on 17.12.17.
//

#ifndef KURSVOYA_ATD_ATD_H
#define KURSVOYA_ATD_ATD_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <ctime>


using namespace std;
class ATD {

    const char* NOTES_FILE = "notes.bin";
    const char* INDEX_FILE = "index.bin";
    int SizeOfBlock = 10000;
    int FreeSpaceInBlock = SizeOfBlock / 10;
    int AmountOfBlock = 1;

public:
    ATD();
    void add_note(void  *note);
    void show_all_note();

};


#endif //KURSVOYA_ATD_ATD_H
