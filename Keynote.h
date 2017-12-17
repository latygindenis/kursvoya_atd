//
// Created by denis on 17.12.17.
//

#ifndef KURSVOYA_ATD_KEYNOTE_H
#define KURSVOYA_ATD_KEYNOTE_H
#include <ctime>
#include <cstdlib>
#include <iostream>

using namespace std;


class Keynote {
    float key;
    long long point;
public:
    long long int getPoint() const {
        return point;
    }

public:
    float randomfloat()
    {
        float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
        return r2;
    }
    Keynote (long long point)
    {
        this->key = randomfloat();
        this->point = point;
    }
    Keynote ()
    {
    }
    void print()
    {
        cout<<"key: " << this->key << " point: "<<point<<endl;
    }

};


#endif //KURSVOYA_ATD_KEYNOTE_H
