//
// Created by denis on 17.12.17.
//

#ifndef KURSVOYA_ATD_ATD_H
#define KURSVOYA_ATD_ATD_H

#include <iostream>
#include <fstream>
#include <unistd.h>
#include <string>
#include <cmath>
#include "Keynote.h"
#include <random>
#include <cstdlib>

using namespace std;

class ATD {
    const char* NOTES_FILE = "notes.bin";
    const char* INDEX_FILE = "index.bin";
    int SizeOfBlock = 1000;
public:
    int getSizeOfBlock() const;

private:
    int AmountOfBlock = 1;
public:
    int getAmountOfBlock() const;

private:


    long long findBlockForInsert(fstream &fl, double key); //Поиск блока для вставки записи
    void deleteEmptyBlock(fstream &fl, long long beginEmptyBlock);
    void moveNotes(int c, long long placeInsert); //Смещение записей в блоке
    void generateBlock (fstream &fl); //Добавление пустого блока в конец
    void rebaseThisBlock(fstream &fl, long long CurrentBlock); //Перестройка индексного файла
    long long binaryBlockSearch(fstream &fl, long long CurrentBlock, double key); //
    long long int findBlockforFind(fstream &fl, double key); //Поиск блока для поиска
    void moveBlockRight(fstream &fl, long long CurrentBlock);

public:
    ATD();
    void add_note(uniform_real_distribution<double> urd, mt19937 &gen, int note, double myKey = -1); //Добавление записи в АТД
    void show_all_note(); //Печать всех записей в индексном файле и значений в файле записей
    int findValueByKey(double key);
    void deleteValueByKey(double key);

};


#endif //KURSVOYA_ATD_ATD_H
