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
            float price; 
            ListNode* next;
            ListNode(const int& element, float p = -1 , ListNode* n = nullptr)
                : neighborID(element), price(p), next(n) { }
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
            cout << dishes[aux->neighborID] << " " << aux->price << endl; 
            aux = aux->next; 
        }
    }

    void searchDish(string dish) {
        int dishIndex = getDishIndex(dish); 
        cout << "Search results for (with price): " << dish << endl;
        ListNode * aux = dishAdj[dishIndex].getFirst(); 
        while (aux){
            cout << restaurants[aux->neighborID] << " " << aux->price << endl; 
            aux = aux->next; 
        }
    }
};

//--------------------------------------- Auxiliary functions ----------------------------------------//

MenuGraph readFile(string fileName){

   /**
     * @brief Reads the txt file and initializes a Restaurant object for every line. 
     * It iterates once trough every line of the database, so the complexity is O(n)
     * Saves it to a Graph.
     * 
     * @param fileName Name of the file to be opened (Needs to end in .txt)
     * @param graph Will contain the Restaurants that each save their menus
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


int main(){
    MenuGraph g = readFile("menus.txt");
    //g.showMenu("Ana");
    g.searchDish("Sopa de Tomate"); 
    return 0; 
}
