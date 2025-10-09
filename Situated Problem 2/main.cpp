#include<string> 
#include<vector>
#include<ctime> 
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>

//Order Class
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
      void setDate(time_t d);
      string orderToString();
};

// Declaring functions

void readFile(string fileName, vector<Order>& orders){

   /**
     * @brief Reads the txt file and initializes an Order object for every line. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * 
     * @param fileName Name of the file to be opened (Needs to end in .txt)
     * @param orders Will contain the Orders 
    */

    string line; 
    int index; 

    ifstream inFile(fileName);

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
            //Create Order
            orders.push_back(Order(name, order, price));
            orders[i].setDate(month, day, hour, min, sec);
            i++;
        }
    }
    inFile.close();
}


void writeFile(string fileName, vector<Order> & orders, int start, int end){
    /**
     * @brief Writes the output file in the correct format. We iterate between start and end, which is linear. 
     * If we assume to be taking the entire vector, complexity is O(n)
     * 
     * @param fileName The name of the output file (needs .txt)
     * @param orders Vector with all orders
     * @param start Start index of the orders to be written 
     * @param end End index of the orders to be written 
     */

    ofstream outFile; 

    outFile.open(fileName);
    

    if(outFile.is_open()){
        for (int i = start; i < end; i++){
            if (i < start + 10 ){
                cout << orders[i].orderToString(); 
            }
            outFile << orders[i].orderToString();
        }
    }
    else{
        cout << "Unable to open file for writing.";
    }
    outFile.close();
}

int binarySearchLower(vector<Order> a, time_t flag){
    /**
     * @brief Binary search adapted to find the date or the lowest index of where it could have
     * been placed in the sorted array. Very similar structure to binary date, which works by halving 
     * the array time and time again, which means O(logn)
     * 
     * @param a Contains the values to be searched
     * @param flag Value being searched
     * 
     * @return Index of the hipothetical position of the flag
     */
    int i = 0;
    int j = a.size(); 
    int m; 
    while(i < j){
        m = (i + j)/2;

        if (a[m].getDate() < flag){
            i = m + 1; 
        } 
        else{
            j = m; 
        }
    }
    return i; 

}

int binarySearchHigher(vector<Order> a, time_t flag){
    /**
     * @brief Binary search adapted to find the date or the highest index of where it could have
     * been placed in the sorted array. Very similar structure to binary date, which works by halving 
     * the array time and time again, which means O(logn)
     * 
     * @param a Contains the values to be searched
     * @param flag Value being searched
     * 
     * @return Index of the hipothetical position of the flag
     */
    int i = 0;
    int j = a.size(); 
    int m; 
    while(i < j){
        m = (i + j)/2;

        if (a[m].getDate() <= flag){
            i = m + 1; 
        } 
        else{
            j = m; 
        }
    }
    return i; 

}

time_t inputToTimeT(int month, int day, int hour = 0, int minute = 0, int second = 0){
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
    temp.tm_year = 2025 - 1900;
    temp.tm_mon = month; 
    temp.tm_mday = day; 
    temp.tm_hour = hour; 
    temp.tm_min = minute; 
    temp.tm_sec = second; 
    temp.tm_isdst = -1;

    return mktime(&temp);
}

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

int main(){
    //Read & Sort
    vector<Order> orders;
    readFile("orders.txt", orders);
    cout << "Reading File..." << endl; 
    cout << "------------ First 10 sorted entries --------------" << endl; 
    //quicksort(orders);
    writeFile("sortedOrders.txt", orders, 0, orders.size());
    cout << "------------ File read and sorted -----------------" << endl; 

    //Console
    string start_date, end_date; 
    cout << "Welcome to the search tool by date" << endl;
    cout << "Please be mindful of the requested format. This tool currently doesn't support years. (dd/mm)" << endl;
    cout << "Enter start and end dates separated by a blank space. (dd/mm dd/mm)" << endl; 
    cin >> start_date >> end_date; 
    time_t start, end; 

    start = inputToTimeT(stoi(start_date.substr(3, 2)) - 1, stoi(start_date.substr(0, 2)));
    end = inputToTimeT(stoi(end_date.substr(3, 2)) - 1, stoi(end_date.substr(0, 2)), 23, 59, 59);

    //Search
    int start_index = binarySearchLower(orders, start);
    int end_index = binarySearchHigher(orders, end);

    char ans; 
    cout << "Would you like to save the results in a separate File? (y/n)";
    cin >> ans; 
    if (ans == 'y'){
        cout << "------------ First 10 entries of search --------------" << endl;
        writeFile("search_results.txt", orders, start_index, end_index);
    }
    else if (ans == 'n'){
        for (int i = start_index; i <= end_index; i++){
            cout << orders[i].orderToString();
        }
    }
    
    




    return 0; 
}