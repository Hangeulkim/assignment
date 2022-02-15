#include<iostream>

using namespace std;

class Number
{
private:
    int num;
public:
    Number(int n=0) : num(n) {
    }
    ~Number(){
    }
    int GetData() {return num;}
    void ShowData() {cout<<num<<'\n';}
    Number& operator=(const Number& ref)
    {
        num=ref.num;
        return *this;
    }
    operator int ()
    {
        return num;
    }
};

template <class T> class Chain;

template <class T>
class ChainNode {
friend class Chain<T>;
public:
    ChainNode(T element = T(0), ChainNode * next = 0){
        data=element; link=next;
    }
    ~ChainNode(){
        ~T();
    }
    void delete_data(){
        delete data;
    }
private:
  T data;
  ChainNode<T> *link;
};

template <class T>
class Chain {
private:
    ChainNode<T> *first;
    int size = 0;

public:
  Chain() {
      first = new ChainNode<T>;
  } // constructor initializing first to 0
  ~Chain() {
      cout<<'\n';
      ChainNode<T> *temp = first;
      while(temp->link != 0){
          ChainNode<T> *deltemp = temp;
          temp = temp->link;
          delete deltemp;
      }
      delete temp;
  }
  void delete_all(){
      cout<<'\n';
      ChainNode<T> *temp = first;
      while(temp->link != 0){
          ChainNode<T> *deltemp = temp;
          temp = temp->link;
          delete deltemp;
      }
      first->link = 0;
      size=0;
  }
  bool IsEmpty(){
      if(first->link == 0) return true;
      return false;
  }
  void push_front(int data){
      ChainNode<T> *temp = new ChainNode<T>(T(data),first->link);
      first->link = temp;
      size++;
  }
  void push_back(int data){
      ChainNode<T> *temp = first;
      while(temp->link != 0) temp = temp->link;
      temp->link = new ChainNode<T>(T(data));
      size++;
  }
  void push_i(int i, int data){
      if(IsEmpty()) throw "비었습니다.";
      if(i>size) throw "i가 size보다 큽니다.";
      ChainNode<T> *temp=first;
      for(int j = 0 ; j <= i ; j++) temp = temp->link;
      ChainNode<T> *NewNode = new ChainNode<T>(T(data),temp->link);
      temp->link = NewNode;
      size++;
  }
  T Front(){
      if(IsEmpty()) throw "비었습니다.";
      return first->link->data;
  }
  T Back(){
      if(IsEmpty()) throw "비었습니다.";
      ChainNode<T> * temp = first;
      while(temp->link != 0) temp = temp->link;
      return temp->data;
  }
  T Get(int i){
      if(IsEmpty()) throw "비었습니다.";
      ChainNode<T> *temp = first;
      for(int j = 0; j < i; j++) temp=temp->link;
      return temp->link->data;
  }
  void pop_front(){
      if(IsEmpty()) throw "비었습니다.";
      ChainNode<T> *temp = first->link;
      first->link = first->link->link;
      delete temp;
  }
  void pop_back(){
      if(IsEmpty()) throw "비었습니다.";
      ChainNode<T> *temp = first;
      while(temp->link->link != 0) temp = temp->link;
      ChainNode<T> *deltemp = temp->link;
      temp->link = 0;
      delete deltemp;
  }
  void pop_i(int i){
      if(IsEmpty()) throw "비었습니다.";
      if(i>size) throw "i가 size보다 큽니다.";
      ChainNode<T> *temp = first;
      for(int j = 0; j < i ; j++) temp = temp->link;
      ChainNode<T> *deltemp = temp->link;
      temp->link = temp->link->link;
      delete deltemp;
  }
  int Size(){
      return size;
  }
  int sum(){
      if(IsEmpty()) throw "비었습니다.";
      ChainNode<T> *temp = first;
      int result=0;
      while(temp->link != 0){
          result+=temp->link->data.GetData();
          temp = temp->link;
      }
      return result;
  }

  class ChainIterator {
    public:
      // typedefs required by C++ for a forward iterator omitted

      // constructor
      ChainIterator(ChainNode<T>* startNode = 0)
        {current = startNode;}

      // dereferencing operators
      T& operator*() const {return current->data;}
      T* operator->() const {return &current->data;}

      // increment
      ChainIterator& operator++()   // preincrement
        {current = current->link; return *this;}
      ChainIterator operator++(int) // postincrement
        {
          ChainIterator old = *this;
          current = current->link;
          return old;
        }

      // equality testing
      bool operator!=(const ChainIterator right) const
        {return current != right.current;}
      bool operator==(const ChainIterator right) const
        {return current == right.current;}



      private:
        ChainNode<T>* current;
    };
    typedef ChainIterator iterator;

    ChainIterator begin() {return ChainIterator(first);}
    ChainIterator end() {return ChainIterator(0);}
};

int main(){
    try{
        Chain<Number> chain;
        Chain<Number>::iterator iter;
        for(int i=1;i<10;i++) chain.push_back(i);
        for(iter = ++chain.begin() ; iter != chain.end() ; iter++) iter->ShowData();

        chain.push_front(0);

        cout<<'\n';
        cout<<chain.sum()<<'\n';

        chain.push_back(10);
        cout<<'\n';
        chain.Front().ShowData();

        cout<<'\n';
        chain.Back().ShowData();

        cout<<'\n';
        chain.Get(2).ShowData();

        chain.pop_front();
        cout<<'\n';
        for(iter = ++chain.begin() ; iter != chain.end() ; iter++) iter->ShowData();
        cout<<chain.sum()<<'\n';

        chain.pop_back();
        cout<<'\n';
        for(iter = ++chain.begin() ; iter != chain.end() ; iter++) iter->ShowData();
        cout<<chain.sum()<<'\n';

        chain.push_i(2,100);
        cout<<'\n';
        for(iter = ++chain.begin() ; iter != chain.end() ; iter++) iter->ShowData();
        cout<<chain.sum()<<'\n';

        chain.pop_i(6);
        cout<<'\n';
        for(iter = ++chain.begin() ; iter != chain.end() ; iter++) iter->ShowData();
        cout<<chain.sum()<<'\n';
    }
    catch(const char *e){
        cout<<e<<'\n';
    }

    return 0;
}
