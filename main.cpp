#include "Order.h"
#include<iostream>
#include<sstream>
#include<string>
#include<vector> 
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
using namespace std; 
vector<Order> orders; 

void readFile(){
    /*
    Reads the txt file and initializes an Order object for every line. 
    */
    string line; 
    int index; 

    ifstream inFile("orders.txt");

    //Check if it worked correctly
    if (inFile.is_open()){ 
        int i = 0;
        while ( getline(inFile, line) ){
            //Temporary variables
            string month, name, order, rest; 
            char extra; 
            int day, hour, min, sec; 
            float price; 
            istringstream iss(line);
            //Diveide line
            iss >> month >> day >> hour >> extra >> min >> extra >> sec;  
            getline(iss, rest);
            rest.erase(0, 3);
            size_t pos = rest.find("O:");
            size_t pos2 = rest.find("(");
            size_t pos3 = rest.find(")");
            name = rest.substr(0, pos - 1);
            order = rest.substr(pos + 2, pos2 - (pos + 2));
            price = strtof(rest.substr(pos2 + 1, pos3 - (pos2 + 1)).c_str(), nullptr);
            //Create Oreder
            orders.push_back(Order(name, order, price));
            orders[i].setDate(month, day, hour, min, sec);
            i++;
        }
    }
    inFile.close();
}



int main(){
    readFile();
    cout << orders[0].getOrderItem();
    return 0; 
}

