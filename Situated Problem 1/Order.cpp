#include "Order.h"
#include "Functions.h"
#include <iomanip>
#include<iostream>


Order::Order(string name, string order, float price){
    date = 0; 
    restaurantName = name; 
    orderItem = order; 
    orderPrice = price; 
}
time_t Order::getDate(){
    return date; 
}

string Order::getName(){
    return restaurantName;
}

string Order::getOrderItem(){
    return orderItem;
}

float Order::getOrderPrice(){
    return orderPrice;
}

void Order::setDate(string m, int d, int hr, int min, int sec){
    string months[12] = {"Jan", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec"};
    if (m == "ene") {
        m = "Jan";
    }
    else if (m == "Dic"){
        m = "Dec";
    }

    int i; 
    for (i = 0; months[i] != m; i++){}
    
    date = inputToTimeT(i, d, hr, min, sec);

}

void Order::setDate(time_t d){
    date = d; 
}

string Order::orderToString(){
     //Formating time
    tm* lt = localtime(&date);
    char fdate[80];
    strftime(fdate, sizeof(fdate), "%b %d %H:%M:%S", lt);
    

    //Formating rest 
    ostringstream oss;
    oss << fdate << " R:" << restaurantName << " O:" << orderItem << 
        "(" << fixed << setprecision(1) << orderPrice << ")" << endl; 
    return oss.str(); 
}