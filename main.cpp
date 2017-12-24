#include <iostream>
#include "ATD.h"


int main() {

    mt19937 gen(static_cast<unsigned long>(time(nullptr)));

    ATD myBase;
    int a=999, b=875; //0.0774815, 0.479748
    uniform_real_distribution<float> urd(0, 100);
//    for (int i=0; i<1000; i++)
//    {
//        myBase.add_note(urd, gen, 785);
//    }
    myBase.show_all_note();
    myBase.findValueByKey(0.353104 );
    return 0;
}