#include<iostream>
#include<ctime>
#include<cstdlib>
#include<cstring>

using namespace std;

class Bizcard{
public:
    char *name;
    char *phone;

public:
    Bizcard(){
        name=NULL;
        phone=NULL;
    }

    Bizcard(const char *name, const char *phone){
        this->name = new char[strlen(name)+1];
        strcpy(this->name, name);
        this->phone = new char[strlen(phone)+1];
        strcpy(this->phone, phone);
    }

    Bizcard(const Bizcard &copy)
    {
        name = new char[strlen(copy.name)+1];
        strcpy(name, copy.name);
        phone = new char[strlen(copy.phone)+1];
        strcpy(phone, copy.phone);
    }

    Bizcard& operator=(const Bizcard& rhs){
        if (this != &rhs){
            this->name = new char[strlen(rhs.name)+1];
            strcpy(this->name, rhs.name);
            this->phone = new char[strlen(rhs.phone)+1];
            strcpy(this->phone, rhs.phone);
        }
        return *this;
    }

    void ShowInfo(){
        cout<<name<<'\n'<<phone<<'\n';
    }

    ~Bizcard(){
        delete [] name;
        delete [] phone;
    }
};

template <class T>
class Bag
{
public:
  Bag (int bagCapacity = 3);
  ~Bag();

  int Size() const;
  bool IsEmpty() const;
  T& Element() const;

  void Push(const T&);
  void Pop();

public:
  T *array;
  int capacity;
  int top;
};

template <class T>
Bag<T>::Bag (int bagCapacity): capacity ( bagCapacity ) {
  if (capacity < 1) throw "Capacity must be > 0";
  array = new T[capacity];
  top = -1;
};

template <class T>
void ChangeSize1D(T*& a, const int oldSize, const int newSize)
{
  if (newSize < 0) throw "New length must be >= 0";

  T* temp = new T[newSize];                                  // new array
  int number = min(oldSize, newSize);  // number to copy
  copy(a, a + number, temp);
  delete [] a;                             // deallocate old memory
  a = temp;
}


template <class T>
Bag<T>::~Bag() {delete [ ] array; }

template <class T>
inline int Bag<T>::Size() const {return top+1;}

template <class T>
inline bool Bag<T>::IsEmpty() const { return Size() == 0;}

template <class T>
inline T& Bag<T>::Element() const{
    if(IsEmpty()) throw "Bag is empty";
    srand(time(NULL));
    int checkpos = rand() % Size();
    return array[checkpos];
}

template <class T>
void Bag<T>::Push(const T& x) {
  if (capacity ==top+1)
  {
    ChangeSize1D(array, capacity, 2 * capacity);
    capacity *=2;
  }
  array[++top] = x;
}

template <class T>
void Bag<T>::Pop() {
  if (IsEmpty()) throw "Bag is empty, cannot delete";
  srand(time(NULL));
  int deletePos = rand() % Size();
  array[deletePos].~T();   // destructor for T
  copy(array+deletePos+1,array+top+1,array+deletePos);
      // compact array
  top--;
}

int main(){
    Bag<Bizcard> bag;
    try{
        cout<<bag.Size()<<'\n';
        cout<<bag.IsEmpty()<<'\n';
        bag.Element();
    }
    catch(const char * e){
        cout<<e<<'\n';
    }

    bag.Push(Bizcard("±èÂùÁø","010-2827-3123"));
    bag.Push(Bizcard("¼Õ½Â¿ì","010-1234-1234"));
    bag.Push(Bizcard("¹Ú¹ÎÈ£","010-1234-5678"));
    cout<<"\n\n";

    bag.Push(Bizcard("¹Ú¿µ¹Î","010-1111-1111"));
    bag.Push(Bizcard("±èµ¿ÇØ","010-2222-2222"));
    bag.Push(Bizcard("±è¿µ±Ô","010-3333-3333"));
    bag.Push(Bizcard("¹Ú±â¼ö","010-4444-4444"));

    cout<<"\n\n";

    for(int i=0;i<bag.Size();i++)
    {
        bag.array[i].ShowInfo();
        cout<<"\n\n";
    }

    bag.Pop();
    bag.Pop();
    cout<<"\n\n";
    for(int i=0;i<bag.Size();i++)
    {
        bag.array[i].ShowInfo();
        cout<<"\n\n";
    }


}
