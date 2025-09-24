#include "Order.h"
#include "functions.h"
#include<iostream>
#include<sstream>
#include<string>
#include<vector> 
#include<fstream>

vector<Order> orders; 

int main(){
    readFile("orders.txt", orders);
    quicksort(orders);
    writeFile(orders);

    return 0; 
}

