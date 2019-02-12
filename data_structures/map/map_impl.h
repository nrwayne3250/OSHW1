#ifndef DOCUMENTS_MAP_IMPL_H
#define DOCUMENTS_MAP_IMPL_H

#include <string>
#include <memory>
#include <functional>
#include "data_structures/map/maybe.h"
#include <vector>
#include <utility>
#include "Node.h"
using namespace std;
namespace data_structures {
namespace map {

template<typename KeyType, typename ValueType>
class MapImpl {
public:
    typedef std::function<bool(const KeyType&, const KeyType&)> KeyComparerFn;
    typedef std::function<uint32_t(const KeyType&)> HashCalculator;
private:
    const KeyComparerFn key_comparer_;
    const HashCalculator hash_calculator_;
    const uint32_t capacity_;

     Node<KeyType,ValueType> **myMap; // pointer to an array of pointers
public:
    int size =0;
    MapImpl(const KeyComparerFn key_comparer,
            const HashCalculator hash_calculator, const uint32_t capacity)
        : key_comparer_(key_comparer), hash_calculator_(hash_calculator),
          capacity_(capacity)
    {
        myMap = new Node<KeyType,ValueType> *[int(capacity)]; // create array of pointers
    }

    int Size() const { // return number of non null Nodes

        return this->size;
    }

    void Put(const KeyType& key, const ValueType& value) { // inserts pointer to a Node into the array
    int index = GetIndex(key); // hash the key to find its corresponding index

    Node<KeyType, ValueType> *ptr = myMap[index]; // create set it to the pointer at the index in the array of pointers
    if(ptr == NULL){// if no pointer to a Node is at index this will run
       Node<KeyType,ValueType> *newNode =  new Node<KeyType,ValueType>(key, value); // creating a new node with the key and value equal to the parameters passed into funtion and retunr a pointer to it
        myMap[index] = newNode; // set the pointer at the index in array to the same address that new pointer points to
        size++; // increment size because Node was added
    }
    else{// if there was a pointer to a node at the index this will run
        while(ptr->getNext() != NULL && ptr->getKey() != key){ // traverse list as long as next element isnt null and the key of the current element is not equal to your key
            ptr = ptr->getNext();
        }
        if(ptr->getKey() == key){// keys match
            ptr->setValue(value); // change value at that key to new value
            // also this will skip inc size because no new node was added just edited
        }
        else{ // keys didnt match so we will create a new node and add a pointer to it in the array
        Node<KeyType,ValueType> *newNode = new Node<KeyType, ValueType>(key,value);

        ptr->setNext(newNode);
        size++;} // inc size because new node was created
    }
    }

    bool Remove(const KeyType& key) {// function take a key looks for key if present Node is deleted and true is returned otherwise false if returned
        int index = GetIndex(key); // find index
        if(size <= 0){ // if there are no existing nodes exit out return false
            return false;
        }
        Node<KeyType, ValueType> *ptr = myMap[index]; // set pointer to same location as the one at the found index

        Node<KeyType, ValueType> *prevPtr = NULL;//create a pointer to keep track of node before you
        while(ptr != NULL && ptr->getKey() != key){ // while you are not null and you are not the node we are looking for keep traversing
            prevPtr = ptr;
            ptr = ptr->getNext();
        }
        if(ptr->getKey() == key){ // node found
            if(ptr->getNext()== NULL){ // if there is no node after you simply delete you
               delete ptr;
            }
            else{ // if there are nodes after you we need to make sure we dont lose them
                prevPtr->setNext(ptr->getNext());

            delete ptr;}
            size--; // decrement size because we removed a node
            return true;
        }
        return false;
    }

    Maybe<ValueType> Get(const KeyType& key) { // function to see if key we are looking for exists and creates a maybe object that tell you whether it is present of not
        int index = GetIndex(key); // hash key
        Node<KeyType, ValueType> *ptr = myMap[index]; // set pointer to that index
        while(ptr != NULL){// this will stop our traversal if we are null meaning the key was not present
            if(ptr->getKey() == key){ // found key
                Maybe<ValueType> maybe = Maybe<ValueType>(ptr->getValue()); // run maybe constructer that takes an argument and shows that the key was present
                return maybe; //true maybe object is return
            }
            if(ptr->getNext() != NULL){//traversal
            ptr = ptr->getNext();}
        }


        return Maybe<ValueType>();// run default constructor will show false maybe object
    }
private:

    uint32_t GetIndex(const KeyType& key) const {// function to find the hash of a given key
        uint32_t hash = hash_calculator_(key);
        uint32_t index = hash % capacity_;
        return index;
    }
};

}  // namespace map
}  // namespace data_structures

#endif //DOCUMENTS_MAP_IMPL_H
