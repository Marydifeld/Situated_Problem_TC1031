#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#include "Order.h"
#include<string>
#include<vector> 

using namespace std; 
//File management
void readFile(string fileName, vector<Order> & orders);
void writeFile(string fileName, vector<Order> & orders, int start, int end);

//Quicksort
time_t pivot(vector<Order> &a, int right, int left);
void quicksort(vector<Order> &a, int left, int right);
void quicksort(vector<Order> &a);

//Binary search
int binarySearchLower(vector<Order> a, time_t flag); 
int binarySearchHigher(vector<Order> a, time_t flag); 

//Helper functions
void insertionSort(vector<Order> & a);
time_t inputToTimeT(int month, int day,int hour = 0, int minute = 0, int second = 0);

#endif /*FUNCTIONS_H*/