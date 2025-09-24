#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Order.h"
#include<string>
#include<vector> 

using namespace std; 
void readFile(string fileName, vector<Order> & orders);
void writeFile(vector<Order> & orders);
time_t pivot(vector<Order> &a, int right, int left);
void insertionSort(vector<Order> & a);
void quicksort(vector<Order> &a, int left, int right);
void quicksort(vector<Order> &a);

#endif /*FUNCTIONS_H*/