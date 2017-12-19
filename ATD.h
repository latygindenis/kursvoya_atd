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
    int SizeOfBlock = 10;
    int ReserveSpaceInBlock = SizeOfBlock / 10;
    int AmountOfBlock = 1;
public:
    void setAmountOfBlock(int AmountOfBlock);

public:
    ATD();
    void add_note(int note);
    int findBlockForInsert(fstream &fl, float key);
    void show_all_note();
    void moveNotes(int c, long long placeInsert);
    void generateBlock (fstream &fl); //Добавление пустого блока в конец
    void rebaseThisBlock( fstream &fl);
};


#endif //KURSVOYA_ATD_ATD_H
