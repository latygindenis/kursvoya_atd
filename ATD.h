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
#include <math.h>


using namespace std;
class ATD {

    const char* NOTES_FILE = "notes.bin";
    const char* INDEX_FILE = "index.bin";
    int SizeOfBlock = 10;
    int ReserveSpaceInBlock = SizeOfBlock / 10;

public:
    int AmountOfBlock = 1;
    void setAmountOfBlock(int AmountOfBlock);

public:
    ATD();
    void add_note(int note); //Добавление записи в АТД
    long long findBlockForInsert(fstream &fl, float key); //Поиск блока для вставки записи
    void show_all_note(); //Печать всех записей в индексном файле и значений в файле записей
    void moveNotes(int c, long long placeInsert); //Смещение записей в блоке
    void generateBlock (fstream &fl); //Добавление пустого блока в конец
    void rebaseThisBlock(fstream &fl, long long CurrentBlock); //Перестройка индексного файла
    int findByKey (float key);
};


#endif //KURSVOYA_ATD_ATD_H
