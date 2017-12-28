#include <iostream>
#include "ATD.h"


int main() {

    mt19937 gen(static_cast<unsigned long>(time(nullptr)));
    ATD myBase;
    int note, amountOfNotes, defaultNote = 145;
    float key;
    uniform_real_distribution<float> urd(0.0, 1000.0);
    int var;
    do
    {
        cout<<"Amount of blocks: "<<myBase.getAmountOfBlock()<<"; Size of Block: "<<myBase.getSizeOfBlock()<<endl;
        cout<<"Add one note(1)"<<endl;
        cout<<"Find note by key(2)"<<endl;
        cout<<"Delete note by key(3)"<<endl;
        cout<<"Show all note(4)"<<endl;
        cout<<"Add N notes(5)"<<endl;
        cout<<"Add note with own key(6)"<<endl;
        cout<<"Exit(0)"<<endl;
        cin>>var;
        switch (var)
        {
            case 1:
                cout<<"Enter note: ";
                cin>>note;
                myBase.add_note(urd, gen, note);
                break;

            case 2:
                cout<<"Enter key: ";
                cin>>key;
                myBase.findValueByKey(key);
                break;

            case 3:
                cout<<"Enter key: ";
                cin>>key;
                myBase.deleteValueByKey(key);
                break;

            case 4:
                myBase.show_all_note();
                break;

            case 5:
                cout<<"Enter amount of notes: ";
                cin>>amountOfNotes;
                for(int i=0; i<amountOfNotes; i++)
                {
                    if (i%100000==0)
                    {
                        cout<<i<<endl;
                    }
                    myBase.add_note(urd, gen, defaultNote);
                }
                break;
            case 6:
                cout<<"Enter key: ";
                cin>>key;
                cout<<"Enter note: ";
                cin>>note;
                myBase.add_note(urd, gen, note, key);
                break;
            default:break;
        }

    }       while (var);

    return 0;
}