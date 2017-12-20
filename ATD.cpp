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

void ATD::moveNotes(int c, long long placeInsert){ //Смещение записей

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
        fstream index_rd(INDEX_FILE, ios::binary | ios::out | ios::app );
        AmountOfBlock = int (index_rd.tellg() / (sizeof(Keynote) * SizeOfBlock));
        cout<<"File of index exist"<<endl;
        index_rd.close();

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
    long long BlockForInsert = findBlockForInsert(index_out, newNote.getKey());
    cout<<"Block for insert: "<<BlockForInsert<<endl;
    cout<<newNote.getKey()<<endl;

    index_out.seekp((SizeOfBlock - 1 )* sizeof(Keynote) + BlockForInsert ); //В конец блока
    index_out.read((char*)&buf, sizeof(Keynote)); //Считываем последнюю запись

    if (buf.getKey()!=-1)//проверяем мусор запись или нет
    {
        cout<<"End of block"<<endl;
        generateBlock(index_out);
        rebaseThisBlock(index_out, BlockForInsert);
        show_all_note();
    }
    cout<<"dddd"<<endl;

    BlockForInsert = findBlockForInsert(index_out, newNote.getKey());

    cout<<"dddd"<<endl;

    cout<<"Block for insert: "<<BlockForInsert<<endl;
    index_out.seekp(BlockForInsert, ios::beg);//Начало блока вставки



    long long L=0;
    long long R=SizeOfBlock;
    long long mid = R/2;
    while (L < R) //бинарный поиск по блоку
    {
        cout<<1<<endl;
        index_out.seekg(mid*sizeof(Keynote) + BlockForInsert, ios::beg);
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
    index_out.seekp(L*sizeof(Keynote) + BlockForInsert| ios::beg);

    index_out.read((char*)&buf, sizeof(Keynote));

    if (buf.getKey() != -1)
    {
        moveNotes(L, L*sizeof(Keynote) + BlockForInsert);
    }

    index_out.seekp(L*sizeof(Keynote) + BlockForInsert | ios::beg);
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

while (index_out.read((char*)&mysmallnote, sizeof(Keynote)))
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
}

void ATD::setAmountOfBlock(int AmountOfBlock) {
    ATD::AmountOfBlock = AmountOfBlock;
}

void ATD::generateBlock(fstream &fl) {
    fl.seekp(SizeOfBlock*sizeof(Keynote)*AmountOfBlock);
    this->AmountOfBlock++;

    Keynote trash(-1, -1);
    for (int i=0; i<SizeOfBlock; i++)
    {
        cout<<fl.tellp()<<endl;
        fl.write((char*)&trash, sizeof(Keynote));
    }
}

void ATD::rebaseThisBlock(fstream &fl, long long CurrentBlock) {

    cout<<"#1"<<endl;
    Keynote buf, buf2, trash (-1, -1);
    long long CurBlockReabase;
    long long NextBlockReabase;
    long long OtherBlockRebase;


    fl.seekp(0, ios::end);
    NextBlockReabase = fl.tellp() - SizeOfBlock * sizeof(Keynote); //Указатель на начало пустого блока
    OtherBlockRebase = NextBlockReabase - SizeOfBlock*sizeof(Keynote);

    if (OtherBlockRebase == CurrentBlock)
    {
        cout<<"#2"<<endl;
        fl.seekp(CurrentBlock + sizeof(Keynote)*(SizeOfBlock/2), ios::beg);
        CurBlockReabase = fl.tellp(); //указатель на перемещаемый блок


        for (int i=0; i<SizeOfBlock/2; i++)
        {

            fl.seekp(CurBlockReabase, ios::beg);
            fl.read((char*)&buf, sizeof (Keynote));
            cout<<"For"<<endl;
            cout<<i<<" "<<buf.getKey()<<" "<<NextBlockReabase;
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.write((char*)&trash, sizeof (Keynote));
            CurBlockReabase += sizeof(Keynote);

            fl.seekp(NextBlockReabase, ios::beg);
            fl.write((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.read((char*)&buf2, sizeof (Keynote));
            cout<<" "<<buf2.getKey()<<endl;
            NextBlockReabase += sizeof(Keynote);
        }
    }
    else{
        cout<<"#3"<<endl;
        while (OtherBlockRebase != CurrentBlock)
        {
            cout<<"While"<<endl;
            for (int i = 0; i<SizeOfBlock; i++)
            {
                fl.seekp(OtherBlockRebase, ios::beg);
                fl.read((char*)&buf, sizeof (Keynote));
                fl.seekp(-sizeof(Keynote), ios::cur);
                fl.write((char*)&trash, sizeof (Keynote));
                OtherBlockRebase += sizeof(Keynote);

                fl.seekp(NextBlockReabase, ios::beg);
                fl.write((char*)&buf, sizeof (Keynote));
                NextBlockReabase += sizeof(Keynote);

            }
            cout<<OtherBlockRebase<<endl;
            cout<<NextBlockReabase<<endl;
            getchar();
            OtherBlockRebase -= sizeof(Keynote)*SizeOfBlock*2;
            NextBlockReabase -= sizeof(Keynote)*SizeOfBlock*2;
            cout<<OtherBlockRebase<<endl;
            cout<<NextBlockReabase<<endl;
            getchar();
        }

        fl.seekp(CurrentBlock + sizeof(Keynote)*(SizeOfBlock/2), ios::beg);
        CurBlockReabase = fl.tellp(); //указатель на перемещаемый блок

        for (int i=0; i<SizeOfBlock/2; i++)
        {
            fl.seekp(CurBlockReabase, ios::beg);
            fl.read((char*)&buf, sizeof (Keynote));
            cout<<i<<" "<<buf.getKey()<<" "<<NextBlockReabase<<endl;
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.write((char*)&trash, sizeof (Keynote));
            CurBlockReabase += sizeof(Keynote);

            fl.seekp(NextBlockReabase, ios::beg);
            fl.write((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.read((char*)&buf2, sizeof (Keynote));
            cout<<" "<<buf2.getKey()<<endl;
            NextBlockReabase += sizeof(Keynote);
        }


    }
}

long long ATD::findBlockForInsert(fstream &fl, float key) { //Возвращает указатель на начало нужного блока
    Keynote buf;
    int L=0, R = AmountOfBlock - 1, mid=(L+R)/2;

    if (L==R) { return 0;} //Если один блок
    while (L < R)
    {
        cout<<1<<endl;
        fl.seekp(mid * SizeOfBlock * sizeof(Keynote), ios::beg);
        fl.read((char*)&buf, sizeof (Keynote));

        if (buf.getKey() > key)
        {
            R = mid - 1;
            mid = (L + R)/2;
        }
        if ( buf.getKey() < key)
        {
            L = mid + 1;
            mid = (R + L)/2;
        }

    }
    if (L<0)
    {
        return 0;
    }
    fl.seekp(L * SizeOfBlock * sizeof(Keynote), ios::beg);
    fl.read((char*)&buf, sizeof (Keynote));
    if (buf.getKey() > key && L >0)
    {
        L--;
    }
    cout<<"L "<<L<<endl;

    return L * SizeOfBlock * sizeof(Keynote);
};
