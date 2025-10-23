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
//Order Class
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
      bool operator==(const Order& other) const;
};
//ListNode struct (Needs to be acces by other functions)
struct ListNode {
            Order value; 
            ListNode* next;
            ListNode* prev; 
            ListNode(const Order& element, ListNode* n = NULL, ListNode* p = NULL)
                : value(element), next(n), prev(p) { }
        };
//Doubly linked list Class
class DoublyLinkedList {
    private: 
        
        ListNode* first = NULL;
        ListNode* last = NULL; 
        int size; 
                                            
    public: 
        DoublyLinkedList(){
                this->first = NULL; 
                this->last = NULL; 
                this->size = 0;
        }
        ~DoublyLinkedList(){
            while(!isEmpty())
        		deleteLast();
        }

        ListNode* getLast(){return last;}
        ListNode* getFirst(){return first;}
        int getSize(){return size;}
        ListNode* getAtIndex(int);
		void showList();
        bool isEmpty();
		
		void insertFirst(Order);		
		void insertLast(Order);
		bool insertAtIndex(int, Order); 	

		void deleteFirst();
		void deleteLast();				
		void deleteAtIndex(int);
        void swap(ListNode*, ListNode*);		

		ListNode* find(Order, int*);			
		void update(int, Order);	

};

//Stack Class
class Stack {
    struct StackNode{
        ListNode* value;
        StackNode* next; 
        StackNode(ListNode* element, StackNode* n = NULL)
            : value(element), next(n) { }
    };

    int size = 0;
    StackNode* top; 
    public:
		Stack(){	
			top = NULL;
		}

		~Stack(){
    		while(!isEmpty())
        		pop();
		}
		
		bool push (ListNode* value);
		ListNode* pop();
		int getSize();
		ListNode* getTop();
		bool isEmpty();
		void show();
};

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
bool Order::operator==(const Order& other) const{
    return (this->date == other.date &&
            this->restaurantName == other.restaurantName &&
            this->orderItem == other.orderItem &&
            this->orderPrice == other.orderPrice);
}

//---------------------------------Declaring Doubly linked list Methods--------------------------------//
ListNode* DoublyLinkedList::getAtIndex(int index){
    int i = 0; 
    if (index >= size || index < 0){
        return nullptr;
    }
    ListNode* aux = first; 
    for (i; i != index; i++){
        aux = aux->next;
    }
    return aux;  
}

void DoublyLinkedList::insertFirst(Order newValue){
/**
 * @brief inserts a value at the start of a list. O(1)
 * 
 * @param newValue The value T to be inserted in the list 
 */
	ListNode* node = new ListNode(newValue);
	ListNode* aux = first;

	node->next = aux;
	first = node;

	// ensure last is defined
	if (size == 0)
		last = node;
	else 
	    aux->prev = first; 
	
	//preserve a circular list
	first->prev = last;
	last->next = first; 
	size++;
}

void DoublyLinkedList::insertLast(Order newValue){
/**
 * @brief inserts a value at the end of the list. O(1)
 * 
 * @param newValue The value T to be inserted in the list 
 */
	ListNode* node = new ListNode(newValue);
	ListNode* secondLast = last;

	node->prev = secondLast;
	last = node;

	// ensure if "first" is defined
	if (size == 0)
	   first = node;
	else 
	   secondLast->next = last;

	//preserve a circular list
	first->prev = last;
	last->next = first;

	size++;
}

void DoublyLinkedList::showList(){
/**
 * @brief Couts the size and the values of in the list. O(n)
 */
	cout<< "Size: " << size << endl;

	ListNode* aux = first;
	int i = 0;
	while(i < size){
		cout << aux->value.orderToString();
		aux = aux->next;
		i++;
	}
	cout << endl;
}

bool DoublyLinkedList::insertAtIndex(int index, Order newValue){	
/**
 * @brief inserts a new Value at a specified index. O(n)
 * 
 * @param index The index where the vaule is to be inserted 
 * @param newValue The value to be inserted
 * 
 * @return A bool depending on if the insetion was successful 
 */
	if (index == 0){
		insertFirst(newValue);
		return true; 
	}
	else if (index == size){
		insertLast(newValue);
		return true; 
	}
	else if (index < size && index >= 0){
		int i = 0; 
		ListNode* aux = first; 
		while (i != index - 1){
			aux = aux->next;
			i ++; 
		}
		ListNode* node = new ListNode(newValue);
		node->next = aux->next;
		aux->next = node; 
		node->prev = aux; 
		node->next->prev = node;
		size++;
		return true;

	}
	return false; 
}

ListNode* DoublyLinkedList::find(Order value, int* index){	
/**
 * @brief Find a specific value and passes the index by pointer O(n)
 * 
 * @param value The value that is being looked for 
 * @param index A pointer to the index, will be modified to contain the
 * index where the value is found 
 * @return The node that was looked for 
 */
	ListNode* aux = first; 
	*index = -1; 
	int i = 0; 
	while (i < size){
		if (value == aux->value){
			*index = i; 
			return aux; 
		}
		aux = aux->next;
		i++; 
	}
	return NULL; 
}

void DoublyLinkedList::deleteFirst(){
/**
 * @brief Deletes the first item of the list, O(1). No inputs or outputs. 
 */
	if (size == 1){
		delete first; 
		size--; 
		first = nullptr; 
		last = nullptr; 

	}
	else if (size != 0){
		ListNode* aux = first; 
		first = aux->next; 
		first->prev = last;
		last->next = first; 
		delete aux; 
		size--; 
	}
}

void DoublyLinkedList::deleteLast(){
	/**
 * @brief Deletes the last item of the list, O(1). No inputs or outputs.
 */
    if (size == 1){
        delete last;
        size--;
        first = NULL;   // en lugar de nullptr
        last = NULL;    // en lugar de nullptr
    }
    else if (size != 0){
        ListNode* aux = last;
        last = aux->prev;
        last->next = first;
        first->prev = last;
        delete aux;
        size--;
    }
}

void DoublyLinkedList::deleteAtIndex(int index){
/**
 * @brief Deletes a node at a specified index, O(n).
 * 
 * @param index The index of the node to delete
 */
    if (index < 0 || index >= size){
        return; 
    }
    else if (index == 0){
        deleteFirst();
    }
    else if (index == size - 1){
        deleteLast();
    }
    else{
        int i = 0;
        ListNode* aux = first;
        while (i != index){
            aux = aux->next;
            i++;
        }
        aux->prev->next = aux->next;
        aux->next->prev = aux->prev;
        delete aux;
        size--;
    }
}

void DoublyLinkedList::update(int index, Order newValue){
/**
 * @brief Updates the value at a specific index, O(n).
 * 
 * @param index The index of the node to update
 * @param newValue The new value to set
 */
    if (index < 0 || index >= size){
        return;
    }
    int i = 0;
    ListNode* aux = first;
    while (i != index){
        aux = aux->next;
        i++;
    }
    aux->value = newValue;
}

bool DoublyLinkedList::isEmpty(){
    return (size == 0) ? true : false; 
}

void DoublyLinkedList::swap(ListNode* a, ListNode* b){
/**
 * @brief Swaps the nodes a and b, only pointer manipulation O(1)
 * 
 * @param a 
 * @param b 
 * 
 */

     if (a == b) return;
     ListNode* aPrev = a->prev;
     ListNode* aNext = a->next;
     ListNode* bPrev = b->prev;
     ListNode* bNext = b->next;
     
     if(aNext == b){
        a->next = bNext;
        a->prev = b;
        b->next = a;
        b->prev = aPrev;
     }   
     else if(bNext == a){
        b->next = aNext;
        b->prev = a;
        a->next = b;
        a->prev = bPrev;
     }
     else {
        a->next = bNext;
        a->prev = bPrev;
        b->next = aNext;
        b->prev = aPrev;
     }

     aPrev->next = b;
     aNext->prev = b;
     bPrev->next = a;
     bNext->prev = a;

}

//--------------------------------------Declaring Stack Methods---------------------------------------//
bool Stack::push(ListNode* value){
	/**
	 * @brief Adds a new value on top of the stack. O(1)
	 * 
	 * @param value The value to be added
	 * 
	 * @return Only if push was successful
	 */

	StackNode* newNode = new StackNode(value);
	newNode->next = top;  
	top = newNode; 
	size++;
	return true; 	
	
}

ListNode* Stack::pop(){
	/**
	 * @brief Deletes the top value of the stack, O(1)
	 * 
	 * @return The top value of the stack 
	 */
    

    StackNode* aux = top;
    ListNode* val = top->value;
    top = top->next;
    delete aux; 
    size--;
	
	return val; 
}

void Stack::show(){
	/**
	 * @brief shows all items in the stack, O(n)
	 */
	StackNode* aux = top; 
	if (isEmpty()){
		cout << "Stack is empty" << endl;
	} else {
		for (int i = 0; aux != NULL; i++){
			cout << "Stack[" << i << "] = " << aux->value->value.orderToString() << endl; 
			aux = aux->next; 
		}
	}
}

bool Stack::isEmpty(){
/** 
 * @brief Check if the stack is empty O(1)
 * 
 * @return true if top is NULL, false otherwise
*/	
    return top == NULL;
}

int Stack::getSize(){
/**
 * @brief Get the number of elements in the stack O(1)
 * 
 * @return int number of elements
 */
	return size; 
}

ListNode* Stack::getTop(){	
/**
 * @brief Get the top value of the stack without removing it O(1)
 * 
 * @return int value at the top
 */
    return top->value;  
}

//--------------------------------------- Auxiliary functions ----------------------------------------//
void readFile(string fileName, vector<Order>& orders){

   /**
     * @brief Reads the txt file and initializes an Order object for every line. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * This overload saves it to a vector.
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

void readFile(string fileName, DoublyLinkedList& orders){
    /**
     * @brief Reads the txt file and initializes an Order object for every line. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * This overload saves it to a linked list. 
     * 
     * @param fileName Name of the file to be opened (Needs to end in .txt)
     * @param orders Will contain the Orders 
    */
   
    string line; 

    ifstream inFile(fileName);

    if (inFile.is_open()){ 
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
            orders.insertLast(Order(name, order, price));
            orders.getLast()->value.setDate(month, day, hour, min, sec);
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

//void writeFile(string fileName,DoublyLinkedList & orders2, int start, int end){}

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

//Partition
ListNode* partition(DoublyLinkedList& a, ListNode* left, ListNode* right){
    /**
     * @brief Partition function for iterative quicksort.
     * 
     * @param a 
     * @param left The starting index of the array to be partitioned
     * @param right The ending index of the array to be partitioned
     * 
     * @return The index of the pivot 
     */
    ListNode* pivot = right; 
    ListNode* i = nullptr;

    for (ListNode* j = left; j != right; j = j->next){
        if (j->value.getName() <= pivot->value.getName()){
            if (i == nullptr){
                i = left; 
            }
            else {
                i = i->next; 
            }
            a.swap(i, j);
        }

    } 
    if (i == nullptr){
        i = left;
    }
    else {
        i = i->next;
    }
    a.swap(i, pivot);

    return i; 
    
}

//iterative_quicksort

void iterative_quicksort(DoublyLinkedList &a, ListNode* left, ListNode* right){
    Stack s;
    s.push(left);
    s.push(right);
    while (!s.isEmpty()){
        right = s.pop();
        left = s.pop();
        ListNode* pivot = partition(a, left, right);

        if (left != pivot->prev && left != pivot){
            s.push(left);
            s.push(pivot->prev);

        }
        if (right != pivot->next && right != pivot){
            s.push(pivot->next);
            s.push(right);
        }
    }



}

//Pivot for recursive sort 
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

//Insertion Sort
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

//Recursive quicksort
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
//int binarySearchHighStrings(arr[], string restaurantName){ }

//Binary search lower for strings 


int main(){
    //Read & Sort
    /*
    vector<Order> orders;
    
    recursive_quicksort(orders);
    readFile("orders.txt", orders);
    

    
    cout << "Reading File..." << endl; 
    cout << "------------ First 10 sorted entries --------------" << endl; 
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
        //writeFile("search_results.txt", orders, start_index, end_index);
    }
    else if (ans == 'n'){
        for (int i = start_index; i <= end_index; i++){
            cout << orders[i].orderToString();
        }
    }
    
    */

    DoublyLinkedList orders2; 
    readFile("orders.txt", orders2);
    orders2.showList();




    return 0; 
}