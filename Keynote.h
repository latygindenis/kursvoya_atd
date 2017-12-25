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
public:
    void setKey(float key) {
        Keynote::key = key;
    }

private:
    long long point;


public:
    Keynote & operator=(Keynote &rightKeynote){
        if (&rightKeynote == this) return *this;//Проверка на самоприсваивание
        key= rightKeynote.getKey();
        point = rightKeynote.point;
        return *this;
    }
    void setNewRandomKey(uniform_real_distribution<float> urd, mt19937 &gen)
    {
        this->key = urd(gen);
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
