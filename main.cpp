#include <iostream>
#include "ATD.h"

int main() {
    srand(static_cast<unsigned int>(time(NULL)));
    ATD myBase;
    int a=999, b=875;
    myBase.add_note(a);
    myBase.show_all_note();
    return 0;
}