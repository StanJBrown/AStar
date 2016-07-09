#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <set>
#include <queue>

class Node
{
    public:
        int id;
        float x;
        float y;
        float node_cost;
        std::vector<std::pair<Node*, float> > edges;

        Node(int id, float x, float y){
            this->id = id;
            this->x = x;
            this->y = y;
        }
};

class QueueEntry
{
    public:
        Node *node;
        float cost;
        QueueEntry *parent;
        QueueEntry *child;
    public:
        QueueEntry(){};
        QueueEntry(Node *node, float cost){
            this->node = node;
            this->cost = cost;
        }
        int setParent(QueueEntry *entry){
            this->parent = entry;
        }
};

struct compareQueueEntry{
    bool operator()(const QueueEntry &q1,const QueueEntry &q2){
        return q1.cost > q2.cost;
    }
};

class Graph
{
    public:
        std::map<int, Node*> graph;

    public:
        int addNode(int id, float x, float y);
        int addNode(int id, float x, float y, float cost);
        int calcEdgeCost(Node *node_1, Node *node_2, float &cost);
        int addEdge(int id_1, int id_2);
        Node* getNode(int id);
        int calcCostToGo(Node *node_1, Node *node_2, float &cost);
        int calcCostToGo(int id_1, int id_2, float &cost);
        int calcDistance(Node *node_1, Node *node_2, float &cost);
        int calcDistance(int id_1, int id_2, float &cost);
        int reconstructPath(QueueEntry end_node, int start_id, std::vector<int> &path);
        int AStarSearch(
            int start_id,
            int end_id,
            float &total_cost,
            std::vector<int> &path
        );

};

int Graph::addNode(int id, float x, float y)
{
    Node *new_node = new Node(id, x, y);
    new_node->node_cost = 0.0;
    this->graph[id] = new_node;

    return 0;
}

int Graph::addNode(int id, float x, float y, float cost)
{
    Node *new_node = new Node(id, x, y);
    new_node->node_cost = cost;
    this->graph[id] = new_node;

    return 0;
}

int Graph::calcEdgeCost(Node *node_1, Node *node_2, float &cost)
{
    float x_sqr;
    float y_sqr;

    x_sqr = pow(node_1->x - node_2->x, 2);
    y_sqr = pow(node_1->y - node_2->y, 2);
    cost = sqrt(y_sqr + x_sqr);

    return 0;
}

int Graph::addEdge(int id_1, int id_2)
{
    Node *node_1;
    Node *node_2;
    float cost;

    node_1 = this->graph.find(id_1)->second;
    node_2 = this->graph.find(id_2)->second;

    this->calcEdgeCost(node_1, node_2, cost);

    node_1->edges.push_back(std::make_pair(node_2, cost) );
    node_2->edges.push_back(std::make_pair(node_1, cost) );

    return 0;
}

Node* Graph::getNode(int id)
{
    return this->graph.find(id)->second;
}

int Graph::calcCostToGo(Node *node_1, Node *node_2, float &cost)
{
    float total_cost;
    float x_sqr;
    float y_sqr;
    float location_cost;

    x_sqr = pow(node_1->x - node_2->x, 2);
    y_sqr = pow(node_1->y - node_2->y, 2);
    cost = sqrt(y_sqr + x_sqr) + node_2->node_cost;

    return 0;
}

int Graph::calcCostToGo(int id_1, int id_2, float &cost)
{
    Node *node_1;
    Node *node_2;

    node_1 = this->graph.find(id_1)->second;
    node_2 = this->graph.find(id_2)->second;
    this->calcCostToGo(node_1, node_2, cost);

    return 0;
}


int Graph::calcDistance(Node *node_1, Node *node_2, float &cost)
{
    float x_sqr;
    float y_sqr;

    x_sqr = pow(node_1->x - node_2->x, 2);
    y_sqr = pow(node_1->y - node_2->y, 2);

    cost = sqrt(x_sqr + y_sqr);

    return 0;
}


int Graph::calcDistance(int id_1, int id_2, float &cost)
{
    return this->calcCostToGo(
        this->getNode(id_1),
        this->getNode(id_2),
        cost
    );

    return 0;
}

int Graph::reconstructPath(QueueEntry end_entry, int start_id, std::vector<int> &path)
{
    QueueEntry *current_entry;
    current_entry = &end_entry;
    std::cout << "im reconstructing :\t" << std::endl;
    std::cout << "current entry \t" << current_entry->node->id << std::endl;
    std::cout << "parent entry \t" << current_entry->parent->node->id << std::endl;
    while(current_entry->node->id != start_id){
        path.push_back(current_entry->node->id);
        current_entry = current_entry->parent;
    }

    return 0;
}
int Graph::AStarSearch(
    int start_id,
    int end_id,
    float &total_cost,
    std::vector<int> &path
)
{
    std::set<int> visited; // used to track visited nodes
    // QueueEntry current_entry;
    // QueueEntry next_entry;
    Node *current_node;
    Node *next_node;
    float distance;
    float cost_to_go;
    int id;

    //set up a priority queue to hold the open set sorted on total cost to go
    std::priority_queue<QueueEntry, std::vector<QueueEntry>, compareQueueEntry> queue;

    this->calcDistance(start_id, end_id, distance);
    QueueEntry starting_entry(this->getNode(start_id), distance);
    starting_entry.parent = &starting_entry;
    queue.push(starting_entry);

    while(!queue.empty()){
        QueueEntry current_entry = queue.top();
        queue.pop();
        std::cout << "********************************" << std::endl;
        std::cout << "current entry starting \t" << current_entry.node->id << std::endl;
        std::cout << "parent entry \t" << current_entry.parent->node->id << std::endl;
        std::cout << "******************************** \n" << std::endl;
        if(current_entry.node->id == end_id){
            this->reconstructPath(current_entry, start_id, path);
            return 0;
        }
        current_node = current_entry.node;
        visited.insert(current_entry.node->id);
        for (int i = 0; i < current_node->edges.size(); i++){
            // check if neighbor has been visted.
            next_node = current_node->edges[i].first;
            id = next_node->id;
            if (visited.find(id) == visited.end()){ // the entry is not yet visited
                this->calcDistance(id, start_id, distance);
                this->calcCostToGo(current_node, next_node, cost_to_go);
                QueueEntry next_entry(next_node, cost_to_go + distance) ;
                next_entry.setParent(&current_entry);
                queue.push(next_entry);
                std::cout << "next_entry id \t" << next_entry.node->id << std::endl;
                std::cout << "next_entry parent id \t" << next_entry.parent->node->id << std::endl;
            }
        }
    }
    std::cout << "FAILED TO FIND PATH" << std::endl;
    return -1;
}

int main()
{
    Graph test_graph;
    test_graph.addNode(1, 20, 20);
    test_graph.addNode(2, 0, 0, 20);
    test_graph.addNode(3, 10, 15, 50);
    test_graph.addEdge(1, 2);
    test_graph.addEdge(2, 3);

    Node *node;
    node = test_graph.getNode(2);
    node = test_graph.getNode(2);
    float cost;
    test_graph.calcCostToGo(1,3, cost);
    std::vector<int> path;
    float total_cost;
    test_graph.AStarSearch(1, 3, total_cost, path);
    // for (int i = 0; i < path.size(); i++){
    //     std::cout << path[i] << std::endl;
    // }
}



