#define LEFT(i) (2 * i + 1)
#define RIGHT(i) (2 * i + 2)

#include "BinaryHeap.h"


#include <iostream>
#include <cmath>
#include <vector>
#include <iomanip>


BinaryHeap::BinaryHeap()
{
    // TODO: or not
    std::vector<HeapElement> elements;
}

bool BinaryHeap::Add(int uniqueId, double weight)
{
    // TODO:
    for (int i = 0; i < elements.size(); i++) 
    {
        if (elements[i].uniqueId == uniqueId)
            return false;
    }
    struct HeapElement newElement; 
    newElement.uniqueId = uniqueId;
    newElement.weight = weight;
    elements.push_back(newElement);
    int size = elements.size();
    int i = size - 1;
    while(i>0 && elements[(i-1)/2].weight > elements[i].weight){
        HeapElement temp = elements[(i-1)/2];
        elements[(i-1)/2] = elements[i];
        elements[i] = temp;
        i = (i-1)/2;
    }
    return true;
}

bool BinaryHeap::PopHeap(int &outUniqueId, double &outWeight)
{
    // TODO:
    if(elements.size() == 0) return false;

    outUniqueId = elements[0].uniqueId;
    outWeight = elements[0].weight;

    elements[0] = elements[elements.size()-1];
    elements.pop_back();

    int size = elements.size();
    int i = 0;

    while(1){
        int leftChild = LEFT(i);
        int rightChild = RIGHT(i);
        if(leftChild >= size) break; // no child
        if(rightChild >= size){ // only left child
            if(elements[i].weight > elements[leftChild].weight){
                HeapElement temp = elements[i];
                elements[i] = elements[leftChild];
                elements[leftChild] = temp;
            }
            break; // heap restored
        }
        if(elements[leftChild].weight < elements[rightChild].weight){ // left child is smaller
            if(elements[i].weight > elements[leftChild].weight){
                HeapElement temp = elements[i];
                elements[i] = elements[leftChild];
                elements[leftChild] = temp;
                i = leftChild;
            }
            else break; // heap restored
        }
        else{ // right child is smaller
            if(elements[i].weight > elements[rightChild].weight){
                HeapElement temp = elements[i];
                elements[i] = elements[rightChild];
                elements[rightChild] = temp;
                i = rightChild;
            }
            else break; // heap restored
        }        
    }
    return true;
}
bool BinaryHeap::ChangePriority(int uniqueId, double newWeight)
{
    // TODO:
    int flag = 0;
    if(elements.size() == 0)
        return false;
    for (int i = 0; i < elements.size(); i++)
    {
        if (elements[i].uniqueId == uniqueId)
        {
            elements.erase(elements.begin() + i);
            flag = 1;
            break;
        }
    }
    if (flag == 0) // if the uniqueId is not in the heap
        return false;
    // insert it in the right place
    return Add(uniqueId, newWeight);
}

int BinaryHeap::HeapSize() const
{
    // TODO:
    return elements.size();
}
void BinaryHeap::PrintHeap(){
    // write a print function for min heap array to print as array [n,n+1]
    
    int size = HeapSize();
    std::cout << "[";   
    for(int i=0; i<size-1; i++){
        std::cout << "(" << elements[i].uniqueId <<","<< elements[i].weight << "),";
    }
    std::cout << "(" << elements[size-1].uniqueId << "," << elements[size-1].weight << ")]" << std::endl; 
}

bool BinaryHeap::checkHeap(){
    // check if the heap is valid
    int size = HeapSize();
    for(int i = 0; i < size; i++){
        if(2 * i + 1 < size && elements[i].weight > elements[2 * i + 1].weight){
            std::cout << "i: " << elements[i].weight << " 2i+1: " << elements[2 * i + 1].weight << std::endl;
            return false;
            }
        if(2 * i + 2 < size && elements[i].weight > elements[2 * i + 2].weight){
            std::cout << "i: " << elements[i].weight << " 2i+2: " << elements[2 * i + 2].weight << std::endl;
            return false;
            }
    }
    return true;
}