#include "lib.hpp"
#include <iostream>

using namespace std;

int main() {

  int num_1, num_2, result;

  cout << "Enter num_1 " << endl;
  cin >> num_1;

  cout << "Enter num_2" << endl;
  cin >> num_2;

  result = num_1 + num_2;
    cout << num_1 <<  " + " <<  num_2 << " = " << result << endl; 
  result = num_1 - num_2;
    cout << num_1 <<  " - " <<  num_2 << " = " << result << endl; 
  result = num_1 * num_2;
    cout << num_1 <<  " * " <<  num_2 << " = " << result << endl; 
  result = num_1 / num_2;
    cout << num_1 <<  " / " <<  num_2 << " = " << result << endl; 

  return 0;
}