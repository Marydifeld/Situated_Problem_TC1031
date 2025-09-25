#include "Order.h"
#include "functions.h"
#include<iostream>
#include<sstream>
#include<string>
#include<vector> 
#include<fstream>

vector<Order> orders; 

int main(){
    //Read & Sort
    readFile("orders.txt", orders);
    quicksort(orders);
    writeFile("sortedOrders.txt", orders, 0, orders.size());
    

    //Console
    string start_date, end_date; 
    cout << "Wellcome to the search tool by date" << endl;
    cout << "Please be mindfull of the requested format. This tool currently doesn't support years. (dd/mm)" << endl;
    cout << "The end date is exclusive." << endl;
    cout << "Enter start and end dates separated by a blank space. (dd/mm dd/mm)" << endl; 
    cin >> start_date >> end_date; 
    
    time_t start = inputToTimeT(stoi(start_date.substr(3, 2)) - 1, stoi(start_date.substr(0, 2)));
    time_t end = inputToTimeT(stoi(end_date.substr(3, 2)) - 1, stoi(end_date.substr(0, 2)));

    //Search
    int start_index = binarySearchLower(orders, start);
    int end_index = binarySearchHigher(orders, end);

    char ans; 
    cout << "Would you like to save the results in a separate File? (y/n)";
    cin >> ans; 
    if (ans == 'y'){
        writeFile("search_results.txt", orders, start_index, end_index);
    }
    else if (ans == 'n'){
        for (int i = start_index; i <= end_index; i++){
            cout << orders[i].orderToString();
        }
    }
    
    




    return 0; 
}

