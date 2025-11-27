#include<string> 
#include<vector>
#include<ctime> 
#include <iomanip>
#include<iostream>
#include<fstream>
#include<sstream>

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
            Coordinate(int a, int b, string t = "House")
            : x(a), y(b), type(t) {}
        };
class CityGraph {
    private: 
        int gridSize; 
    public: 
        

        vector<Coordinate> coordinates; 
        vector<LinkedList> Adjlocations; 

        CityGraph(int gs) {
            gridSize = gs * gs; 
            Adjlocations.resize(gridSize); 

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
        
        void insertLocation(int locationId1, int locationId2, float dist){
        /**
         * @brief Inserts a new node into the Graph 
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
 * modifies the coordinate to contain the restaurant name. 
 * 
 * @param fileName The file with the locations 
 * @param g The Graph with the cityNodes
 */

    string line; 
    ifstream inFile(fileName);
    if (inFile.is_open()){ 
        while ( getline(inFile, line) ){
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
            g.coordinates[coId].type = name;  
             
           
            
        }
    }
    inFile.close();

 }

int main(){
    //MenuGraph g = CreateMenuGraph("menus.txt");
    //g.showMenu("Ana");
    //g.searchDish("Sopa de Tomate"); 
    CityGraph g2 = CreateCityGraph("city30x30.txt", 30);
    addRestaurantsToCity("restaPlaces.txt", g2); 
    for (int i = 0; i < 100; i++){
        cout << g2.coordinates[i].type << endl;
    }

    return 0; 
}
