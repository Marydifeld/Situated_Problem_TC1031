#ifndef ORDER_H
#define ORDER_H
#include<string> 
#include<ctime> 
using namespace std; 
class Order{
    private:
      time_t date;
      string restaurantName;
      string orderItem;
      float orderPrice;
    public: 
      Order(string restaurantName, string orderItem, float orderPrice);
      time_t getDate();
      string getName();
      string getOrderItem();
      float getOrderPrice();
      void setDate(string month, int day, int hour, int minute, int second);
};

#endif /*ORDER_H*/