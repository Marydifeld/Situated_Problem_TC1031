#include<string> 
#include<vector>
#include<ctime> 
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
using namespace std; 
using std::cout; 

//Initialize helper functions used in classes
time_t inputToTimeT(int month, int day, int hour = 0, int minute = 0, int second = 0); 
//------------------------------- Classes --------------------------------------//


class Order{
    private:
      time_t date;
      string orderItem;
      float orderPrice;
    public: 
      Order(string orderItem, float orderPrice);
      time_t getDate() const;
      string getOrderItem();
      float getOrderPrice();
      void setDate(string month, int day, int hour, int minute, int second);
      void setDate(time_t d);
      bool operator==(const Order& other) const;
};

class Restaurant{
    private:
        string restaurantName;
        vector<Order> orders;
    public:
        Restaurant(string name){restaurantName = name;}
        string getName(){return restaurantName;}
        void addOrder(Order order){orders.push_back(order);}
        vector<Order> getOrders(){return orders;}
};

//----------------------------------------Declaring Order Methods--------------------------------//
Order::Order(string order, float price){
    date = 0; 
    orderItem = order; 
    orderPrice = price; 
}

time_t Order::getDate() const{
    return date; 
}

string Order::getOrderItem(){
    return orderItem;
}
float Order::getOrderPrice(){
    return orderPrice;
}
void Order::setDate(string m, int d, int hr, int min, int sec){
    string months[12] = {"Jan", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec"};
    if (m == "Ene") {
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

bool Order::operator==(const Order& other) const{
    return (this->date == other.date &&
            this->orderItem == other.orderItem &&
            this->orderPrice == other.orderPrice);
}

// ----------------------------------------Auxiliary functions----------------------------------------//
time_t inputToTimeT(int month, int day, int hour, int minute, int second){
    /**
     * @brief A helper function to turn input into time_t. It's only conversion so O(1)
     * 
     * @param month Month of the date
     * @param day Day of the given date 
     * @param hour Hour of the given time (Defaults to 0)
     * @param minute Minute of the given time (Defaults to 0)
     * @param second Second of the given time (Defaults to 0)
     * 
     * @return The time_t equivalent of the date given 
     */
    tm temp = {};
    temp.tm_isdst = 0;
    temp.tm_year = 2025 - 1900;
    temp.tm_mon = month; 
    temp.tm_mday = day; 
    temp.tm_hour = hour; 
    temp.tm_min = minute; 
    temp.tm_sec = second; 
    temp.tm_isdst = -1;

    return mktime(&temp);
}

string orderToString(Restaurant& restaurant, Order& order){
     //Formating time
    time_t dt = order.getDate();
    tm* lt = localtime(&dt);
    char fdate[80];
    strftime(fdate, sizeof(fdate), "%b %d %H:%M:%S", lt);
    

    //Formating rest 
    ostringstream oss;
    oss << fdate << " R:" << restaurant.getName() << " O:" << order.getOrderItem() << 
        "(" << fixed << setprecision(1) << order.getOrderPrice() << ")" << endl; 
    return oss.str(); 
}







int main(){
    return 0;
}
