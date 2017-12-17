//
// Created by denis on 17.12.17.
//

#include "ATD.h"
#include "Keynote.h"

float randomfloat()
{
    float r2 = static_cast <float> (rand()) / (static_cast <float> (RAND_MAX));
    return r2;
}

ATD::ATD() {
    ifstream notes_in(NOTES_FILE, ios::binary);

    if (access(INDEX_FILE, 0) == 0) // проверка на наличие файла индексов
    {
        cout<<"File of index exist"<<endl;
    } else{
        cout<<"File of index not exist"<<endl;
        ofstream index_out(INDEX_FILE, ios::binary | ios::app);// создание бинарного файла индексов
        index_out.close();
    }
    notes_in.close();

}

void ATD::add_note(void *note) {

    int size = sizeof(note);
    long long point;

    fstream note_out(NOTES_FILE, ios::binary | ios::app);
    ofstream index_out(INDEX_FILE, ios::binary | ios::app);

    point = note_out.tellg();

    cout<<"Point: "<<point<<endl;
    note_out.write((char*)&size, sizeof(int));
    note_out.write((char*)note, size);
    note_out.close();

    Keynote newNote(point);
    index_out.write((char*)&newNote, sizeof(Keynote));
    index_out.close();

}
void ATD::show_all_note(){
    ifstream index_out(INDEX_FILE, ios::binary);
    ifstream note_out (NOTES_FILE, ios::binary);
    int size;
    int myNote;
    long long point;
    note_out.seekg (0, ios::beg);
    index_out.seekg (0, ios::beg);

while (!index_out.eof())
{
    Keynote mysmallnote;
    index_out.read((char*)&mysmallnote, sizeof(Keynote));
    mysmallnote.print();
    point = mysmallnote.getPoint();

    note_out.seekg(point);
    note_out.read((char*)&size , sizeof(int));
    note_out.read((char*)&myNote, size);

    cout<<myNote<<endl;
}
};
