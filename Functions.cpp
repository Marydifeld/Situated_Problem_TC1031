#include "Functions.h"
#include<fstream>
#include<sstream>
#include<iostream>

void readFile(string fileName, vector<Order>& orders){
    /*
    Reads the txt file and initializes an Order object for every line. 
    */
    string line; 
    int index; 

    ifstream inFile(fileName);

    //Read data
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


void writeFile(vector<Order> & orders){
    ofstream outFile; 

    outFile.open("sortedOrders.txt");

    if(outFile.is_open()){
        for (int i = 0; i < orders.size(); i++){
            time_t t = orders[i].getDate();
            outFile << ctime(& t); 
            outFile << orders[i].getDate() << endl;

        }
    }
    else{
        cout << "Unable to open file for writing.";
    }
    outFile.close();
}

time_t pivot(vector<Order> &a, int right, int left) {
  int center = (left + right) / 2;
  if (a[center].getDate() < a[left].getDate()) {
    swap(a[center], a[left]);
  }
  if (a[left].getDate() > a[right].getDate()) {
    swap(a[right], a[left]);
  }
  if (a[right].getDate() < a[center].getDate()) {
    swap(a[center], a[right]);
  }
  swap(a[center], a[right - 1]);
  return a[right - 1].getDate();
}

void insertionSort(vector<Order> & a){ 
    int j; 
    for (int i = 1; i < a.size(); i++){ 
        time_t temp = a[i].getDate();
        for (j = i; j > 0 && a[j - 1].getDate() > temp; j--){
            a[j] = a[j - 1]; 
        }
        a[j].setDate(temp);
        
    }
}

void quicksort(vector<Order> &a, int left, int right){
    //discard small arrays 
    if (left + 10 <= right){
        const int & PIVOT = pivot(a, right, left);
        int i = left, j = right - 1; 
        for (;;){
            while (a[++i].getDate() < PIVOT){ }
            while (a[--j].getDate() > PIVOT) { }
            if (i < j ) {
                swap(a[i], a[j]);
            }else{
                break; 
            }
        } 
        //Pivot back in place
        swap(a[i], a[right - 1]);

        //Sort left and right partitions 
        quicksort(a, left, i - 1);
        quicksort(a, i + 1, right);
        
    } else {
        insertionSort(a);
    }
}

void quicksort(vector<Order> &a){
    quicksort(a, 0, a.size()-1);
}