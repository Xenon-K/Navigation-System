#include <iostream>
#include <fstream>
#include "graph.h"
#include "iomanip"
#include "minHeap-graph.h"
#include <cstdlib>
using namespace std;

//g++ -std=c++11 ABET.cpp
/*
  road.txt can be handled by graph.h

  we are using a city class to store all the city infos. To quickly access each note base on its city code, we are using hash table

  we also want to search by id, so we can simply use an array for that, each city will be insert to the index same as its id, since id is not repeatable and it's in order. 
*/

//unsigned for positive numbers or zero only. long takes up 4 bytes. 0-2^32(4294967295)
unsigned long hashNum(const string& key);

//hash :)
class city{
  friend class hashTbl;
private:
  int ID; //---> the ID of the city
  string City_code; //---> the code of the city
  string City_name; //---> the name of the city
  int population; //---> the population of the city
  int elevation; //---> the elevation of the city
  city* next; //---> stores the address of next city
public:
  city(){ID = -999; City_code = "XX"; City_name = "INVALID"; population = -999; elevation = -999, next = NULL;} //---> default constructor
  city(const int id, const string& code, const string& name, int popu, int elev){ID = id; City_code = code; City_name = name; population = popu; elevation = elev; next = NULL;} //---> constructor
  int returnID() const {return ID;} //---> returns the ID of a city
  int returnPop() const {return population;} //---> returns the population of a city
  int returnElev() const {return elevation;} //---> returns the elevation of a city
  string returnCode() const {return City_code;} //---> returns the code of a city
  string returnName() const {return City_name;} //---> returns the name of a city
};

class hashTbl
{
  city** table; //---> table points to a dynamic array. Each slot points to an city, which points to the next city.
  int size; //---> size of the array
public:
  hashTbl(int size); //---> constructor
  ~hashTbl(); //---> destructor
  void put(city* e); //---> add a new city to the table
  city* get(const string& key) const; //---> search for a key   
  city** getTbl() const {return table;} //---> return the pointer to the table
  class underflow //---> exception class - used when key wasn't found
  {
    string message;
  public:
    underflow(const string& mes) { message = mes; }
    string getMessage() { return message; }
  };
};

/**********************************************************************************
constructor 
it returns nothing 
 
int s: this is the size of the hash 
**********************************************************************************/
hashTbl::hashTbl(int s)
{
  size = s; //---> initialize size
  table = new city*[size]; //---> make the array that contains the city node
  for(int i = 0; i < size; i++) //---> initialize all the slot with NULL
    table[i] = NULL;
}

/**********************************************************************************
destructor 
it returns nothing 
**********************************************************************************/
hashTbl::~hashTbl()
{
  city* cur;
  city* next;
  for (int i = 0; i < size; i++){ //---> go throgh each slot in the hash
    cur = table[i];
    while(cur != NULL) //---> until this slot is empty
      {
        next = cur->next;
        delete cur;
        cur = next;
      }
  }
  delete[] table; //---> delete the array
}

/**********************************************************************************
This function puts a city into the hash base on its city code
it returns nothing 
 
city* e: this is a city node that's ready to be insert into the array 
**********************************************************************************/
void hashTbl::put(city* e)
{
  string key = e->returnCode(); //---> our hash is base on city code
  int index = hashNum(key) % size;
  city* cur = table[index];//---> add to front
  e->next = cur;
  table[index] = e;
}

/**********************************************************************************
This function returns a city in the hash base on the city code we want to search
it returns the city node we are looking for

string& key: this is the city code we want to search for
**********************************************************************************/
city* hashTbl::get(const string& key) const {
  int index = hashNum(key) % size; //---> calculate the index
  if (table[index] == NULL){ //---> not found, throw error
    throw underflow(key);
  }

  //iterate through the array, if found, return this city node
  for (city* link = table[index]; link != NULL; link = link->next){
    if (link->City_code == key){
      return link;
    }
  }  
  throw underflow(key); //---> not found, throw error
}

/**********************************************************************************
This function calculates the index for the city
it returns the index base on city code

string& key: this is the city code
**********************************************************************************/
unsigned long hashNum(const string& key)
{
  const char* cstr = key.c_str(); //---> converting C++ string to c-string.
  
  unsigned long hash = 5381;
  int c;
  while (c == *cstr++) //---> checking each character until it reaches '\0'.
    {
      hash = ((hash << 5) + hash) + c; //---> hash * 33 + c;
    }
  return hash;
}

//20*4/3 round to next prime number = 29
const int SIZE = 29; //---> for hash :)
//prototypes
void group_info();
void showInfo(const city& c, const city& c2);
void DijkstraShortestPath_client(const graph& road, const city& c, const city& c2, const city destinations[]);

int main(int num_args, char* arg[])
{
  // cout << "num_args = " << num_args << endl;
  if(num_args < 3 || num_args > 3)
    {
      cout << "Run as follows: ./a.out number" << endl;
      return 1;
    }

  hashTbl citys(SIZE); //city
  graph roads(20); //roads
  ifstream fin;
  city* destinations = new city[roads.returnNum_ver()]; //the array that stores the city by its id
  fin.open("road.txt");
  //fin.open("/cs/slott/cs311/road.txt")
  int f, t, d; //---> From_City, To_City, and Distance.
  if(!fin)
    cout << "road.txt doesn't exist" << endl;
  else
    {
      fin >> f >> t >> d;
      while(fin)
	{
	  roads.addEdge(f,t,d);
	  fin >> f >> t >> d;
	}
    }
  fin.close();

  fin.open("city.txt");
  //fin.open("/cs/slott/cs311/city.txt")
  string c, n; //---> code, name
  int i, p, e; //--> id, population, elevation
  if(!fin)
    cout << "city.txt doesn't exist" << endl;
  else
    {
      fin >> i >> c >> n >> p >> e;
      while(fin)
	{
	  city* temp = new city(i,c,n,p,e);
	  citys.put(temp);
	  destinations[i] = *temp;
	  fin >> i >> c >> n >> p >> e;
	}
    }
  fin.close();
 
  city* c1;
  city* c2;

  string start(arg[1]);
  string end(arg[2]);
  try//---> try to search for the city, it immediately throws error when a city code is wrong
    {
      c1 = citys.get(start);
    
      c2 = citys.get(end);
    }
  catch(hashTbl::underflow cuddlefish)
    {
      cout << "Invalid city code: " << cuddlefish.getMessage();
       if(end != cuddlefish.getMessage()) //---> if second command is also wrong
        {
          try{
            city* temp2 = citys.get(end);
          }
          catch (hashTbl::underflow cuddlecuddlefish)
            {
	      cout << " " << cuddlecuddlefish.getMessage();
            }
        }
       cout << endl;
       exit(0);
    }
  
  group_info(); //---> show group info
  showInfo(*c1, *c2); //---> show from city and to city
  DijkstraShortestPath_client(roads, *c1, *c2, destinations); //---> get the shortest path
  delete [] destinations;
  return 0;
} 

/**********************************************************************************
This function prints the group information
it returns nothing
**********************************************************************************/
void group_info()
{
  cout << "Author: Ji Guo, Kaila Manangan, and Eric Hernandez" << endl;
  cout << "Date: 11/28/2022" << endl;
  cout << "Course: CS311 (Data structures and Algorithms)" << endl;
  cout << "Description : Program to find the shortest route between cities" << endl;
  cout << setfill('-');
  cout << setw(65) << "-";
  //----------------------------------------------------------------
}

/**********************************************************************************
This function shows the from city info and to city into
it returns nothing

city& c: this is the from city node
city& c2: this is the to city node
**********************************************************************************/
void showInfo(const city& c, const city& c2)
{
  cout << "\nFrom City: "<< c.returnName() << ", population " << c.returnPop() << ", elevation " << c.returnElev() << endl;
  cout << "To City: "<< c2.returnName() << ", population " << c2.returnPop() << ", elevation " << c2.returnElev() << endl;
}

/**********************************************************************************
This function is the dijkstra algorithm, calculates the shortest path base on the from city and to city
it returns nothing

graph& road: this is the city road graph
city& c: this is the from city
city& c: this is the to city
**********************************************************************************/
void DijkstraShortestPath_client(const graph& road, const city& c, const city& c2, const city destinations[])
{
  minHeap<int> toBeChecked(road.returnNum_ver());
  int* curDist = new int[road.returnNum_ver()];
  int* predecessor = new int[road.returnNum_ver()];
  int* locator = new int[road.returnNum_ver()];
  //initialize
  for(int i = 0; i < road.returnNum_ver(); i++)
    {
      curDist[i] = 999;
      toBeChecked.insert(curDist, locator, i);
      locator[i] = i;
      predecessor[i] = -1;
    }

  //we are using city id in graph
  int id1 = c.returnID();
  int id2 = c2.returnID();
  curDist[id1] = 0;
  int v = id1; //---> we start at from city
  while(toBeChecked.getNum() > 0) //---> before all go through all the cities
    {
      toBeChecked.fixHeap(curDist, locator, locator[v]); //---> maintain priorityQ property
      v = toBeChecked.getMin(curDist, locator); //---> take out the closest path
      road.DijkstraShortestPath_iterator(toBeChecked, curDist, locator,  predecessor, v); //---> this function will get the closest neighbor city to the top of priorityQ
    }
  
  if(predecessor[id2] != -1) //---> if there is a path between from city and to city
    {
      cout << "The shortest distance from " << c.returnName() << " to " << c2.returnName() << " is " << curDist[id2] << "\nthrough the route: ";
      stack<int> distance;
      while(id2 != id1) //---> trace back to the start
	{
	  distance.push(id2);
	  id2 = predecessor[id2];
	  //cout << id2 << endl;
	  //cout << destinations[id2].returnName() << " ";
	}
      //distance.push(id2);
      int ready_to_pop = id2;
      //distance.pop();

      while(!distance.empty()) //---> pop the entire stack, while display the poped city info
	{
	  //(ready_to_pop);
	  cout << destinations[ready_to_pop].returnName();
	  cout << "->";
	  ready_to_pop = distance.top();
	  distance.pop();
	}
      //showName;
      cout << destinations[ready_to_pop].returnName();
      cout << endl;
    }
  else{
    cout << "No route from " << c.returnName() << " to " << c2.returnName() << endl;
  }

  //delete these dynamic arrays
  delete[] curDist;
  delete[] predecessor;
  delete[] locator;
}
