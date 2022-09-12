#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <cmath>
#include <map>

struct Node{
    std::string name;
    float x, y;
    float distance_to_node, distance_to_goal;
    Node *node_via;
    std::vector<Node*> neighbor_nodes;

    Node(std::string name, float x, float y){
        this->name = name;
        this->node_via = this;
        this->x = x;
        this->y = y;
        this->distance_to_node = 0;
        this->distance_to_goal = 0;
    }

    void Connect_2Ways(Node* node){
        std::vector<Node*>::iterator it_found;

        it_found = std::find_if(this->neighbor_nodes.begin(),this->neighbor_nodes.end(),
                                [node](Node *neighbor_node){return node->name == neighbor_node->name;});

        if (it_found == this->neighbor_nodes.end()){
            this->neighbor_nodes.push_back(node);
        }

        it_found = std::find_if(node->neighbor_nodes.begin(),node->neighbor_nodes.end(),
                                [this](Node *neighbor_neighbor_node){return this->name == neighbor_neighbor_node->name;});

        if (it_found == node->neighbor_nodes.end()){
            node->neighbor_nodes.push_back(this);
        }

    }

    void Connect_1Way(Node* node){
        std::vector<Node*>::iterator it_found;

        it_found = std::find_if(this->neighbor_nodes.begin(),this->neighbor_nodes.end(),
                                [node](Node *neighbor_node){return node->name == neighbor_node->name;});

        if (it_found == this->neighbor_nodes.end()){
            this->neighbor_nodes.push_back(node);
        }

    }



    float Distance(Node* node){
        float dis;
        dis = std::sqrt(std::pow(this->x - node->x, 2) + std::pow(this->y - node->y, 2));
        return dis; 
    }
};

std::ostream& operator << (std::ostream& out, const Node& node){
    out << node.name << " -> ";
    std::for_each(node.neighbor_nodes.begin(),node.neighbor_nodes.end(),[&out](Node *neighbor_node){out << neighbor_node->name << " ";});
    return out;
}

void Print(std::vector<Node*> vec){
    std::for_each(vec.begin(),vec.end(),[](Node *node){
            std::cout << node->name << " ";
        });
    std::cout << std::endl;
}


bool A_star(Node *&node_start, Node *&node_goal, std::vector<Node*> &graph, std::vector<Node*> &path){
    std::vector<Node*> Closed_List;
    std::vector<Node*> Open_List;

    Node *node_current;
    std::vector<Node*>::iterator it_found;
    float distance_via_current_node;

    Closed_List.clear();
    Open_List.clear();
    path.clear();

    for (Node *node : graph){
        node->distance_to_goal = node->Distance(node_goal);
    }
    Open_List.push_back(node_start);

    while(1){
        std::sort(Open_List.begin(),Open_List.end(),[](Node *node_a, Node *node_b){
        float cost_a = node_a->distance_to_node + node_a->distance_to_goal;
        float cost_b = node_b->distance_to_node + node_b->distance_to_goal;

        return cost_a <= cost_b;
        });

        node_current = Open_List.front();

        if (node_current->name == node_goal->name){
            do{
            path.insert(path.begin(),node_current);
            node_current = node_current->node_via;
            }while(node_current->name != node_start->name);
            path.insert(path.begin(),node_start);
            
            return true;
        }

        if (node_current->neighbor_nodes.size() == 0){
            return false;
        }
        
        for (Node *neighbor_node : node_current->neighbor_nodes){
            it_found = std::find_if(Open_List.begin(),Open_List.end(),[&neighbor_node](Node *open_list_node){
                return neighbor_node->name == open_list_node->name;
            });

            if (it_found != Open_List.end()){
                // If neighbor_node is already in Open_List
                distance_via_current_node = node_current->distance_to_node + node_current->Distance(neighbor_node);
                if ( distance_via_current_node < neighbor_node->distance_to_node){
                    neighbor_node->node_via = node_current;
                    neighbor_node->distance_to_node = distance_via_current_node;
                }
                
            }
            else{
                it_found = std::find_if(Closed_List.begin(),Closed_List.end(),[&neighbor_node](Node *closed_list_node){
                return neighbor_node->name == closed_list_node->name;
                });

                if (it_found != Closed_List.end()){
                    continue;
                }
                else{
                    neighbor_node->node_via = node_current;
                    neighbor_node->distance_to_node = neighbor_node->node_via->distance_to_node + neighbor_node->Distance(neighbor_node->node_via);
                    Open_List.push_back(neighbor_node);
                }
                
            }

        }
     
    
        Open_List.erase(Open_List.begin());
  
        Closed_List.push_back(node_current);  
        
    }

}

int main(){
    // ---------------- Constructing Graph --------------
    std::vector<Node*> graph;

    Node *A = new Node("A",0,0);
    Node *B = new Node("B",1,0);
    Node *C = new Node("C",0,1);
    Node *D = new Node("D",1,1);
    Node *E = new Node("E",0,2);
    Node *F = new Node("F",1,2);

    A->Connect_2Ways(B);
    B->Connect_1Way(D);
    D->Connect_2Ways(C);
    D->Connect_1Way(F);
    F->Connect_2Ways(E);
    E->Connect_1Way(C);
    C->Connect_1Way(A);

    graph.clear();
    graph = {A,B,C,D,E,F};

    // --------- User Interface -----------
    Node *node_start;
    Node *node_goal;
    std::vector<Node*> path;
    bool success;
    bool terminate = false;
    std::string answer;
    
    std::map<std::string,Node*> node_options;
    for (Node *node : graph){
        node_options.insert({node->name,node});
    }
    std::map<std::string,Node*>::iterator it_found;

    enum class STATE_MACHINE {INIT, NODE_START, NODE_GOAL, A_STAR, PRINT_PATH, PROCEED_MENU, CONTINUE, REACH_GOAL, EXIT};

    STATE_MACHINE state = STATE_MACHINE::INIT;

    while (!terminate){
        switch(state){
            case STATE_MACHINE::INIT:
                state = STATE_MACHINE::NODE_START;
                break;

            case STATE_MACHINE::NODE_START:
                std::cout << "Enter start node: ";
                std::cin >> answer;

                it_found = node_options.find(answer);
                if (it_found == node_options.end()){
                    std::cout << "Invalid start node!!!\n";
                    state = STATE_MACHINE::NODE_START;
                }
                else{
                    node_start = (*it_found).second;
                    state = STATE_MACHINE::NODE_GOAL;
                }

                break;

            case STATE_MACHINE::NODE_GOAL:
                std::cout << "Enter goal node: ";
                std::cin >> answer;

                it_found = node_options.find(answer);
                if (it_found == node_options.end()){
                    std::cout << "Invalid goal node!!!\n";
                    state = STATE_MACHINE::NODE_GOAL;
                }
                else{
                    node_goal = (*it_found).second;
                    state = STATE_MACHINE::A_STAR;
                }

                break;

            case STATE_MACHINE::A_STAR:
                success = A_star(node_start, node_goal, graph, path);

                if (success){
                    std::cout << "Path is found!\n";
                    state = STATE_MACHINE::PRINT_PATH;
                }
                else{
                    std::cout << "Path is NOT found!!! Please enter new goal node.\n";
                    state = STATE_MACHINE::NODE_GOAL;
                }
                break;

            case STATE_MACHINE::PRINT_PATH:
                for (Node *node : path){
                    std::cout << node->name;
                    if (node->name != node_goal->name){
                        std::cout << " -> ";
                    }
                    else{
                        std::cout << std::endl;
                    }
                }
                state = STATE_MACHINE::PROCEED_MENU;
                break;

            case STATE_MACHINE::PROCEED_MENU:
                std::cout << "Continue? [y/n]: ";
                std::cin >> answer;

                if (answer == "y"){
                    state = STATE_MACHINE::CONTINUE;
                }
                else if (answer == "n"){
                    std::cout << "Change goal node? Restart? Exit? [new_goal/restart/exit]: ";
                    std::cin >> answer;
                    
                    if (answer == "new_goal"){
                        node_start = path.front();
                        state = STATE_MACHINE::NODE_GOAL;
                    }
                    else if (answer == "restart"){
                        state = STATE_MACHINE::INIT;
                    }
                    else if (answer == "exit"){
                        state = STATE_MACHINE::EXIT;
                    }
                    else{
                        std::cout << "Invalid answer! Please try again \n";
                        state = STATE_MACHINE::PROCEED_MENU;
                    }
                }
                else{
                    std::cout << "Invalid answer! Please try again \n";
                    state = STATE_MACHINE::PROCEED_MENU;
                }
                break;

            case STATE_MACHINE::CONTINUE:
                path.erase(path.begin());
                if (path.size() == 0){
                    state = STATE_MACHINE::REACH_GOAL;

                }
                else{
                    state = STATE_MACHINE::PRINT_PATH;
                }

                break;


            case STATE_MACHINE::REACH_GOAL:
                std::cout << "GOAL IS REACHED!!! Restart? [y/n]: ";
                std::cin >> answer;

                if (answer == "y"){
                    state = STATE_MACHINE::INIT;
                }
                else{
                    state = STATE_MACHINE::EXIT;
                }
                break;

            case STATE_MACHINE::EXIT:
                terminate = true;
                break;

        }
        
    }

    return 0;
}