#include<string> 
#include<vector>
#include<ctime> 
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
#include <algorithm> 
using namespace std; 

//------------------------------- Classes --------------------------------------//
//Order Class
class Order{
    private:
      time_t date;
      string orderItem;
      float orderPrice;
    public: 
      Order(time_t d, string order, float price){
        date = d; 
        orderItem = order; 
        orderPrice = price; 
    }
      time_t getDate() {return date;}
      string getOrderItem() {return orderItem;}
      float getOrderPrice() {return orderPrice;}

      bool operator==(const Order& other) const{
    return (this->date == other.date &&
            this->orderItem == other.orderItem &&
            this->orderPrice == other.orderPrice);
}

     
};

class Restaurant{
    private: 
        string restaurantName;
        vector<Order> orders; 
        int totalOrders = orders.size();
    public: 
        Restaurant(){
            restaurantName = "";
        }
        Restaurant(string name){
            restaurantName = name; 
        }
        
        string getName() {return restaurantName;}
        int getTotalOrders() {return totalOrders;}
        float getTotalCost() {
            float cost = 0; 
            if (totalOrders == 0) {
                return cost; 
            }
            for (int i = 0; i < totalOrders;i++){
                cost = cost + orders[i].getOrderPrice();
            }
            return cost; 
        }
        
        void addOrder(Order b) {orders.push_back(b);}
        void updateOrders() {totalOrders = orders.size();}

        bool operator==(const Restaurant& other) const{
           return this->restaurantName == other.restaurantName;
        }
};

class NodeTree {
public:
    Restaurant data;
    NodeTree* left;
    NodeTree* right;
    NodeTree* up;
    int height;

    NodeTree(Restaurant value) {
        data = value;
        left = right = up = nullptr;
        height = 1;
    }
};

class AVLTree {
private:
    NodeTree* root;
    string typeOfSort; 

    int height(NodeTree* node) {
        if (node == nullptr)
            return 0;
        return node->height;
    }

    int getBalance(NodeTree* node) {
        if (node == nullptr)
            return 0;
        return height(node->left) - height(node->right);
    }

    NodeTree* rotateRight(NodeTree* y) {
        NodeTree* x = y->left;
        NodeTree* T2 = x->right;

        x->right = y;
        y->left = T2;

        if (T2 != nullptr)
            T2->up = y;

        x->up = y->up;

        if (y->up != nullptr) {
            if (y->up->left == y)
                y->up->left = x;
            else if (y->up->right == y)
                y->up->right = x;
        }

        y->up = x;
         if (!T2) y->left = nullptr;
        y->height = 1 + max(height(y->left), height(y->right));
        x->height = 1 + max(height(x->left), height(x->right));

        return x;
    }


    NodeTree* rotateLeft(NodeTree* x) {
        NodeTree* y = x->right;
        NodeTree* T2 = y->left;

        y->left = x;
        x->right = T2;

        if (T2 != nullptr)
            T2->up = x;

        y->up = x->up;

        if (x->up != nullptr) {
            if (x->up->left == x)
                x->up->left = y;
            else if (x->up->right == x)
                x->up->right = y;
        }
   
        x->up = y;

        if (!T2) x->right = nullptr;

        x->height = max(height(x->left), height(x->right)) + 1;
        y->height = max(height(y->left), height(y->right)) + 1;

        return y; 
    }

public:
    AVLTree(string tos) {
        root = nullptr;
        typeOfSort = tos; 
    }
    vector<string> formated; 

//Drivers (Recurssive)
    void remove(Restaurant key) {
        root = deleteNode(root, key);
    }
    void insert(Restaurant value) {
        root = insertNode(root, value);
        if (root) root->up = nullptr; 
    }
    void show() {
        cout << "Tree elements: ";
        show(root);
        cout << endl;
    }
    AVLTree makeNewTree(string sortKey) {
        AVLTree aux(sortKey);
        makeNewTree(aux, root);
        return aux; 
    }
    void restaurantToString() {
        restaurantToString(root);
    }

//Chosing how to sort the tree
bool isLessThan(Restaurant & a, Restaurant & b) {
/**
 * @brief Helper function that can compare diffrent attributes of the restaurant depending
 * on the sort key. Corresponds to (a < b)
 * 
 * @param a First restaurant 
 * @param b second restaurant 
 * 
 * @return If (a < b)
 */
    if (typeOfSort == "by_name") {
        return a.getName() < b.getName();
    }
    else if (typeOfSort == "by_orders") {
        return a.getTotalOrders() < b.getTotalOrders(); 
    }
    else if (typeOfSort == "by_sales") {
        return a.getTotalCost() < b.getTotalCost(); 
    }
    else {
        throw runtime_error("Invalid key");
    }
}

bool isEqual(Restaurant & a, Restaurant & b) {
/**
 * @brief Helper function that can compare diffrent attributes of the restaurant depending
 * on the sort key. Corresponds to (a < b)
 * 
 * @param a First restaurant 
 * @param b second restaurant 
 * 
 * @return If (a < b)
 */
    if (typeOfSort == "by_name") {
        return a.getName() == b.getName();
    }
    else if (typeOfSort == "by_orders") {
        return a.getTotalOrders() == b.getTotalOrders(); 
    }
    else if (typeOfSort == "by_sales") {
        return a.getTotalCost() == b.getTotalCost(); 
    }
    else {
        throw runtime_error("Invalid key");
    }
}

    NodeTree* deleteNode(NodeTree* root, Restaurant key) {
    /**
     * @brief Deletes a Node an balances the tree. O(log n), because it only traverses a path, not the whole tree
     * 
     * @param root The root of the recursions subtree
     * @param key The value of the node to be deleted
     * 
     * @returns The new root
     */
        if (root == nullptr)
            return root;

        if (key.getName() < root->data.getName()) {
            root->left = deleteNode(root->left, key);
            if (root->left) root->left->up = root;
        }
        else if (key.getName() > root->data.getName()) {
            root->right = deleteNode(root->right, key);
            if (root->right) root->right->up = root;
        }
        else {
            if (root->left == nullptr || root->right == nullptr) {
                NodeTree* temp = root->left ? root->left : root->right;

                if (temp == nullptr) {
                    // Case without children
                    temp = root;
                    root = nullptr;
                } else {
                    // One child case
                    temp->up = root->up;
                    *root = *temp;
                }
                delete temp;
            } 
            else {
                // Node with two children
                NodeTree* temp = root->right;
                while (temp->left != nullptr)
                    temp = temp->left;

                root->data = temp->data;  
                root->right = deleteNode(root->right, temp->data);
                if (root->right) root->right->up = root;
            }
        }

        if (root == nullptr)
            return root;

        root->height = 1 + max(height(root->left), height(root->right));

        int balance = getBalance(root);

        if (balance > 1 && getBalance(root->left) >= 0)
            return rotateRight(root);

        if (balance > 1 && getBalance(root->left) < 0) {
            root->left = rotateLeft(root->left);
            return rotateRight(root);
        }

        if (balance < -1 && getBalance(root->right) <= 0)
            return rotateLeft(root);

        if (balance < -1 && getBalance(root->right) > 0) {
            root->right = rotateRight(root->right);
            return rotateLeft(root);
        }

        return root;
    }

    NodeTree* insertNode(NodeTree* node, Restaurant value) {
    /**
     * @brief Inserts node in its intended position for a BST and balances the tree. O(log n). 
     * Duplicates are inserted on the right. 
     * 
     * @param node The new Node to be inserted
     * @param value The value of the new node
     * 
     * @return The root of the tree after being balanced
     */

        if (node == nullptr)
            return new NodeTree(value);

        if (isLessThan(value, node->data)) { 
            node->left = insertNode(node->left, value);
            node->left->up = node; 
        }
        else if (isLessThan(node->data, value)){ 
            node->right = insertNode(node->right, value);
            node->right->up = node; 
        }
        else {
            node->right = insertNode(node->right, value); 
            node->right->up = node;
        }
        node->height = 1 + max(height(node->left), height(node->right));

        int balance = getBalance(node);

        if (balance > 1 && isLessThan(value, node->left->data))
            return rotateRight(node);
        if (balance < -1 && !isLessThan(value, node->right->data))
            return rotateLeft(node);
        if (balance > 1 && isLessThan(node->left->data, value)) {
            node->left = rotateLeft(node->left);
            node->left->up = node; 
            return rotateRight(node);
        }
        if (balance < -1 && isLessThan(value, node->right->data)) {
            node->right = rotateRight(node->right);
            node->right->up = node; 
            return rotateLeft(node);
        }

        return node;
    }

    void show(NodeTree* node) {
    /**
     * @brief Shows the tree by depth. It starts with the leftmost leaf node and goes up. O(n)
     * 
     * @param node The root of the recursions Tree
     */
        if (node == nullptr) return;
        show(node->left);
        cout << node->data.getName() << " ";
        show(node->right);
    }

    void restaurantToString(NodeTree* node){
    /**
     * @brief Traverses all restaurants to later format their orders O(n)
     * 
     * @param node The root of the recursions Tree
     */
        if (node == nullptr) return;
        ostringstream oss; 
        restaurantToString(node->left);

        oss << "Name: " << node->data.getName() << " Total Orders: " << node->data.getTotalOrders()
        << " Acumulated Sales: " << node->data.getTotalCost() << endl;
        formated.push_back(oss.str());
        
        restaurantToString(node->right);
    }
    NodeTree* search(Restaurant key) {
    /**
     * @brief Performs a search in O(log n). It is guaranteed because the tree is always balanced. 
     * 
     * @param key The name of the restaurant that is being looked for
     * 
     * @return A pointer to the node with the key 
     */
        if (root == nullptr) {
            return nullptr; 
        }

        NodeTree* aux = root; 

        while(aux != nullptr) {
            if (isEqual(aux->data, key)) {
                return aux; 
            }
            else if (isLessThan(key, aux->data)){
                aux = aux->left; 
            }
            else {
                aux = aux->right; 
            }
        }
        return nullptr; 
    }

    void makeNewTree(AVLTree &newTree, NodeTree* node){
        if (node==nullptr) return; 
        makeNewTree(newTree, node->left); 
        newTree.insert(node->data); 
        makeNewTree(newTree, node->right); 
    }

    NodeTree* getRoot(){ return root;}
    string getTypeOfSort() {return typeOfSort;}
};
//--------------------------------------- Auxiliary functions ----------------------------------------//
time_t inputToTimeT(string month, int day, int hour, int minute, int second){
    /**
     * @brief A helper function to turn input into time_t. It's only conversion so O(1)
     * 
     * @param month Month of the date in string, will be converted
     * @param day Day of the given date 
     * @param hour Hour of the given time (Defaults to 0)
     * @param minute Minute of the given time (Defaults to 0)
     * @param second Second of the given time (Defaults to 0)
     * 
     * @return The time_t equivalent of the date given 
     */

    string months[12] = {"Jan", "Feb", "Mar", "Abr", "May", "Jun", "Jul", "Ago", "Sep", "Oct", "Nov", "Dec"};
    if (month == "ene") {
        month = "Jan";
    }
    else if (month == "Dic"){
        month = "Dec";
    }

    int i; 
    for (i = 0; months[i] != month; i++){}

    tm temp = {};
    temp.tm_isdst = 0;
    temp.tm_year = 2025 - 1900;
    temp.tm_mon = i; 
    temp.tm_mday = day; 
    temp.tm_hour = hour; 
    temp.tm_min = minute; 
    temp.tm_sec = second; 
    temp.tm_isdst = -1;

    return mktime(&temp);
}

void readFile(string fileName, AVLTree& tree){

   /**
     * @brief Reads the txt file and initializes an Order object for every line. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * This overload saves it to a vector.
     * 
     * @param fileName Name of the file to be opened (Needs to end in .txt)
     * @param tree Will contain the Restaurants that each save their orders
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
            istringstream iss(line)
            ;
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
            
            //Create Restaurant
            Restaurant temp(name);
            NodeTree* res = tree.search(temp);
            if (!res) {
                tree.insert(temp);
                res = tree.search(name);  
            }
            Restaurant& aux = res->data; 

            //Add orders
            time_t date = inputToTimeT(month, day, hour, min, sec);
            aux.addOrder(Order(date, order, price));
            aux.updateOrders();
           
            
        }
    }
    inFile.close();
}

void writeFile(string fileName, AVLTree & tree){
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
        tree.restaurantToString(); 
        for (int i = 0; i < tree.formated.size(); i++) {
            if (i < 10) {
                cout << tree.formated[i]; 
            }
            outFile << tree.formated[i]; 
        }
        
    }
    else{
        cout << "Unable to open file for writing.";
    }
    outFile.close();
}

int main(){

    AVLTree tree("by_name");
    readFile("orders.txt", tree);

    int option = 0;

    while (option != 4){
        cout << "\n Menu Options: \n";
        cout << "1. Order by name\n";
        cout << "2. Order by number of orders\n";
        cout << "3. Order by total sales\n";
        cout << "4. Exit\n";
        cout << "Select an option: ";
        cin >> option;

        switch (option){
            case 1:{
                cout << "Ordering by name...\n";
                AVLTree tree1 = tree.makeNewTree("by_name");
                writeFile("sortedByName.txt", tree1);
                cout << "File 'sortedByName.txt' created successfully.\n";
                break;
            }
            case 2:{
                cout << "\nOrdering by number of orders...\n";
                AVLTree tree2 = tree.makeNewTree("by_orders"); 
                writeFile("sortedByOrders.txt", tree2); 
                cout << "File 'sortedByOrders.txt' created successfully.\n";
                break;
            }
            case 3:{
                cout << "\nOrdering by total sales...\n";
                AVLTree tree3 = tree.makeNewTree("by_sales"); 
                writeFile("sortedBySales.txt", tree3); 
                cout << "File 'sortedBySales.txt' created successfully.\n";
                break;
            }   
            case 4:
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid option, please try again.\n";
        }
    }
    return 0; 
}