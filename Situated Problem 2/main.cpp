#include<string> 
#include<vector>
#include<ctime> 
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
//------------------------------- Classes --------------------------------------//
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

//Doubly linked list Class

//Stack Class

//----------------------------------------Declaring Order Methods--------------------------------//
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


//---------------------------------Declaring Doubly linked list Methods--------------------------------//

//--------------------------------------Declaring Stack Methods---------------------------------------//

//--------------------------------------- Auxiliary functions ----------------------------------------//
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

//SORTING ALGORITHMS 

//Parttition

//iterative_quicksort

time_t pivot(vector<Order> &a, int right, int left) {
    /**
     * @brief Picks a pivot for quicksort (median of three). No iterations so, constant time O(1)
     * 
     * @param a Vector that in which we need to pick the pivot
     * @param right Index of the start of the subarray with the pivot
     * @param left Index of the end of the subarray with the pivot
     * 
     * @return A time_t value representing the pivot for quicksort
     */
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
    /**
     * @brief The insertion sort algorithm. It works by setting aside an unsorted number
     * and pushing the sorted numbers forward until it can fit. This is a complementary 
     * function to quicksort. Since we iterate once for every element, complexity is O(n^2)
     * 
     * @param a The array to be sorted 
     */
    int j; 
    for (int i = 1; i < a.size(); i++){ 
        time_t temp = a[i].getDate();
        for (j = i; j > 0 && a[j - 1].getDate() > temp; j--){
            a[j] = a[j - 1]; 
        }
        a[j].setDate(temp);
        
    }
}

void recursive_quicksort(vector<Order> &a, int left, int right){
    /**
     * @brief Helper function of quicksort(). Uses pivot to find the median of three and sorts
     * the numbers by moving the ones that are smaller than pivot to the left and bigger ones to the right. 
     * It returns the pivot into place and repeats this process in smaller halves recurssivly. Because
     * the array is halved, complexity is usually O(nlogn), but worst case it's O(n^2).
     * 
     * @param a The array to be sorted 
     * @param left The start of the current subarray 
     * @param right The end of the current subarray 
     */
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
        recursive_quicksort(a, left, i - 1);
        recursive_quicksort(a, i + 1, right);
        
    } else {
        insertionSort(a);
    }
}

void recursive_quicksort(vector<Order> &a){
    recursive_quicksort(a, 0, a.size()-1);
}

//SEARCH ALGORITHMS

int binarySearchLower(vector<Order> a, time_t flag){
    /**
     * @brief Binary search adapted to find the date or the lowest index of where it could have
     * been placed in the sorted array. Very similar structure to binary date, which works by halving 
     * the array time and time again, which means O(logn). Made to find a date 
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
     * the array time and time again, which means O(logn). Made to find a date 
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

//Binary search higher for strings
//Binary search lower for strings 



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