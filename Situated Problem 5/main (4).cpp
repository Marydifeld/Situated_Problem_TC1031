#include<string> 
#include <vector>
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>
#include <limits>
#include <list>
#include <type_traits>
#include <functional>

using namespace std;
int numRestaurants, numDishes, numLines;
void insertionSort(vector<string> & vec);
//Classes
struct Coordinate{
            int x; 
            int y; 
            bool operator==(const Coordinate& other) const {
                return x == other.x && y == other.y;
            }
            vector<int> restaurantsInHere; 
            Coordinate(int a = -1, int b = -1)
            : x(a), y(b) {}
        };
struct HeapNode {
        int nodeID; 
        float distance; 
        HeapNode(const int& index, float d)
        : nodeID(index), distance(d) { }
};
class LinkedList {
    private: 
        struct ListNode {
            int neighborID; 
            float weight; 
            ListNode* next;
            ListNode(const int& element, float p = -1 , ListNode* n = nullptr)
                : neighborID(element), weight(p), next(n) { }
        };
        ListNode* first = nullptr;
        int size; 
                                            
    public: 
        LinkedList(){
                this->first = nullptr; 
                this->size = 0;
        }
        ListNode* createNode(int id, float weight) {
        /**
         * @brief Creates a new list node with the default pointer.
         * This function helps maintian encapsulation, only the Linked list class
         * can create and delete nodes. 
         * 
         * @param id The index of the element in teh Graph to be stored in the node
         * @param weight The weight of the edge to the next Graph Node
         * 
         * @return The new List node 
         */

         return new ListNode(id, weight); 

        }
        ListNode* getFirst(){return first;}
        int getSize(){return size;}
		
		void insertFirst(int id, float weight) {
            ListNode* newNode = createNode(id, weight);
            if (!first){
                first = newNode;
                size++; 
                return; 
            }

            newNode->next = first; 
            first = newNode; 
            size++; 
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

template <class T> 
class Graph {
    //A parent class to simplify the implementation of future classes for weighted unirected Graphs
    protected: 
        int size; 
        vector<T> values;
        vector<LinkedList> AdjList;
    public: 
        Graph( ) { }
        Graph(int s) {
            size = s; 
            values.resize(s); 
            AdjList.resize(s); 
        } 

        vector<T>& getValues() {return values;}
        vector<LinkedList>& getAdjList() {return AdjList;}
        void setValue(T value, int index) {
            values[index] = value; 
        }
        int getSize(){return size;}
        int getId(T value) {
         /**
         * @brief Finds the index of a specified value in the values
         * vector. O(n)
         * 
         * @param value The name of the value to be found
         * 
         * @return The index of said value. If not found -1 
         */  
            for (int i = 0; i < values.size(); i++){
                if (values[i] == value) { return i; }
            }
            return -1; 
        }
        virtual void insertNode(int id1, int id2, float weight) {
        /**
         * @brief Inserts a new node into the Graph. O(1) because
         * insertion is at head. 
         * 
         * @param id1 The Index of the first value
         * @param id12 The Index of the second value
         */  
            if (!AdjList[id1].find(id2)){ 
                AdjList[id1].insertFirst(id2, weight); 
            }

             if (!AdjList[id2].find(id1)){
                AdjList[id2].insertFirst(id1, weight); 
            }

        }

}; 
class CityGraph: public Graph<Coordinate> {
    public: 
        CityGraph(int s): Graph<Coordinate>(s) { }
        
        bool hasRestaurants(Coordinate cor) {
            int corId = getId(cor); 
            return !values[corId].restaurantsInHere.empty(); 
        }
        void addRestaurantToCoordinate(int coordID, int restaurantID) {
            values[coordID].restaurantsInHere.push_back(restaurantID); 
        }

};
template <typename AnyType>
class SeparateChainingHashTable {
public:

    // Construct the hash table.
    SeparateChainingHashTable()
        : SeparateChainingHashTable(DEFAULT_TABLE_SIZE) {}

    // Construct the hash table with approximate size.
    explicit SeparateChainingHashTable(int size)
        : theLists(nextPrime(size)), currentSize(0) {}

    // Insert (key, value). If key exists, update its value.
    void insert(const AnyType& key, int value) {
    /**
     * @brief Inserts a value into the hashtable by hashing the key
     * O(1), it gets inserted at the end of the linked list of the bucket
     * 
     * @param key The value that gets hashed 
     * @param value The value to be stored in the bucket that the key indicates
     */
        auto& whichList = theLists[myhash(key)];

        // Check if key already exists → update
        for (auto& kv : whichList) {
            if (kv.first == key) {
                kv.second = value; 
                return;
            }
        }

        // Otherwise insert new key–value pair
        whichList.push_back({key, value});

        if (++currentSize > static_cast<int>(theLists.size()))
            rehash();
    }

    // Remove a key if present.
    void remove(const AnyType& key) {
        auto& whichList = theLists[myhash(key)];

        for (auto it = whichList.begin(); it != whichList.end(); ++it) {
            if (it->first == key) {
                whichList.erase(it);
                --currentSize;
                return; 
            }
        }
    }

    // Return true if key is present.
    bool contains(const AnyType& key) const {
        const auto& whichList = theLists[myhash(key)];
        for (const auto& kv : whichList)
            if (kv.first == key)
                return true;
        return false;
    }

    // Get the value associated with a key.
    int get(const AnyType& key) const {
        const auto& whichList = theLists[myhash(key)];
        for (const auto& kv : whichList)
            if (kv.first == key)
                return kv.second;
        cout << key << endl; 
        throw runtime_error("Key not found");
    }

    // Make the hash table logically empty.
    void makeEmpty() {
        for (auto& lst : theLists) lst.clear();
        currentSize = 0;
    }

    // Size (number of stored elements).
    int size() const {
        return currentSize;
    }

    // Expose hash index for an item.
    int getHash(const AnyType& key) const {
        return myhash(key);
    }

    // Expose the bucket array (read-only).
    const vector<list<pair<AnyType, int>>>& getLists() const {
        return theLists;
    }

    // A hash routine for String objects (matches Java static hash).
    static int hash(const std::string& key, int tableSize) {
        int hashVal = 0;
        for (char c : key)
            hashVal = 37 * hashVal + static_cast<unsigned char>(c);

        hashVal %= tableSize;
        if (hashVal < 0)
            hashVal += tableSize;

        return hashVal;
    }

private:
    static constexpr int DEFAULT_TABLE_SIZE = 101;

    // Stores Pairs: key = AnyType, value = int
    vector<list<pair<AnyType, int>>> theLists;
    int currentSize;

    // Rehash into a new table of ~double size (next prime).
    void rehash() {
        auto oldLists = theLists;

        theLists.clear();
        theLists.resize(nextPrime(2 * static_cast<int>(oldLists.size())));
        currentSize = 0;

        for (const auto& lst : oldLists)
            for (const auto& kv : lst)
                insert(kv.first, kv.second);
    }

    // myhash:
    // - uses std::hash for general types
    // - if AnyType is std::string, uses hash(key, DEFAULT_TABLE_SIZE)
    int myhash(const AnyType& key) const {
        int hashVal;

        if constexpr (std::is_same_v<AnyType, std::string>) {
            hashVal = hash(key, DEFAULT_TABLE_SIZE);
        } else {
            std::size_t hv = std::hash<AnyType>{}(key);
            hashVal = static_cast<int>(hv);
        }

        hashVal %= static_cast<int>(theLists.size());
        if (hashVal < 0)
            hashVal += static_cast<int>(theLists.size());

        return hashVal;
    }

    // Prime utilities (same logic as Java).
    static int nextPrime(int n) {
        if (n % 2 == 0) n++;
        while (!isPrime(n)) n += 2;
        return n;
    }

    static bool isPrime(int n) {
        if (n == 2 || n == 3) return true;
        if (n == 1 || n % 2 == 0) return false;

        for (int i = 3; i * i <= n; i += 2)
            if (n % i == 0) return false;

        return true;
    }
};
class MenuGraph: public Graph<string> {
    private: 
        vector<string> dishes;
        vector<LinkedList> AdjDish;
        //To make restaurant look up faster 
        SeparateChainingHashTable<string> restaurantNameToId;
    public: 
        MenuGraph (int numRestaurants, int numDishes) 
        : Graph<string> (numRestaurants), restaurantNameToId(numRestaurants / 2){
            dishes.resize(numDishes); 
            AdjDish.resize(numDishes);
        }
        vector<string>& getRestaurants() {return values;}
        vector<LinkedList>& getAdjRes() {return AdjList;}
        vector<string>& getDishes() {return dishes;}
        vector<LinkedList>& getAdjDish() {return AdjDish;}
        void setDish(string dish, int i) {
            dishes[i] = dish; 
        }
        int getId2(string dish) {
            for (int i = 0; i < dishes.size(); i++){
                if (dishes[i] == dish) { return i; }
            }
            return -1; 
        }
        int getRestaurantId(string rest){
        /**
         * @brief Hides the getId() method. Intended to be used as a lookup for the 
         * restaurant id, because it's faster. O(1) while getId is O(n). It was implemented
         * because of how often CityGraph has to use de restaurant id
         * 
         * @param rest The key for the value that is being looked for
         * @return The id of the restaurant
         */
            return restaurantNameToId.get(rest);
        }
        bool cointainsRestaurant(string rest) {
            return restaurantNameToId.contains(rest);
        }
        void insertNode(int restaurantId, int dishId, float price) override{
            /**
         * @brief Checks if a value is already a neighbour, if it isn't it adds it. O(n), because search
         * function is used. 
         * 
         * @param restaurantId The index of the restaurant
         * @param dishId The index of the dish 
         * @param price The weight of the edge from dish to restaurant 
         */
             
            if (!AdjList[restaurantId].find(dishId)){
                AdjList[restaurantId].insertFirst(dishId, price);
            }
            
            if (!AdjDish[dishId].find(restaurantId)) {
                AdjDish[dishId].insertFirst(restaurantId, price);
            } 
        }
        void initRestaurantHash() {
            for (int i = 0; i < numRestaurants; i++) {
                restaurantNameToId.insert(values[i], i); 
            }
        }

        void showMenu(string restaurant) {
            int restIndex = getRestaurantId(restaurant); 
            cout << restaurant << "'s Menu" << endl; 
            cout << "---------------------" << endl; 
            auto aux = AdjList[restIndex].getFirst(); 
            while (aux){
                cout << dishes[aux->neighborID] << " " << aux->weight << endl; 
                aux = aux->next; 
        }    
    }

    vector<string> createDishVector(int dishIndex) {
        auto aux = AdjDish[dishIndex].getFirst();
        vector<string> dishvec;  
        while (aux){
            string parsed; 
            parsed =  values[aux->neighborID] + " "  + to_string(static_cast<int>(aux->weight)); 
            dishvec.push_back(parsed); 
            aux = aux->next; 
        }
        return dishvec; 
    }
    void searchDish(string dish) {
        int dishIndex = getId2(dish);
        vector<string> dishvec = createDishVector(dishIndex);
        insertionSort(dishvec);
        cout << "Search results for: " << dish << endl;
       for (int i = 0; i < dishvec.size(); i++ ) {
            cout << dishvec[i] << endl; 
       }
        
    }
 
};

//Auxiliary functions
MenuGraph createMenuGraph(string fileName){
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
    int restIndex = 0;
    int dishIndex = 0; 
   
    MenuGraph g(0,0); 
    ifstream inFile(fileName);
    if (inFile.is_open()){ 
        while ( getline(inFile, line) ){
            istringstream iss(line);
            if (firstLine) {  
                iss >> numRestaurants >> numDishes >> numLines;      
                firstLine = false; 
                g = MenuGraph(numRestaurants, numDishes);
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
           
           
            //Add both to their respective adjecency lists 
            int posRes = g.getId(name);
             
            if (posRes == -1) {
                g.setValue(name, restIndex);
                posRes = restIndex;
                restIndex++;      
            }
            int posDish = g.getId2(dish); 
            if (posDish == -1) {
                g.setDish(dish, dishIndex);
                posDish = dishIndex;
                dishIndex++; 
            }

            //Add to graph
            g.insertNode(posRes, posDish, price); 
            
        }
    }
    inFile.close();
    g.initRestaurantHash();
    return g; 
}
//Add integration to hashtable
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
        while (getline(inFile, line) ){
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

           
            int posCor1 = g.getId(co1);
            int posCor2 = g.getId(co2);
             
            if (posCor1 == -1) {
                g.setValue(co1, i);
                posCor1 = i;
                i++;    
            }
            if (posCor2 == -1) {
                g.setValue(co2, i);
                posCor2 = i;  
                i++;
                    
            }


            //Add to graph
            g.insertNode(posCor1, posCor2, distance); 
           
            
        }
    }
    inFile.close();
    return g; 
}

void generateMenuHashtables(MenuGraph& g, SeparateChainingHashTable<string>& restaurants,
                            SeparateChainingHashTable<string>& dishes){

    SeparateChainingHashTable<string>  restaurantTable(numRestaurants/2); 
    SeparateChainingHashTable<string>  dishTable(numDishes/2); 

    for (int i = 0; i < g.getRestaurants().size(); i++) {
        string current = g.getRestaurants()[i];
        restaurantTable.insert(current, g.getId(current));
    }

    for (int i = 0; i < g.getDishes().size(); i++) {
        string current = g.getDishes()[i];
        dishTable.insert(current, g.getId2(current));
    }
    restaurants = restaurantTable;
    dishes = dishTable; 
}
 
void insertionSort(vector<string>& vec) {
/**
 * @brief Sorts a vector in ascending order, O(n^2), but works great for small vectors.
 * 
 * @param vec The vector to be sorted 
 */
    string temp; 

    for (int i = 1; i < vec.size(); i++) {
        temp = vec[i]; 
        int j = i - 1; 
        for (j; j >= 0  && vec[j] > temp; j--) {
            vec[j + 1] = vec[j];
        }
        vec[j + 1] = temp; 
        
    }
}

void addRestaurantsToCity(string fileName, CityGraph &g, MenuGraph &m){
/**
 * @brief Reads a file that specifies a restaurant location, then it 
 * adds the corresponding restaurant id to the Coordinates node 
 * 
 * @param fileName The file with the locations 
 * @param g The Graph with the cityNodes
 * @param m The MenuGraph that contains the restaurant ids
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
        
            int coId = g.getId(co); 
            int restId = m.getRestaurantId(name);
            g.addRestaurantToCoordinate(coId, restId); 
              
        }
    }
    inFile.close();

 }

vector<int> dijkstra(CityGraph &g, Coordinate source, vector<float> &d, int k){
    /**
     * @brief Impletenation of a modified Dijkstra's algorithm. It runs normally until it
     * finds the k closest restaurants, so complexity is reduced to O(k log n) 
     * 
     * @param g The graph that is being traversed
     * @param source The starting node
     * @param d a vector that will contain all distances right until the k locations are found
     * @param k The amount of locations that are being searched
     * 
     * @return The vector containing name of k locations
     */

    int n = g.getSize(); 

    vector<float> distance(n, INT_MAX);
    vector<int> previous(n, -1);
    vector<bool> visited(n, false);
    vector<int> nearestRestaurants; 
    int counter = 0; 

    MinHeap h(n); 
    int sourceID = g.getId(source);
    distance[sourceID] = 0; 

    for (int i = 0; i < n; i++) { 
        HeapNode* aux = new HeapNode(i, distance[i]); 
        h.insert(aux);
    }

    while (!h.isEmpty()){
       
        HeapNode* node = h.extractMin();
        int u = node->nodeID;

        if (visited[u] == true) { continue; } 
        visited[u] = true; 

        vector<int> &restHere = g.getValues()[u].restaurantsInHere;  
        if (!restHere.empty()) {
            nearestRestaurants.push_back(u);
            counter++; 
            if (counter == k) {
                d = distance;
                return nearestRestaurants; 
            }
        }

        auto cur = g.getAdjList()[u].getFirst();

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
    return nearestRestaurants;  
}



vector<int> buildPath(int destination, const vector<int>& previous) {
    vector<int> path;
    for (int v = destination; v != -1; v = previous[v])
        path.insert(path.begin(), v);

    return path;
}

// Main 
int main(){
    //Create Menu & City graphs with restaurant
    MenuGraph g = createMenuGraph("menus.txt");
    CityGraph g2  = CreateCityGraph("city30x30.txt", 30 * 30);
    addRestaurantsToCity("restaPlaces.txt", g2, g); 

    //Create Hashtable to query by restaurant or dish
    SeparateChainingHashTable<string> rests;
    SeparateChainingHashTable<string> dishs;
    generateMenuHashtables(g, rests, dishs);

    int option;
    do{
        cout << "\n===== Menu =====" << endl;
        cout << "1. See restaurant menu" << endl;
        cout << "2. Search restaurants by dish" << endl;
        cout << "3. Find the 3 nearest restaurants" << endl;
        cout << "4. Exit" << endl;
        cout << "\nSelect an option: ";
        cin >> option;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');

        if(option == 1){
            string restaurant;
            cout << "Enter restaurant name: ";
            getline(cin, restaurant);
            if(!g.cointainsRestaurant(restaurant)){
                cout << "Restaurant not found!" << endl;
            }
            else{
                g.showMenu(restaurant);
            }
            }

        else if(option == 2){
            string dish;
            cout << "Enter dish name: ";
            getline(cin, dish);
            int id = g.getId2(dish);
            if(id == -1){
                cout << "Dish not found!" << endl;
            }
            else{
                g.searchDish(dish);
            }
        }

        else if(option == 3){
            int x, y;
            cout << "Enter your (x, y) coordinates: ";
            cin >> x >> y;
            Coordinate user(x, y); //Location of user (need to turn into input)
            vector<float> distance; // Distance vector to the restaurant coordinates
            vector<int> coordsWRest = dijkstra(g2, user, distance, 3); 

            if(coordsWRest.empty()){
                cout << "No restaurant found nearby!" << endl;
            }
            else{
                cout << "\nThe 3 nearest restaurants are: " << endl;
                for (int i = 0; i < coordsWRest.size(); i++){
                vector<int> restsInCoor = g2.getValues()[coordsWRest[i]].restaurantsInHere;
                    for (int j = 0; j < restsInCoor.size(); j++) {
                        int index = restsInCoor[j];
                        string restaurant = g.getValues()[index];
                        cout << "Close restaurant: " << restaurant << endl;
                        cout << "At distance: " << distance[coordsWRest[i]]; 
                        cout << endl; 

                        //Maybe make displaying the menu optional? 
                        char showMenuOption;
                        cout << "Do you want to see the menu? (y/n): ";
                        cin >> showMenuOption;
                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                        if (showMenuOption == 'y' || showMenuOption == 'Y'){
                        g.showMenu(restaurant);
                        }
                        cout << endl; 
                    }
                }     
            }
        }
    }while(option != 4);
return 0;
}    