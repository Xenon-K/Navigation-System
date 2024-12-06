/*****************************************************
Template prepared by Kazumi Slott
CS311
min heap class

Your name: Ji Guo
Your programmer number: 15
Hours spent making and testing your min heap class: 30min
An y difficulties?: none
*******************************************************/
#ifndef MINHEAP_GRAPH_H
#define MINHEAP_GRAPH_H
#include <iostream> //for operator<<()
using namespace std;
#include "swap.h" //for mySwap().  If you didn't create it, you can use the library's swap()

template <class T>
class minHeap;

template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h);

template <class T>
class minHeap
{
  friend ostream& operator<< <T>(ostream& o, const minHeap<T>& h);
private:
  T* ar; //dynamic array
  int capacity; //the size of ar
  int num; //the number of elements I have in ar
public:
  minHeap(){ar = NULL; capacity = 0; num = 0;}
  minHeap(int c);
  ~minHeap(){if(ar!=NULL)delete [] ar;}
  void min_heapify(int* curDist, int* locator, int i);
  void build_min_heap(int* curDist, int* locator);
  void bubbleUp(int* curDist, int* locator, int i);
  void insert(int* curDist, int* locator, const T& el);
  int find(const T& key) const;
  void remove(int* curDist, int* locator, int i);
  T getMin(int* curDist, int* locator);
  int getNum() const;
  void fixHeap(int* curDist, int* locator, int i);

  class Overflow{};
  class BadIndex{};
  class NotFound{};
};

/*************
This function will output what we have in minHeap
*************/
template <class T>
ostream& operator<<(ostream& o, const minHeap<T>& h)
{
  if(h.num == 0)//if nothing is in the minHeap
    o << "none";
  else
    {
      for (int i = 0; i < h.num; i++)
	o << h.ar[i] << " ";
    }
  o << endl;
  return o;
}

/*************
Constructor, with capacity setting
*************/
template<class T>
minHeap<T>::minHeap(int c)
{
  ar = new T[c];//set a new array
  capacity = c;//set capacity
  num = 0;
}

/*************
This function make sure the array follows the minHeap property, and it will start from the root i, search down to the leaf direction

i is the index we start with
It returns nothing
************/
template<class T>
void minHeap<T>::min_heapify(int* curDist, int* locator, int i)
{
  int l = 2*i+1;//left child
  int r = 2*i+2;//right child
  int smallest = i;
  //the following two if statement is to determine who gets the smallest number
  if(l < num && curDist[ar[smallest]] > curDist[ar[l]])
    smallest = l;
  if(r < num && curDist[ar[smallest]] > curDist[ar[r]])
    smallest = r;
  if(smallest != i)//if smallest is still i, the heap is in order(for this pair)
    {
      locator[ar[smallest]] = i;
      locator[ar[i]] = smallest;
      mySwap(ar[smallest], ar[i]);
      min_heapify(curDist, locator, smallest);//not in order, then after the swap, we move down
    } 
}

/*************
This function will build the array by following the heap property by min heap

It returns nothing
************/
template <class T>
void minHeap<T>::build_min_heap(int* curDist, int* locator)
{
  for(int i = num/2-1; i >= 0; i--)//start from the last parent level and sort down
    min_heapify(curDist, locator, i);
}

/*************
This function is the same as min_heapify, but we are moving toward the root

i is index we start with
It returns nothing
************/
template <class T>
void minHeap<T>::bubbleUp(int* curDist, int* locator, int i)
{
  if(i == 0)//if we reach the root
    return ;
  //if not, we keep comparing to the parent of the current node
  for(int parent = (i-1)/2; curDist[ar[i]] < curDist[ar[parent]]; i = parent, parent = (i-1)/2)
    {
      locator[ar[i]] = parent;
      locator[ar[parent]] = i;
      mySwap(ar[i], ar[parent]);
    }
}

/*************
This function will insert an element to the array. Then use bubble up to maintain the heap property

el is the element we insert
It returns nothing
************/
template <class T>
void minHeap<T>::insert(int* curDist, int* locator, const T& el)
{
  if(num >= capacity)
    throw Overflow(); //"The array is full"; 
  else
    {
      ar[num] = el;//add the element
      bubbleUp(curDist, locator, num);//keep the heap property
      num++;//increment the size
    }
}


template <class T>
int minHeap<T>::find(const T& key) const
{
  for(int i = 0; i < num; i++) 
    if(ar[i] == key)
      return i;

  return -1;
  //The element doesn't exist 
  throw NotFound();// "The element doesn't exist";                                                                                            
}

/*************
This function will remove an element from the array

i is the index of the element to be removed
It returns nothing
************/
template <class T>
void minHeap<T>::remove(int* curDist, int* locator, int i)
{
  if(i >= num || i < 0)
    throw BadIndex(); //"The element doesn't exist";
  else
    {
      locator[ar[i]] = num-1;
      locator[ar[num-1]] = i;
      mySwap(ar[i],ar[num-1]);//swap with last index
      num--;//decrement the size
      fixHeap(curDist,locator, i);
    }
}

/*************
This function will remove the first element from the array

It returns the smallest element
************/
template <class T>
T minHeap<T>::getMin(int* curDist, int* locator)
{
  if(num > 0)
    {
      T el = ar[0];
      remove(curDist, locator, 0);
  //This function removes the top element and returns it.
  //You should be calling remove()
      return el;
    }
  else//if the we don't have anything in the heap
    throw BadIndex();
}

/******************************************************************************
Make a copy of minHeap.h and name it minHeap-graph.h (dash not underscore)
Add the following 2 functions to your minHeap-graph.h
A lot of the member functions in minHeap class need to have 2 more parameters, int* curDist, int* locator.
To see why, check minHeap::fixHeap() below.
Don't forget to include minHeap-graph.h to graph.h
******************************************************************************/

//return the number of elements in the heap.
template <class T>
int minHeap<T>::getNum() const
{
  return num;
}

//i is the index (within heap) of the vertex whose distance was changed. 
//For example, if vertex 0's distance changed when it is located at 3, i is 3. This is where the heap needs to be fixed by either going up or down.
//curDist is the array that contains the shortest distance from start to every other vertex.                                           
//locator is the array that contains the location of each vertex within the heap. Make sure you understand the homework document posted on Canvas to understand how locator works.                                               
template <class T>
void minHeap<T>::fixHeap(int* curDist, int* locator, int i)
{
  
  //We need to maintain the min heap based on curDist. When 2 vertices swap within the heap, their locations will change, hence you need to update the locator array. Make sure you understand how each array works by reading the homework document (there are pictures to explain). 

  //From i, either bubble up or down.                                         

  //For up, call bubbleUp(). For down, call min_heapify().
  if(i < num && i >= 0)
    {
      if(curDist[ar[i]] < curDist[ar[(i-1)/2]])//if it is smaller than it's parent, we search up
	bubbleUp(curDist, locator, i);
      else if(curDist[ar[i]] > curDist[ar[2*i+1]] || curDist[ar[i]] > curDist[ar[2*i+2]])//if it is largeer than it's child, we search down
	min_heapify(curDist, locator, i);
    }
  //Reminder: A lot of the member functions in minHeap class need to have 2 more parameters, int* curDist, int* locator.

  //Since I have the same code inside minHeap::remove(), I am now calling fixHeap() from remove() so I won't have duplicate code.
}


#endif
