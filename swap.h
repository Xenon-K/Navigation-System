/********************************
The following mySwap works like the swap function in the library.

Your name:Ji Guo
Your programmer number:15
Hours spent:50 seconds
Any difficulties?:No
********************************/

/****************
This function will swap the parameters

n1 can be any data type
n2 can be any data type
 ****************/
template <class T>
void mySwap(T&n1, T&n2)
{
  T temp;//keep a value from parameter

  //swap parameters
  temp = n1;
  n1 = n2;
  n2 = temp;
  return;
}
