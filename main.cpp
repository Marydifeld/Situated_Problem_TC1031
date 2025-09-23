#include<iostream>
#include<string>
#include <stdio.h>
#include <stdlib.h>
#include<fstream>
using namespace std; 
void readFile(){
    string sline; 
    int index; 

    ifstream inFile("orders.txt");
    ofstream outFile("out2.txt");

    //Check if it worked correctly
    if (inFile.is_open() && outFile.is_open() ){
        while ( getline(inFile, sline) ){
            cout << "Read:\t"<< sline << endl;
            outFile << sline << endl;
        }
    }
    inFile.close();
    outFile.close();
}

int main(){
    readFile();
    return 0; 
}

