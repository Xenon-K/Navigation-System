/****************************
Templated created by Kazumi Slott
CS311

Your name: Ji Guo
Your programmer number: 15
Hours spent: 3hr
Any difficulties?: None
*****************************/
#ifndef GRAPH_H
#define GRAPH_H
#include <iostream>
#include "minHeap-graph.h"
#include <queue>
#include <stack>
#include <list>
using namespace std;

class edge
{
  friend class graph;  //You want to access the private members of edge class from graph class
  int neighbor; //adjacent vertex
  int wt; //weight of the edge
 public:
  edge() { neighbor = -999, wt = -999;};
  edge(int u, int w) {neighbor = u, wt = w;};
};

class graph
{
  int num_ver; //the total number of vertices in the graph
  list<edge*>* ver_ar; //pointer to the dynamic array that holds linked lists. The element of a node in a linked list is a pointer to an edge object 
                       //Let's use the library's list class. It is actually a doubly linked list. 
  int nextUnvisitedNodes(int* num, int start, int s) const;
  void DFT_helper(int v, int& i, int* num, queue<string>& q) const;
 public:
  graph(int V);
  ~graph();
  void addEdge(int v, int u, int w=1);  //If the weight is not given, 1 is used for wt.
  void BFT(int start) const;
  void DFT(int start) const;
  int returnNum_ver() const {return num_ver;}
  void DijkstraShortestPath(int start) const;
  void DijkstraShortestPath_iterator(minHeap<int>& h, int* curDist, int* locator, int* predecessor, int v) const;
};

/********************************************************************************** 
constructor
it returns nothing 
 
int num: the number of vertex in the graph
**********************************************************************************/ 
graph::graph(int num)
{
  //make a dynamic array with num slots where each slot holds a list object. 
  //The element of a node in the linked list is a pointer to an edge object 
  num_ver = num;
  ver_ar = new list<edge*> [num];
}

/********************************************************************************** 
destructor
it returns nothing
**********************************************************************************/ 
graph::~graph()
{
  //destroy all the edge objects created in heap
  for (int i = 0; i < num_ver; i++)
    for (list<edge*>::iterator u = ver_ar[i].begin(); u != ver_ar[i].end(); u++)
      delete (*u);
  //For each vertex in ver_ar
  //    go through each node in the linked list. The element field of each node points to an edge object created with "new". You need to delete these.  

  //The library's list class has a destructor. All nodes belonging to the list will be destroyed by the destructor.
  //https://www.cplusplus.com/reference/list/list/~list/
  delete[] ver_ar;
  //destroy the ver_ar dynamic array
}

/********************************************************************************** 
add an edge from v to u with weight w into the graph
it returns nothing

int v: this is the vertex
int u: this is the vertex that is connected to v
int w: this is the length of the edge
**********************************************************************************/ 
void graph::addEdge(int v, int u, int w)
{
  //We are using the list class from the library. Which member function do you need to use to add a node at the end of the list?
  //https://www.cplusplus.com/reference/list/list/
  //Don't create an edge object statically, then it would get destroyed as soon as you leave this function. You need to create an edge object dymamically in heap, which will remain in heap even after you leave this function.. Remember "new" operator returns the memory address of the newly created object.
  //I have one line of code in this function.
  ver_ar[v].push_back(new edge(u,w));
}

//I decided to make this function a stand alone function. It doesn't have anything to do with the graph class.
/********************************************************************************** 
prints the queue
it returns nothing

queue<T>& q is the queue
**********************************************************************************/ 
template<class T>
void diplayQueue(queue<T>& q)
{
  while(!q.empty())
    {
      cout << q.front() << ",";
      q.pop();
    }
}

/********************************************************************************** 
start Traversal at start, print all the vertices by DFT order
it returns nothing

int start: this is the starting vertex
**********************************************************************************/ 
void graph::DFT(int start) const
{
  queue<string> q;//q stores the path
  //The algorithm is in my lecture notes.
  //I made dynamic array called "num"
  int *num = new int[num_ver];
  //I created a queue object of string data type from the library's queue class to save all the paths.
  for(int i = 0; i < num_ver; i++)//initialize the vertices counter
    num[i] = 0;
  int j = 1;
  int v = start;//v is the spot we start with
  //I used do-while
  do{
    DFT_helper(v, j, num, q);//puts path into the queue
    v = nextUnvisitedNodes(num, v, v+1);//checks if the vertex is 0
  }while(v != -1);
  //I am calling nextUnvisitedNodes() to see if there are more unvisited nodes.
  //You could call displayQueue() implemented above to display all the paths. 
  diplayQueue<string>(q);
  //Don't forget to destroy the dynamic array
  delete[] num;
}

//I made the following function to see if there are still unvisited nodes. Start checking at s, which is the one after the vertext we started last time                       
//num points to an array containing visit numbers. 0 indicates, the vertex hasn't been visited yet.                                           
//s is the vertext right after the vertex we started last time
/********************************************************************************** 
this function goes to the index of next unvisited node
it returns the index of next unvisited node

int* num: this is the vertices array
int start: this is the starting vertex
int s: this is the vertex right after the one we just checked
**********************************************************************************/ 
int graph::nextUnvisitedNodes(int* num, int start, int s) const
{
  //go through the num array to see if you find a vertext with num=0. If so, return the index. If all visited, return -1.
  for (int i = s; i != start; i=(i+1) % num_ver)
    if (num[i] == 0)
      return i;
  return -1;
}

/********************************************************************************** 
this function puts the next unvisited vertex and put that edge into the queue
it returns nothing

int v: this is the vertex right after the one we just checked
int& i: this is the how many nodes are being checked
int* num: this is the vertices array
queue<string>& q: this is the queue that has all the edges
**********************************************************************************/ 
void graph::DFT_helper(int v, int& i, int* num, queue<string>& q) const
{
  //The algorithm is in my lecture notes
  num[v] = i++;
  //The following shows how to iterate the library's linked list. You use an iterator.
  cout << v << " ";//output the node
  //https://www.cplusplus.com/reference/list/list/end/ 
  //iterator class is nested inside list class
  for (list<edge*>::iterator u = ver_ar[v].begin(); u != ver_ar[v].end(); u++)
    {
      //From the example on https://www.cplusplus.com/reference/list/list/end/
      //Notice *u gives you the element of the node pointed to by u. The element is a pointer to an edge object.
      //Between * and -> operator, -> has higher precedence. You need to use () around *u to get the element first.  

      //push each path e.g. 0->1 into q. to_string(int) converts the int to a string. 
      //to_string() is in c++11, so compile with the option. g++ -std=c++11 graphClient.cpp 
      if(num[(*u)->neighbor] == 0)//if this node is unvisited
	{
	  q.push(to_string(v) + "->" + to_string((*u)->neighbor));//push it into the queue
	  DFT_helper((*u)->neighbor, i, num, q);//look for other nodes connected to current node
	}
    }
  /***********************************************************************************************
   **********************************************************************************************
    Compile with a c++11 option if you are using to_string()
        g++ -std=c++11 graphClient.cpp 
	**********************************************************************************************
	***********************************************************************************************/
}


//start is the index for the start vertex
/********************************************************************************** 
start Traversal at start, print all the vertices by BFT order
it returns nothing

int start: this is the starting vertex
**********************************************************************************/ 
void graph::BFT(int start) const
{
  //The algorithm is in my lecture notes

  //Use the queue class from the library
  queue<int> q;
  //I used another queue of strings to save all the paths. 
  queue<string> path;

  int *num = new int[num_ver];//initializing the vertices counter
  for(int i = 0; i < num_ver; i++)
    num[i] = 0;
  int i = 1;
  int v = start;//v is the spot we start with
  //I used do-while to visit all unconnected graphs. Call nextUnvisitedNodes() to check to see if there are more unvisited verteces.
  do{
    //Check the manual to figure out what member functions of the queue class you need to use.
    // https://www.cplusplus.com/reference/queue/queue/
    num[v] = i++;//mark the place we start with
    q.push(v);
    while(!q.empty())
      {
	int f = q.front();
	cout << f << " ";
	q.pop();
	//get all the nodes that we haven't visit yet
	for (list<edge*>::iterator u = ver_ar[f].begin(); u != ver_ar[f].end(); u++)
	  {
	    if(num[(*u)->neighbor] == 0)
	      {
		num[(*u)->neighbor] = i++;
		q.push((*u)->neighbor);
		path.push(to_string(f) + "->" + to_string((*u)->neighbor));
	      }
	  }
      }
    v = nextUnvisitedNodes(num, v, v+1);
  }while(v != -1);
  //You could call show all paths by calling displayQueue()
  diplayQueue(path);
  //Don't forget to destroy the dynamic array if you used one
  delete[] num;
}

/******************************************************************************************************
Prepared by Kazumi Slott
CS311
Additional functions for graph part 2
*******************************************************************************************************/

/******************************************************************************************************
Add the following 3 functions to your graph.h.
Notice the first 2 functions are not member functions. I could have made them member functions, but I decided
not to. showShortestDistance() is like a part of an application. printHeapArrays() is just for testing.
You can list them above dijkstraShortestPath() in graph.h.
*******************************************************************************************************/

//dijkstra's algorithm calculates the shortest distance from start to every other vertex
//This stand alone function shows the shortest path from start to destination in the following format.
//  The shortest path from 3 to 5 is 3 0 4 5 
//  The distance is 8
/********************************************************************************** 
this function shows the shortest distance and the path
it returns nothing

int* curDist: this is the curDist array
int* predecessor: this is the predecessor array
int start: this is the starting vertex
**********************************************************************************/ 
void showShortestDistance(int* curDist, int* predecessor, int start)
{
  int dest; //destination
  cout << "Enter the destination: ";                                                                          
  cin >> dest;

  stack<int> distance;
  //trace the shortest path from dest back to start  
  //I suggest you use either library's stack or queue. Which one will work?
  //I use stack because we are tracing from dest to start, in this way, so the start will at top
  int dest_dist = curDist[dest];
  cout << "The shortest path from " << start << " to " << dest << " is ";
  while(dest != start)//trace back to the start
    {
      distance.push(dest);
      dest = predecessor[dest];
    }
  distance.push(dest);
  while(!distance.empty())//pop the entire stack
    {
      cout << distance.top() << " ";
      distance.pop();
    }
  cout << "\nThe distance is " << dest_dist << endl;      
}

//You don't need to change the following function.
//This function is for checking the heap and all the arrays. You may want to call it while you are developing Dijkstra's function
//This is not part of the graph class.  It is made for testing anyway.
/********************************************************************************** 
this function shows all three arrays we are passing
it returns nothing

int* curDist: this is the curDist array
int* locator: this is the locator array
int* predecessor: this is the predecessor array
int num_ver: this is the number of vertices in the graph
**********************************************************************************/ 
void printHeapArrays(const minHeap<int>& h, int* curDist, int* locator, int* predecessor, int num_ver)
{
  cout << "heap ------" << endl;
  cout << h << endl;  //This works if you made operator<<() to display a heap

  cout << "locator ------" << endl;
  for(int i = 0; i < num_ver; i++)                                                                                          
    cout << locator[i] << " ";                                                                                               
  cout << endl;

  cout << "curDist ------- " << endl;                                                                                           
  for(int i = 0; i < num_ver; i++)                                                                                          
    cout << curDist[i] << " ";                                                                                               
  cout << endl << endl;

  cout << "Predecessor ------- " << endl;                                                                                      
  for(int i = 0; i < num_ver; i++)                                                                                         
    cout << predecessor[i] << " ";                                                                                             
  cout << endl << endl;
}

//Dijkstra's shortest path. This function will generate a table that contains the shortest distance from start to every other vertex and the predecessor of each vertex.
/********************************************************************************** 
this function calculates all the shortest path base on the starting point
it returns nothing

int start: this is the starting vertex
**********************************************************************************/ 
void graph::DijkstraShortestPath(int start) const
{
  minHeap<int> toBeChecked(num_ver); //the top of this heap has the vertex with the shortest distance
  int* curDist = new int[num_ver];
  //declare a dynamic array called curDist //contains the current shortest distance from start to every other vertex
  int* predecessor = new int[num_ver];
  //declare a dynamic array called predecessor //contains the predecessor of each vertex
  int* locator = new int[num_ver];
  //declare a dynamic array called locator //tells where each vertex exists within the heap. e.g. heap [v3, v1, v2, v4, v0] locator [4, 1, 2, 0, 3] <== vertext 0 can be found at index 4 in heap, vertex 3 can be found at index 0 in heap

  //The algorithm is in my lecture notes. Make sure you understand how each array and heap changes by reading my homework document posted on Canvas.

  //initialize each array
  //curDist should be 999 for all vertices to start with 
  //populate toBeChecked heap 
  //insert all vetices into toBeChecked heap: [0, 1, 2, 3, 4, ...] the numbers are vertex numbers
  for(int i = 0; i < num_ver; i++)
    {
      curDist[i] = 999;
      toBeChecked.insert(curDist, locator, i);
      locator[i] = i;
      predecessor[i] = -1;
    }
  //to start with, locator [0, 1, 2, 3, 4, ...]
  
  //I didn't initialize predecessor.  You could if you want.

  //A lof of code here - check the algorithm in my lecture notes
  
  curDist[start] = 0;//distance from start to start is 0
  int v = start;//start from start
  while(toBeChecked.getNum() > 0)//before minHeap is empty
    {
      toBeChecked.fixHeap(curDist, locator, locator[v]);//matain the minHeap property
      v = toBeChecked.getMin(curDist, locator);//pop the closest location
      for (list<edge*>::iterator u = ver_ar[v].begin(); u != ver_ar[v].end(); u++){//iterate through all the connected location
	if(toBeChecked.find((*u)->neighbor) != -1)//if this location does not have a closest path
	  {
	    if(curDist[(*u)->neighbor] > ((*u)->wt + curDist[v]))//if this path is shorter than what we have
	      {
		curDist[(*u)->neighbor] = ((*u)->wt + curDist[v]);//store this distance
		predecessor[(*u)->neighbor] = v;//set predecessor
		toBeChecked.fixHeap(curDist, locator, locator[(*u)->neighbor]);//maintain the minHeap property
	      }
	  }
      }
    }

  showShortestDistance(curDist, predecessor, start);//print the result

  //delete these arrays
  delete[] curDist;
  delete[] predecessor;
  delete[] locator;
}

/**********************************************************************************
This function calculate the closest neighbor for city node v, and updates all the relevant info in minHeap, curDist, locator, and predecessor
it returns nothing

minHeap<int>& h: this is the priorityQ we use to determine the closest neighbor
int* curDist: this is the curDist array that only stores the shortest distance
int* locator: this is the array that contains the index of each city in priorityQ
int* predecessor: this is the array that has all the previous vertex for each city
int v: this is the city node we are on, we want to look through its neighbors
**********************************************************************************/
void graph::DijkstraShortestPath_iterator(minHeap<int>& h, int* curDist, int* locator, int* predecessor, int v) const
{
  for (list<edge *>::iterator u = ver_ar[v].begin(); u != ver_ar[v].end(); u++) { //---> iterate through all the connected location
    if (h.find((*u)->neighbor) != -1) //---> if this neighbor is still in the priorityQ
      {
	if (curDist[(*u)->neighbor] > ((*u)->wt + curDist[v])) //---> if this path is shorter than what we have
	  {
	    curDist[(*u)->neighbor] = ((*u)->wt + curDist[v]); //---> store this distance
	    predecessor[(*u)->neighbor] = v; //---> set predecessor
	    h.fixHeap(curDist, locator, locator[(*u)->neighbor]); //---> maintain the minHeap property
	  }
      }
  }
}

#endif
