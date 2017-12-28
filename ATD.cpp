//
// Created by denis on 17.12.17.
//

#include "ATD.h"

void ATD::moveNotes(int c, long long placeInsert){ //Смещение записей

    Keynote kpr, buf;
    char *SmallBlock = new char[sizeof(Keynote)*(SizeOfBlock-c-1)];
    fstream iout(INDEX_FILE, ios::binary | ios::in | ios::out);
    iout.seekg(placeInsert, ios::beg);

    iout.read(SmallBlock, sizeof(Keynote)*(SizeOfBlock-c-1));
    iout.seekg(placeInsert + sizeof(Keynote), ios::beg);
    iout.write(SmallBlock,sizeof(Keynote)*(SizeOfBlock-c-1) );
    delete SmallBlock;
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
            index_out.write((char*)&trash, sizeof(Keynote));
        }
        index_out.close();
    }
    notes_in.close();
}

void ATD::add_note(uniform_real_distribution<float> urd, mt19937 &gen,  int note, float myKey) {

    Keynote buf;
    int size = sizeof(note);
    long long point;
    ofstream note_out(NOTES_FILE, ios::binary | ios::app);
    fstream index_out(INDEX_FILE, ios::binary | ios::in | ios::out);
    point = note_out.tellp();
    note_out.write((char*)&size, sizeof(int));
    note_out.write((char*)&note, size);
    note_out.close();
    Keynote newNote(urd, gen, point);
    if (myKey > -1 )
    {
       newNote.setKey(myKey);
    }

    long long BlockForInsert = findBlockForInsert(index_out, newNote.getKey());
    index_out.seekp((SizeOfBlock - 1 )* sizeof(Keynote) + BlockForInsert ); //В конец блока
    index_out.read((char*)&buf, sizeof(Keynote)); //Считываем последнюю запись


    if (buf.getKey()!=-1)//проверяем мусор запись или нет
    {

        if (buf.getKey() < newNote.getKey() && index_out.tellp()<SizeOfBlock*AmountOfBlock* sizeof(Keynote))
        {
            BlockForInsert += sizeof(Keynote)*SizeOfBlock;
        } else{
            generateBlock(index_out);
            rebaseThisBlock(index_out, BlockForInsert);
            BlockForInsert = findBlockForInsert(index_out, newNote.getKey());
        }
    }

    index_out.seekp(BlockForInsert, ios::beg);//Начало блока вставки
    long long L=0;
    long long R=SizeOfBlock;
    long long mid = R/2;
    while (L < R) //бинарный поиск по блоку
    {
        index_out.seekg(mid*sizeof(Keynote) + BlockForInsert, ios::beg);
        index_out.read((char*)&buf, sizeof(Keynote));

        if (fabs(buf.getKey() - newNote.getKey()) < 0.000005) //Если зарандомился одинаковый ключ
        {
           // cout<<":("<<endl;
            newNote.setNewRandomKey(urd, gen);
            L=0;
            R = SizeOfBlock;
            mid = R/2;
            index_out.seekg(mid*sizeof(Keynote) + BlockForInsert, ios::beg);
            index_out.read((char*)&buf, sizeof(Keynote));
        }
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
    int design = 0;
while (index_out.read((char*)&mysmallnote, sizeof(Keynote)))
{
    if(design%SizeOfBlock == 0)
    {
        cout<<"-------------------------"<<endl;
    }


    cout<<index_out.tellg() - sizeof(Keynote)<<" ";
    mysmallnote.print();
    point = mysmallnote.getPoint();
    if (point>=0)
    {
        note_out.seekg(point);
        note_out.read((char*)&size , sizeof(int));
        note_out.read((char*)&myNote, size);
        cout<<" "<<myNote;
    }
    cout<<endl;
    design ++;
}
    cout<<"-------------------------"<<endl;
    index_out.close();
    note_out.close();
}

void ATD::generateBlock(fstream &fl) {
    fl.seekp(SizeOfBlock*sizeof(Keynote)*AmountOfBlock);
    this->AmountOfBlock++;

    Keynote trash(-1, -1);
    for (int i=0; i<SizeOfBlock; i++)
    {
        fl.write((char*)&trash, sizeof(Keynote));
    }
}

void ATD::rebaseThisBlock(fstream &fl, long long CurrentBlock) {
    Keynote buf, buf2, trash (-1, -1);
    long long CurBlockReabase;
    long long NextBlockReabase;
    long long OtherBlockRebase;


    fl.seekp(0, ios::end);
    NextBlockReabase = fl.tellp() - SizeOfBlock * sizeof(Keynote); //Указатель на начало пустого блока
    OtherBlockRebase = NextBlockReabase - SizeOfBlock*sizeof(Keynote);

    if (OtherBlockRebase == CurrentBlock)
    {
        fl.seekp(CurrentBlock + sizeof(Keynote)*(SizeOfBlock/2), ios::beg);
        CurBlockReabase = fl.tellp(); //указатель на перемещаемый блок

        for (int i=0; i<SizeOfBlock/2; i++)
        {

            fl.seekp(CurBlockReabase, ios::beg);
            fl.read((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);

            fl.write((char*)&trash, sizeof (Keynote));
            CurBlockReabase += sizeof(Keynote);

            fl.seekp(NextBlockReabase, ios::beg);
            fl.write((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.read((char*)&buf2, sizeof (Keynote));
            NextBlockReabase += sizeof(Keynote);
        }
    }
    else{
        while (OtherBlockRebase != CurrentBlock)
        {
            moveBlockRight(fl, OtherBlockRebase);
            OtherBlockRebase -= sizeof(Keynote)*SizeOfBlock;
            NextBlockReabase -= sizeof(Keynote)*SizeOfBlock;
        }

        fl.seekp(CurrentBlock + sizeof(Keynote)*(SizeOfBlock/2), ios::beg);

        CurBlockReabase = fl.tellp(); //указатель на перемещаемый блок



        for (int i=0; i<SizeOfBlock/2; i++)
        {
            fl.seekp(CurBlockReabase, ios::beg);
            fl.read((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.write((char*)&trash, sizeof (Keynote));
            CurBlockReabase += sizeof(Keynote);

            fl.seekp(NextBlockReabase, ios::beg);
            fl.write((char*)&buf, sizeof (Keynote));
            fl.seekp(-sizeof(Keynote), ios::cur);
            fl.read((char*)&buf2, sizeof (Keynote));
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

    return L * SizeOfBlock * sizeof(Keynote);
}
long long ATD::binaryBlockSearch(fstream &fl, long long CurrentBlock, float key) {
    Keynote buf;
    long long L=0;
    long long R=SizeOfBlock-1;
    long long mid = R/2;
    while (L < R) //бинарный поиск по блоку
    {
        fl.seekg(mid*sizeof(Keynote) + CurrentBlock, ios::beg);
        fl.read((char*)&buf, sizeof(Keynote));
        if (fabs(buf.getKey() - key) < 0.00005)
        {
            cout<<key<<endl;
            cout<<"key: "<<buf.getKey()<<" founded"<<endl;
            return fl.tellp() - sizeof(Keynote);

        }
        if (buf.getKey() == -1 or  buf.getKey() > key)
        {
            R = mid - 1;
            mid = (R+L)/2;
        }
        if (buf.getKey() < key && buf.getKey() != -1)
        {
            L = mid + 1;
            mid = (R + L)/2;
        }
    }
    fl.seekp(L*sizeof(Keynote)  + CurrentBlock, ios::beg);
    fl.read((char*)&buf, sizeof(Keynote));
    cout<<buf.getKey()<<endl;
    cout<<key<<endl;
    cout<<fabs(buf.getKey() - key)<<endl;

    if (fabs(buf.getKey() - key) <0.00005)
    {
        cout<<"base key:"<<key<<endl;
        cout<<"key: "<<buf.getKey()<<" founded"<<endl;
        return fl.tellp() - sizeof(Keynote);
    }
    else
    {
        cout<<"Not found key"<<endl;
    }
    return -1;
};

long long ATD::findBlockforFind(fstream &fl, float key)
{
    Keynote buf;
    int L=0, R = AmountOfBlock - 1, mid=(L+R)/2;

    if (L==R) { return 0;} //Если один блок
    while (L < R)
    {
        fl.seekp(mid * SizeOfBlock * sizeof(Keynote), ios::beg);
        fl.read((char*)&buf, sizeof (Keynote));

        if (fabs(buf.getKey() - key) < 0.00001)
        {
            L = mid;
            R = -1;
        }
        else if (buf.getKey() > key)
        {
            R = mid - 1;
            mid = (L + R)/2;
        }
        else if ( buf.getKey() < key)
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

    if (L < AmountOfBlock -1 or buf.getKey() - key >=  0.00005)
    {
        L--;
    }

    fl.seekp(L * SizeOfBlock * sizeof(Keynote), ios::beg);
    fl.read((char*)&buf, sizeof (Keynote));
    return L * SizeOfBlock * sizeof(Keynote);

}

int ATD::findValueByKey(float key) {
    Keynote buf;
    long long foundedPoint;
    fstream index_out(INDEX_FILE, ios::binary | ios::out |ios::in);
    fstream note_out (NOTES_FILE, ios::binary | ios::out |ios::in);

    long long findedBlock = findBlockforFind(index_out, key);
    cout<<"findedBlock: "<<findedBlock<<endl;

    long long L=0;
    long long R=SizeOfBlock-1;
    long long mid = R/2;
    while (L < R) //бинарный поиск по блоку
    {
        index_out.seekg(mid*sizeof(Keynote) + findedBlock, ios::beg);
        index_out.read((char*)&buf, sizeof(Keynote));
        if (fabs(buf.getKey() - key) < 0.00005)
        {
            int value;
            cout<<"Founded point: "<<index_out.tellp() - sizeof(Keynote)<<endl;
            cout<<"base key:"<<key<<endl;
            cout<<"key: "<<buf.getKey()<<" founded"<<endl;
            note_out.seekp(buf.getPoint());
            note_out.read((char*)&value, sizeof(int));
            note_out.read((char*)&value, sizeof(int));
            cout<<"Finded value: "<<value<<endl;
            note_out.close();
            index_out.close();
            return value;

        }
        if (buf.getKey() == -1 or  buf.getKey() > key)
        {
            R = mid - 1;
            mid = (R+L)/2;

        }
        if (buf.getKey() < key && buf.getKey() != -1)
        {

            L = mid + 1;
            mid = (R + L)/2;
        }
    }
    cout<<L<<endl;
    index_out.seekp(L*sizeof(Keynote)  + findedBlock, ios::beg);
    foundedPoint = index_out.tellp();
    index_out.read((char*)&buf, sizeof(int));

    index_out.close();
    cout<<buf.getKey()<<endl;
    cout<<key<<endl;

    if (fabs(buf.getKey() - key) < 0.00005)
    {
        int value;
        cout<<"Founded point: "<<foundedPoint<<endl;
        cout<<"base key:"<<key<<endl;
        cout<<"key: "<<buf.getKey()<<" founded"<<endl;
        note_out.seekp(buf.getPoint());
        note_out.read((char*)&value, sizeof(int));
        note_out.read((char*)&value, sizeof(int));
        cout<<"Finded value: "<<value<<endl;
        note_out.close();
        return value;
    }
    else
    {
        cout<<"Not found key"<<endl;
    }
    note_out.close();
    return 0;
}

void ATD::deleteValueByKey(float key) { //Пока находим указатель на нужный к удалнию элемент

    Keynote buf, trash(-1, -1);
    fstream index_out (INDEX_FILE, ios::binary | ios::out | ios::in);
    fstream notes_out (NOTES_FILE, ios::binary | ios::out | ios::in);
    long long findedBlock = findBlockforFind(index_out, key);
    cout<<"foundedBlock: "<<findedBlock<<endl;
    long long foundedIndexPoint = binaryBlockSearch(index_out, findedBlock, key);
    cout<<"foundedIndexPoint: "<<foundedIndexPoint<<endl;
    index_out.seekp(foundedIndexPoint, ios::beg);
    index_out.read((char*)&buf, sizeof(Keynote));
    index_out.seekp(-sizeof(Keynote), ios::cur);
    int posDel = (findedBlock + sizeof(Keynote) * SizeOfBlock - foundedIndexPoint) / sizeof(Keynote) - 1;
    cout<<posDel<<endl;
    int size = -sizeof(int);
    if (foundedIndexPoint < 0)
    {
        cout<<"Not founded index :( for delete"<<endl;
    }
    else {
        cout<<"Buffer key: "<<buf.getKey()<<endl;
        cout<<posDel<<endl;
        cout<<index_out.tellp()<<endl;
        notes_out.seekp(buf.getPoint(), ios::beg);
        notes_out.write((char*)&size, sizeof(int)); //Метка об удалении
        index_out.write ((char*)&trash, sizeof(Keynote));

        for (int i=0; i<posDel; i++)
        {
            index_out.read((char*)&buf, sizeof(Keynote));
            index_out.seekp(-2*sizeof(Keynote), ios::cur);
            index_out.write((char*)&buf, sizeof(Keynote));
            index_out.seekp(sizeof(Keynote), ios::cur);
        }
        index_out.seekp(-sizeof(Keynote), ios::cur);
        index_out.write((char*)&trash, sizeof(Keynote));
    }
    index_out.close();
    notes_out.close();
}

void ATD::deleteEmptyBlock(fstream &fl, long long beginEmptyBlock) {

    Keynote buf;
    for (long long i=fl.tellp(); i<AmountOfBlock* sizeof(Keynote)*SizeOfBlock; i+= sizeof(Keynote))
    {
        fl.seekp(i, ios::beg);
        fl.read((char*)&buf, sizeof(Keynote));
        fl.seekp(i - SizeOfBlock* sizeof(Keynote));
        fl.write((char*)&buf, sizeof(Keynote));
    }
}

int ATD::getAmountOfBlock() const {
    return AmountOfBlock;
}

int ATD::getSizeOfBlock() const {
    return SizeOfBlock;
}

void ATD::moveBlockRight(fstream &fl, long long CurrentBlock) {
    Keynote trash(-1, -1);
    fl.seekg(CurrentBlock, ios::beg);
    char *buf = new char[SizeOfBlock*sizeof(Keynote)];

    fl.read(buf, SizeOfBlock*sizeof(Keynote));
    fl.seekg(-SizeOfBlock*sizeof(Keynote), ios::cur);
    for (int i=0; i<SizeOfBlock; i++)
    {
        fl.write((char*)&trash, sizeof(Keynote));
    }
    fl.write(buf, SizeOfBlock*sizeof(Keynote));
    delete buf;
}
