#include<string> 
#include<vector>
#include<ctime> 
#include<iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
#include <limits>

using namespace std; 
int  numDishes, numRestaurants, numLines; 
//------------------------------- Classes --------------------------------------//
struct ListNode {
            int neighborID; 
            float weight; 
            ListNode* next;
            ListNode(const int& element, float p = -1 , ListNode* n = nullptr)
                : neighborID(element), weight(p), next(n) { }
        };
struct HeapNode {
        int nodeID; 
        float distance; 
        HeapNode(const int& index, float d)
        : nodeID(index), distance(d) { }
};
class LinkedList {
    private: 
        ListNode* first = nullptr;
        ListNode* last = nullptr; 
        int size; 
                                            
    public: 
        LinkedList(){
                this->first = nullptr; 
                this->size = 0;
        }

        ListNode* getFirst(){return first;}
        int getSize(){return size;}
		void showList();
		
		void insertFirst(ListNode* newNode) {

            if (!first){
                first = newNode; 
                return; 
            }

            newNode->next = first; 
            first = newNode; 
            return; 
        }	
		

		ListNode* find(int id){
            ListNode* aux = first; 
            while (aux != nullptr){
                if (aux->neighborID == id){
                    return aux; 
                }
               
                aux = aux->next;
            }
            return nullptr;
        }			

};
class MinHeap {
    public: 
        vector<HeapNode*> H; 
        int capacity; 
        int size;
        
        MinHeap(int cap){
            capacity = cap;
            H.resize(cap); 
            size = 0; 
        }

        int parent(int i) {return (i - 1)/2;}
        int leftChild(int i) {return (2*i + 1);}
        int rightChild(int i) {return (2*i + 2);}

        void insert(HeapNode* node){
        /**
         * @brief Inserts a new node into the heap and percolates it up to maintain heap property. O(log n)
         * 
         * @param node The node to be inserted
         */    
            if (size == capacity) return; 

            H[size] = node; 
            int i = size; 
            size++; 

            while (i > 0 && H[parent(i)]->distance > H[i]->distance) {
                swap(H[parent(i)], H[i]); 
                i = parent(i);
            }

        }

        void percolateDown(int i) {
        /**
         * @brief Percolates a node down the heap to maintain heap property. O(log n)
         * 
         * @param i The index of the node to be percolated down 
         */    
            int leftCh = leftChild(i); 
            int rightCh = rightChild(i); 
            int smallest = i;
            
            
            if (leftCh < size && H[leftCh]->distance < H[smallest]->distance) {
                smallest = leftCh; 
            }

            if (rightCh < size && H[rightCh]->distance < H[smallest]->distance){
                smallest = rightCh; 
            }

            if (smallest != i) {
                swap(H[i], H[smallest]); 
                percolateDown(smallest); 
            }


        }

        HeapNode* extractMin() {
            HeapNode* root = H[0];
            H[0] = H[size - 1];
            size--;
            percolateDown(0);
            return root;
        }

        bool isEmpty() {
            return size == 0; 
        }


};
class MenuGraph {
    public: 
        vector<string> restaurants; 
        vector<string> dishes; 

        vector<LinkedList> restAdj; 
        vector<LinkedList> dishAdj; 
    
        MenuGraph(){ }

    int getRestaurantIndex(string name){
    /**
     * @brief Finds the index of a specified restaurant in the restaurants 
     * vector. O(n)
     * 
     * @param name The name of the restaurant to be found
     * 
     * @return The index of said restaurant. If not found -1 
     */
        for (int i = 0; i < restaurants.size(); i++){
            if (restaurants[i] == name) { return i; }
        }
        return -1; 
    }

    int getDishIndex(string dish){
    /**
     * @brief Finds the index of a specified dish in the dishes 
     * vector. O(n)
     * 
     * @param name The name of the dish to be found
     * 
     * @return The index of said dish. If not found -1 
     */
        for (int i = 0; i < dishes.size(); i++){
            if (dishes[i] == dish) { return i; }
        }
        return -1; 
    }
    void addDishToRestaurant(int restaurantID, int dishID, float price){
        /**
         * @brief Checks if a value is already a neighbour, if it isn't it adds it. O(n), because search
         * function is used. 
         * 
         * @param restaurantID The index of the restaurant
         * @param dishID The index of the dish 
         * @param price The weight of the edge from dish to restaurant 
         */
             
            if (!restAdj[restaurantID].find(dishID)){
                ListNode* aux = new ListNode(dishID, price);
                restAdj[restaurantID].insertFirst(aux);
            }
            
            if (!dishAdj[dishID].find(restaurantID)) {
                ListNode* aux2 = new ListNode(restaurantID, price);
                dishAdj[dishID].insertFirst(aux2);
            }
    }

    
    void showMenu(string restaurant) {
    /**
     * @brief Display the menu of a specified restaurant. O(n) with n being the number of dishes in the restaurant
     * 
     * @param restaurant The name of the restaurant
     */
        int restIndex = getRestaurantIndex(restaurant); 
        cout << restaurant << "'s Menu" << endl; 
        cout << "---------------------" << endl; 

        ListNode * aux = restAdj[restIndex].getFirst(); 
        while (aux){
            cout << dishes[aux->neighborID] << " " << aux->weight << endl; 
            aux = aux->next; 
        }
    }

    void searchDish(string dish) {
    /** 
     * @brief Search which restaurants serve a specified dish. O(n) with n being the number of restaurants that serve the dish
     * 
     * @param dish The name of the dish to be searched 
     */
        int dishIndex = getDishIndex(dish); 
        cout << "Search results for (with price): " << dish << endl;
        ListNode * aux = dishAdj[dishIndex].getFirst(); 
        while (aux){
            cout << restaurants[aux->neighborID] << " " << aux->weight << endl; 
            aux = aux->next; 
        }
    }
};
struct Coordinate{
            int x; 
            int y; 
            string type; 
            Coordinate(int a = -1, int b = -1)
            : x(a), y(b) {}
        };
class CityGraph {
    public: 
        
        int gridSize;
        vector<Coordinate> coordinates; 
        vector<vector<string>> typeOfEstablishment; 
        vector<LinkedList> Adjlocations; 

        CityGraph(int gs) {
            gridSize = gs * gs; 
            Adjlocations.resize(gridSize); 
            typeOfEstablishment.resize(gridSize);  
         }

        int getLocationID(Coordinate co){
        /**
         * @brief Finds the index of a specified location in the locations 
         * vector. O(n)
         * 
         * @param name The name of the location to be found
         * 
         * @return The index of said location. If not found -1 
         */
            for (int i = 0; i < coordinates.size(); i++){
                if (coordinates[i].x == co.x && coordinates[i].y == co.y) { return i; }
            }
            return -1; 
        }

        int getRestaurantID(string restaurant) {
        /**
         * @brief Finds the index of a specified location in the locations 
         * matrix. O(n^2) (Why do restaurants have to share a building??)
         * 
         * @param name The name of the location to be found
         * 
         * @return The index of said location. If not found -1 
         */
            for (int i = 0; i < gridSize; i++){
                for (int j = 0; j < typeOfEstablishment[i].size(); j++)
                    if (typeOfEstablishment[i][j] == restaurant) { return i; }
            }
            return -1; 
            
        }
        
        void insertLocation(int locationId1, int locationId2, float dist){
        /**
         * @brief Inserts a new node into the Graph. O(n) because of search function
         * 
         * @param locationId The Index of the location
         */
            if (!Adjlocations[locationId1].find(locationId2)){
                ListNode* aux = new ListNode(locationId2, dist); 
                Adjlocations[locationId1].insertFirst(aux); 
            }

             if (!Adjlocations[locationId2].find(locationId1)){
                ListNode* aux = new ListNode(locationId1, dist); 
                Adjlocations[locationId2].insertFirst(aux); 
            }
            
            


        }
    };

//--------------------------------------- Auxiliary functions ----------------------------------------//

MenuGraph CreateMenuGraph(string fileName){

   /**
     * @brief Reads the txt file and initializes a Graph. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * 
     * 
     * @param fileName Name of the file to be opened (Needs to end in .txt)
     * 
     * @return a graph with the data
    */

    string line; 
    bool firstLine = true; 
    MenuGraph g; 

    ifstream inFile(fileName);
    if (inFile.is_open()){ 
        int i = 0;
        while ( getline(inFile, line) ){
            istringstream iss(line);
            if (firstLine) {
                iss >> numRestaurants >> numDishes >> numLines; 
                g.restAdj.resize(numRestaurants);
                g.dishAdj.resize(numDishes);
                firstLine = false; 
                continue; 
            }
            //Temporary variables
            string name, dish, rest; 
            float price;

            //Divide line
            getline(iss, rest);
            rest.erase(0, 2); 
            size_t pos = rest.find("O:");
            size_t pos2 = rest.find("(");
            size_t pos3 = rest.find(")");
            name = rest.substr(0, pos - 1);
            dish = rest.substr(pos + 2, pos2 - (pos + 2));
            price = strtof(rest.substr(pos2 + 1, pos3 - (pos2 + 1)).c_str(), nullptr);
            i++;
           
            //Add both to their respective adjecency lists 
            int posRes = g.getRestaurantIndex(name);
             
            if (posRes == -1) {
                g.restaurants.push_back(name);
                posRes = g.restaurants.size()-1;      
            }
            int posDish = g.getDishIndex(dish); 
            if (posDish == -1) {
                g.dishes.push_back(dish);
                posDish = g.dishes.size() -1; 
                
            }

            //Add to graph
            g.addDishToRestaurant(posRes, posDish, price); 
           
            
        }
    }
    inFile.close();
    return g; 
}
CityGraph CreateCityGraph(string fileName, int gridSize) {
    /**
     * @brief Reads the txt file and initializes a Graph. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * 
     * 
     * @param fileName Name of city file to be opened (Needs to end in .txt)
     * 
     * @return a graph with the data
    */
    string line; 
    CityGraph g(gridSize); 

    ifstream inFile(fileName);
    if (inFile.is_open()){ 
        int i = 0;
        while ( getline(inFile, line) ){
            istringstream iss(line);

            //Temporary variables
            int x, y, x2, y2;   
            char extra; 
            float distance;

            //Divide line
            iss >> extra >> x >> extra >> y >> extra; 
            iss >> extra >> x2 >> extra >> y2 >> extra; 
            iss >> distance; 
            Coordinate co1(x,y), co2(x2,y2); 
            i++;

           
            int posCor1 = g.getLocationID(co1);
            int posCor2 = g.getLocationID(co2);
             
            if (posCor1 == -1) {
                g.coordinates.push_back(co1);
                posCor1 = g.coordinates.size()-1;      
            }
            if (posCor2 == -1) {
                g.coordinates.push_back(co2);
                posCor2 = g.coordinates.size()-1;      
            }


            //Add to graph
            g.insertLocation(posCor1, posCor2, distance); 
           
            
        }
    }
    inFile.close();
    return g; 
}
void addRestaurantsToCity(string fileName, CityGraph &g){
/**
 * @brief Reads a file that specifies a restaurant location, then it 
 * modifies the coordinate to contain the restaurant name. Complexity is O(n)
 * 
 * @param fileName The file with the locations 
 * @param g The Graph with the cityNodes
 */

    string line; 
    ifstream inFile(fileName);
    if (inFile.is_open()){ 
        while (getline(inFile, line) ){
            istringstream iss(line);

            //Temporary variables
            int x, y;   
            string name; 
            string rest; 
            char ch; 

            //Divide line
            getline(iss, rest); 
            size_t pos = rest.find("(");
            name = rest.substr(0, pos - 1);
            rest.erase(0, pos - 1);
            istringstream iss2(rest);
            iss2 >> ch >> x >> ch >> y;            
            Coordinate co(x,y);
            int coId = g.getLocationID(co); 
            g.typeOfEstablishment[coId].push_back(name);
              
        }
    }
    inFile.close();

 }
vector<int> dijkstra(CityGraph &g, Coordinate source, vector<float> &d){
/**
 * @brief Impletenation of Dijkstra's algorithm. O((V + E) log V) with V being the number of vertices and E the number of edges.
 * 
 * @param 
 */
    int n = g.gridSize; 

    vector<float> distance(n, INT_MAX);
    vector<int> previous(n, -1);
    vector<bool> visited(n, false);

    MinHeap h(n); 
    distance[g.getLocationID(source)] = 0; 

    for (int i = 0; i < n; i++) { 
        HeapNode* aux = new HeapNode(i, distance[i]); 
        h.insert(aux);
    }
    while (!h.isEmpty()){
       
        HeapNode* node = h.extractMin();
        int u = node->nodeID;

        if (visited[u] == true) { continue; } 
        visited[u] = true; 

        ListNode* cur = g.Adjlocations[u].getFirst();

        while (cur != nullptr) {
            int v = cur->neighborID; 
            float cost = cur->weight;
            
            if (!visited[v] && distance[u] + cost < distance[v]) {
                distance[v] = distance[u] + cost;
                previous[v] = u; 
                HeapNode* aux = new HeapNode(v, distance[v]); 
                h.insert(aux);
                

            }
            cur = cur->next; 
        }

    }
    d = distance;
    return previous;  
}
vector<int> buildPath(int destination, const vector<int>& previous) {
    vector<int> path;
    for (int v = destination; v != -1; v = previous[v])
        path.insert(path.begin(), v);

    return path;
}
void printResult(const vector<int>& path, const vector<float>& distance, int goal, CityGraph &g) {
    /**
     * @brief Prints the result of the pathfinding algorithm. O(n) with n being the number of nodes in the path
     * 
     * @param path The vector with the path nodes
     * @param distance The vector with the distances to each node
     * @param goal The index of the goal node
     * @param g The graph with the city nodes
     */
    int edges = path.size() - 1;
    float totalDistance = distance[goal];

    cout << "Number of edges: " << edges << endl;
    cout << "Total distance: " << totalDistance << endl;

    cout << "Route: ";
    for (int i = 0; i < path.size(); i++) {
        Coordinate aux = g.coordinates[path[i]];
        cout << "(" << aux.x << ", " << aux.y << ")";
        if (i != path.size() - 1)
            cout << " -> ";
    }
    cout << endl;
}
void deliverOrder(CityGraph& g, string restaurantName, Coordinate destination) {
    /**
     * @brief Finds and prints the optimal path from a restaurant to a destination using Dijkstra's algorithm. O((V + E) log V)
     * 
     * @param g The graph with the city nodes
     * @param restaurantName The name of the restaurant
     * @param destination The coordinates of the destination
     */
    //Look for Restaurant coordinates
    Coordinate source = g.coordinates[g.getRestaurantID(restaurantName)]; 
    int dest = g.getLocationID(destination);

    //Run algorithm 
    vector<float> distance; 
    vector<int> previous = dijkstra(g, source, distance);
    
    //Build path
    vector<int> path = buildPath(dest, previous);
    cout << "Delivering from " << restaurantName << " to " << "( " << destination.x << 
    ", " << destination.y << " )" << endl; 
    printResult(path, distance, dest, g); 
}
void readOrder(string fileName, CityGraph& g) {
/**
 * @brief Takes all relevant information from the orders file. O(n) with n being the number of files. 
 * 
 * @param fileName The name of the file 
 * @param n Refrence to the variable for the restaurant name to be stored
 * @param s Refrence to the variable for the coordinates to be stored 
 */
    string line; 
    int index; 

    ifstream inFile(fileName);

    if (inFile.is_open()) { 
        int i = 0;
        while ( getline(inFile, line) ){
            //Temporary variables
            string month, name, order, rest;
 
            char extra; 
            int day, hour, min, sec; 
            float price; 
            istringstream iss(line);
            //Take order information
            iss >> month >> day >> hour >> extra >> min >> extra >> sec;  
            getline(iss, rest);
            rest.erase(0, 3);
            size_t pos = rest.find("O:");
            size_t pos2 = rest.find("(");
            size_t pos3 = rest.find(")");
            name = rest.substr(0, pos - 1);
            order = rest.substr(pos + 2, pos2 - (pos + 2));
            price = strtof(rest.substr(pos2 + 1, pos3 - (pos2 + 1)).c_str(), nullptr);
             
            //Take coordinates
            
            istringstream iss2(rest.substr(pos3 + 1)); 
            int x,y; 
            char ch; 
            iss2 >> ch >> x >> ch >> y;            
            Coordinate s(x,y);

            //Look for path 
            deliverOrder(g, name, s);
            
        }       
    }
}

int main(){
    cout << "Uploading graphs from files..." << endl;

    MenuGraph menuG = CreateMenuGraph("menus.txt");
    CityGraph cityG = CreateCityGraph("city30x30.txt", 30);
    addRestaurantsToCity("restaPlaces.txt", cityG);

    bool running = true;
    while (running) {
        cout << "\n=== MENU ===\n";
        cout << "1) Show a restaurant's menu\n";
        cout << "2) Search in which restaurants serves an specific dish\n";
        cout << "3) Execute orders from file orders-city30x30.txt\n";
        cout << "Select one option: ";

        int option;
        cin >> option; 

        if (option == 1) {
            cout << "Insert the restaurant's name: ";
            string rest;
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
            getline(cin, rest);
            int idx = menuG.getRestaurantIndex(rest);
            if (idx == -1) {
                cout << "Restaurant not found: " << rest << endl;
            } else {
                menuG.showMenu(rest);
            }
        }
        else if (option == 2) {
            cout << "Insert the dish name: ";
            string dish;
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(cin, dish);
            int idx = menuG.getDishIndex(dish);
            if (idx == -1) {
                cout << "Dish not found: " << dish << endl;
            } else {
                menuG.searchDish(dish);
            }
        }
        else if (option == 3) {
            cout << "Executing all orders from orders-city30x30.txt...\n";
            readOrder("orders-city30x30.txt", cityG);
        }
        else {
            cout << "Option not valid, try again.\n";
        }
    }

    return 0;
}
