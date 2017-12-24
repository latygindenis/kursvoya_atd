//
// Created by denis on 17.12.17.
//

#ifndef KURSVOYA_ATD_KEYNOTE_H
#define KURSVOYA_ATD_KEYNOTE_H

#include <cstdlib>
#include <iostream>
#include <random>
#include <cstdlib>



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

    Keynote (uniform_real_distribution<float> urd, mt19937 &gen, long long point)
    {
        this->key = urd(gen);
        this->point = point;
        cout<<"Gener key: "<<this->key<<endl;
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
