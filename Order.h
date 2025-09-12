#ifndef ORDER_H
#define ORDER_H
class Order{
    private:
      string date;
      string restaurantName;
      int hour;
      int minute;
      int second;
      string orderItem;
      int orderPrice;
    public: 
      Order(string date, string restaurantName, int hour, int minute, int second, string orderItem, int orderPrice);
      getDate();
      getrestaurantName();
      getHour();
      getminute();
      getsecond();
      getorderItem();
      getorderPrice();
      sethour(int hour, int minute, int second);

};

#endif /*ORDER_H*/