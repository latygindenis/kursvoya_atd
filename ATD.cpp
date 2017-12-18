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

void ATD::moveNotes(int c, long long placeInsert){ //бинарный поиск по блоку

    Keynote kpr, buf;
    fstream iout(INDEX_FILE, ios::binary | ios::in | ios::out);
    iout.seekg(placeInsert, ios::beg);
    iout.read((char*)&buf, sizeof(Keynote));
    for(int j=c; j<SizeOfBlock-1; j++) { //смещаем все записи (начиная с места вставки)
        iout.read((char*)&kpr, sizeof(Keynote));
        iout.seekp(-sizeof(Keynote), ios::cur);
        iout.write((char*)&buf, sizeof(Keynote));
        buf = kpr;
    }
    iout.close();
}

ATD::ATD() {
    ifstream notes_in(NOTES_FILE, ios::binary);

    if (access(INDEX_FILE, 0) == 0) // проверка на наличие файла индексов
    {
        cout<<"File of index exist"<<endl;
    } else{
        cout<<"File of index not exist"<<endl;
        Keynote trash(-1, -1);
        ofstream index_out(INDEX_FILE, ios::binary | ios::app);// создание бинарного файла индексов
        for (int i=0; i<SizeOfBlock; i++)
        {
            cout<<index_out.tellp()<<endl;
            index_out.write((char*)&trash, sizeof(Keynote));
        }
        index_out.close();
    }
    notes_in.close();
}

void ATD::add_note(int note) {

    Keynote buf;
    int size = sizeof(note);
    long long point;
    float buf_key;
    ofstream note_out(NOTES_FILE, ios::binary | ios::app);
    fstream index_out(INDEX_FILE, ios::binary | ios::in | ios::out);

    point = note_out.tellp();
    cout<<"Point: "<<point<<endl;
    cout<<note<<endl;

    note_out.write((char*)&size, sizeof(int));
    note_out.write((char*)&note, size);

    note_out.close();

    Keynote newNote(point);
    cout<<newNote.getKey()<<endl;
    long long L=0;
    long long R=SizeOfBlock;
    long long mid = R/2;
    while (L < R)
    {
        index_out.seekg(mid*sizeof(Keynote), ios::beg);
        index_out.read((char*)&buf, sizeof(Keynote));
        cout<<"buf_key "<<buf.getKey()<<endl;
        cout<<"mid "<<mid<<endl;

        if (buf.getKey() == -1 or  buf.getKey() > newNote.getKey())
        {
            R = mid;
            mid = (R+L)/2;

        }
        if (buf.getKey() < newNote.getKey() && buf.getKey() != -1)
        {

            L = mid + 1;
            mid = (R + L)/2;
        }
    }

    cout<<L<<endl;
    cout<<R<<endl;
    cout<<mid<<endl;
    index_out.seekp(L*sizeof(Keynote) | ios::beg);

    index_out.read((char*)&buf, sizeof(Keynote));

    if (buf.getKey() != -1)
    {
        moveNotes(L, L*sizeof(Keynote));
    }

    index_out.seekp(L*sizeof(Keynote) | ios::beg);
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

    Keynote mysmallnote;

while ( index_out.read((char*)&mysmallnote, sizeof(Keynote)))
{
    cout<<index_out.tellg()<<" ";
    mysmallnote.print();

    point = mysmallnote.getPoint();
    if (point>=0)
    {
        note_out.seekg(point);
        note_out.read((char*)&size , sizeof(int));
        note_out.read((char*)&myNote, size);
        cout<<myNote<<endl;
    }
}
    index_out.close();
    note_out.close();
};
