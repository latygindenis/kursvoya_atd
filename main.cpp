#include <iostream>
#include "ATD.h"


int main() {

    mt19937 gen(static_cast<unsigned long>(time(nullptr)));

    ATD myBase;
    float  fff;
    int a=999, b=875; //0.0774815, 0.479748
    uniform_real_distribution<float> urd(0, 5);
//    for (int i=0; i<50; i++)
//    {
//        myBase.add_note(urd, gen, 785);
//    }
    myBase.show_all_note();

//    myBase.add_note(45);
//    myBase.show_all_note();
//    myBase.findValueByKey(2.02432);
    myBase.deleteValueByKey(1.78919);
//    myBase.show_all_note();
////    myBase.deleteValueByKey(0.274076);
    myBase.show_all_note();

    return 0;
}