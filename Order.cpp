#include "Order.h"
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
    string months[12] = {"ene", "Feb", "Mar", "Abr", "May" "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dic"};
    int i; 
    for (i = 0; months[i] != m; i++){}
    tm temp = {};
    temp.tm_year = 2025 - 1900;
    temp.tm_mon = i; 
    temp.tm_mday = d; 
    temp.tm_hour = hr; 
    temp.tm_min = min; 
    temp.tm_sec = sec; 
    temp.tm_isdst = -1;

    date = mktime(&temp);
}

void Order::setDate(time_t d){
    date = d; 
}