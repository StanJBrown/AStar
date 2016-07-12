#include "graph.hpp"


Graph::~Graph(){
    // clean up the allocated nodes
    std::map<int, Node*>::iterator it;
    for(it = this->graph.begin(); it != this->graph.end(); ++it){
        delete it->second;
    }
}

int Graph::addNode(int id, float x, float y, float cost)
{
    // check if a node with id exists
    if (this->graph.count(id) == 0){ //
        Node *new_node = new Node(id, x, y);
        new_node->node_cost = cost;
        this->graph[id] = new_node;
    }else{
        std::cout << "ERROR, node id already exists" << std::endl;
        return -1;
    }
    return 0;
}

int Graph::addNode(int id, float x, float y)
{
    return this->addNode(id, x, y, 0);
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


int Graph::reconstructPath(QueueEntry *end_entry, int start_id, std::vector<int> &path)
{
    QueueEntry *current_entry;
    current_entry = end_entry;
    while(current_entry->node->id != start_id){
        path.push_back(current_entry->node->id);
        current_entry = current_entry->parent;
    }
    path.push_back(start_id);
    return 0;
}


int Graph::cleanupEntries(std::set<QueueEntry*> &entries){
    // delete all of the entries created by new
    std::set<QueueEntry*>::iterator it;
    for(it = entries.begin(); it != entries.end(); ++it){
        delete *it;
    }
}


int Graph::AStarSearch(
    int start_id,
    int end_id,
    float &total_cost,
    std::vector<int> &path
)
{
    std::set<int> visited; // used to track visited nodes
    std::set<QueueEntry*> entries; // keep a list of all new entries created for deletion
    Node *current_node;
    Node *next_node;
    float distance;
    float cost_to_go;
    int id;

    // set up a priority queue to hold the open set sorted on total cost to go
    // A queue Entry object is used in order to create track parents of nodes without changing
    // the underlying graph structure
    std::priority_queue<QueueEntry*, std::vector<QueueEntry*>, compareQueueEntry> queue;

    this->calcDistance(start_id, end_id, distance);
    QueueEntry *starting_entry;
    // intialized and first Queue Entry
    starting_entry = new QueueEntry(this->getNode(start_id), distance);
    entries.insert(starting_entry);
    starting_entry->parent = starting_entry;
    queue.push(starting_entry);

    QueueEntry *next_entry;
    while(!queue.empty()){
        QueueEntry *current_entry = queue.top();
        queue.pop();

        // we are done
        if(current_entry->node->id == end_id){
            if(this->reconstructPath(current_entry, start_id, path) == 0){
                this->cleanupEntries(entries); // free the memory allocated to the queue entries
                return 0;
            }else{
                break;
            }
        }
        current_node = current_entry->node;
        visited.insert(current_entry->node->id);

        // go through each edge in the graph
        for (int i = 0; i < current_node->edges.size(); i++){
            next_node = current_node->edges[i].first;
            id = next_node->id;
            if (visited.find(id) == visited.end()){ // the entry is not yet visited
                this->calcDistance(id, start_id, distance);
                this->calcCostToGo(current_node, next_node, cost_to_go);
                next_entry = new QueueEntry(next_node, cost_to_go + distance) ;
                next_entry->setParent(current_entry);
                entries.insert(next_entry);
                queue.push(next_entry);
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
    test_graph.addNode(2, 0, -10, 20);
    test_graph.addNode(3, 10, 15, 0);
    test_graph.addNode(4, 10, 16, 0);
    test_graph.addNode(1, 29, 29, 20);
    test_graph.addEdge(1, 2);
    test_graph.addEdge(2, 3);
    test_graph.addEdge(2, 4);
    test_graph.addEdge(3, 4);

    Node *node;
    node = test_graph.getNode(2);
    node = test_graph.getNode(2);
    float cost;
    test_graph.calcCostToGo(1,3, cost);
    std::vector<int> path;
    float total_cost;
    test_graph.AStarSearch(1, 4, total_cost, path);
    for (int i = 0; i < path.size(); i++){
        std::cout << path[i] << std::endl;
    }
}



