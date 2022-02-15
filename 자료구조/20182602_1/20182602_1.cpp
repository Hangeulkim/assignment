#include<iostream>

using namespace std;

void divide(int a1,int a2,int &q,int &r){
    q=a1/a2;
    r=a1%a2;
}

int main(){
    int num1=0,num2=0;
    int q=0,r=0;
    cin>>num1>>num2;

    try{
        if(num2==0)
            throw 'e';
        divide(num1,num2,q,r);
    }
    catch (char error){
        cout<<"0으로 나누기는 불가능 합니다.\n";
        return 0;
    }

    cout<<q<<'\n'<<r;
    return 0;
}
