//
// Created by denis on 17.12.17.
//

#ifndef KURSVOYA_ATD_KEYNOTE_H
#define KURSVOYA_ATD_KEYNOTE_H

#include <cstdlib>
#include <iostream>
#include <random>

using namespace std;


class Keynote {
    float key;
    long long point;


public:
    Keynote & operator=(Keynote &rightKeynote){
        if (&rightKeynote == this) return *this;//Проверка на самоприсваивание
        key= rightKeynote.getKey();
        point = rightKeynote.point;
        return *this;
    }

    float getKey() {
        return key;
    }

    long long int getPoint() const {
        return point;
    }

    Keynote (long long point)
    {
        mt19937 gen(time(0));
        uniform_real_distribution<float> urd(0, 5); //Генерация случайных вещественных чисел
        this->key = urd(gen);
        this->point = point;
    }
    Keynote (float key, long long point)
    {
        this->key = key;
        this->point = point;
    }
    Keynote ()
    {

    }
    void print()
    {
        cout<<"key: " << this->key << " point: "<<point;
    }
};


#endif //KURSVOYA_ATD_KEYNOTE_H
