#include <iostream>
#include "ATD.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));



    ATD myBase;
    int a=999, b=875; //0.0774815, 0.479748

    myBase.add_note(785);
    myBase.show_all_note();
    myBase.findValueByKey(0.274076);
    myBase.deleteValueByKey(0.274076);

    return 0;
}