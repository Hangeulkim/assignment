#include<iostream>
#include<algorithm>

using namespace std;

template <class K,class E> class BST; // forward declaration

template <class K,class E>
class TreeNode {
friend class BST<K,E>;
private:
  pair<K,E> data;
  TreeNode<K,E> *leftChild;
  TreeNode<K,E> *rightChild;
  TreeNode<K,E> *high;
public:
    TreeNode(K a,E b){
        data=make_pair(a,b);
        leftChild=0;
        rightChild=0;
        high=0;
    }
    TreeNode(const pair<K,E> data){
        this->data=data;
        leftChild=0;
        rightChild=0;
        high=0;
    }
};

template <class K, class E>
class BST {
public:
    BST(){
        root=0;
    }
    // creates an empty binary tree

    void Visit(TreeNode<K,E> *currentNode);

    pair<K,E>* Get(const K& k);

    pair<K,E>* Get(TreeNode<K,E>* p, const K& k);

    void Insert(const pair<K,E>& thePair);

    void Delete(const K&);

    void Inorder();

    void Inorder(TreeNode<K,E> *currentNode);


private:
  TreeNode<K,E> *root;
};

template <class K, class E>
void BST<K,E>::Delete(const K& k){
    TreeNode<K,E> *p=root;
    TreeNode<K,E> *pp=0;

    while(p){
        if(p->data.first==k){
            break;
        }

        else if(p->data.first>k){
            p=p->leftChild;
        }

        else{
            p=p->rightChild;
        }
    }

    if(!p) return;

    while(p){
        pp=p;
        if(p->leftChild!=0){
            p->data=p->leftChild->data;
            p=p->leftChild;
        }

        else{
            if(p->rightChild==0) break;
            p->data=p->rightChild->data;
            p=p->rightChild;
        }
    }

    if(pp->high->leftChild!=0 && pp->high->leftChild->data.first == pp->data.first){
        p=pp;
        p->high->leftChild=0;
    }
    else{
        p=pp;
        p->high->rightChild=0;
    }
    delete pp;
}

template <class K, class E>
void BST<K,E>::Inorder()
{// Driver calls workhorse for traversal of entire tree.  The driver is
// declared as a public member function of Tree.
    Inorder(root);
    cout<<'\n';
}

template <class K,class E>
void BST<K,E>::Visit(TreeNode<K,E> *currentNode){
    cout<<currentNode->data.second<<" ";
}

template <class K,class E>
void BST<K,E>::Inorder(TreeNode<K,E> *currentNode)
{// Workhorse traverses the subtree rooted at currentNode.
// The workhorse is declared as a private member function of Tree.
  if (currentNode) {
    Inorder(currentNode->leftChild);
    Visit(currentNode);
    Inorder(currentNode->rightChild);
  }
}


template <class K, class E>  // Driver
pair<K,E>* BST<K,E>::Get(const K& k)
{// Search the binary search tree (*this) for a pair with key k.
// If such a pair is found, return a pointer to this pair; otherwise, return 0.
  return Get(root, k);
}

template <class K, class E>  // Workhorse
pair<K,E>* BST<K,E>::Get(TreeNode<K,E>* p, const K& k)
{
  if (!p) return 0;
  if (k < p->data.first) return Get(p->leftChild,k);
  if (k > p->data.first) return Get(p->rightChild,k);
  return &p->data;
}

template <class K, class E>
void BST<K,E>::Insert(const pair<K,E>& thePair)
{// Insert thePair into the binary search tree.
// search for thePair.first, pp is parent of p
   TreeNode<K,E> *p = root, *pp = 0;

   while (p) {
      pp = p;
      if (thePair.first < p->data.first) p = p->leftChild;
      else if (thePair.first > p->data.first) p = p->rightChild;
      else //  duplicate, update associated element
        {p->data.second = thePair.second; return;}
   }

   // perform insertion
   p = new TreeNode<K,E> (thePair);
   if (root)  // tree not empty
   {
       if (thePair.first < pp->data.first) pp->leftChild = p;
       else pp->rightChild = p;
       p->high = pp;
   }
   else root = p;
}


int main(){
    BST<int,int> tree;

    tree.Insert(make_pair(8,88));
    tree.Insert(make_pair(4,44));
    tree.Insert(make_pair(9,99));
    tree.Insert(make_pair(2,22));
    tree.Insert(make_pair(1,11));
    tree.Insert(make_pair(6,66));
    tree.Insert(make_pair(3,33));
    tree.Insert(make_pair(5,55));
    tree.Insert(make_pair(7,77));

    tree.Inorder();

    tree.Delete(8);

    tree.Inorder();

    tree.Delete(2);

    tree.Inorder();

    pair<int, int> *a=tree.Get(2);
    if(a==0) cout<<"없습니다."<<'\n';
    else cout<<a->second<<'\n';

    a=tree.Get(9);
    if(a==0) cout<<"없습니다."<<'\n';
    else cout<<a->second<<'\n';

    return 0;
}
