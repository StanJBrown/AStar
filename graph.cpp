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


bool Graph::checkIfEdgeExists(Node *node_1, Node *node_2){
    std::list<std::pair<Node*, float> >::iterator it;
    for (it = node_1->edges.begin(); it !=node_1->edges.end(); ++it){
        if (it->first == node_2){
            return true;
        }
    }
    return false;
}


bool Graph::checkIfEdgeExists(int id_1, int id_2){
    return this->checkIfEdgeExists(this->getNode(id_1), this->getNode(id_2));
}


int Graph::sortEdges(Node *node){
    node->edges.sort(sortEdgesByCost);
    return 0;
}


int Graph::sortEdges(int id){
    Node *node;
    node = this->getNode(id);
    node->edges.sort(sortEdgesByCost);
    return 0;
}


int Graph::addEdge(Node *node_1, Node *node_2)
{
    float cost;

    if (this->checkIfEdgeExists(node_1, node_2) == false){
        this->calcEdgeCost(node_1, node_2, cost);
        node_1->edges.push_back(std::make_pair(node_2, cost));
    }
    if (this->checkIfEdgeExists(node_2, node_1) == false){
        this->calcEdgeCost(node_2, node_1, cost);
        node_2->edges.push_back(std::make_pair(node_1, cost));
    }
    return 0;
}


int Graph::addEdge(int id_1, int id_2)
{
    Node *node_1;
    Node *node_2;
    float cost;

    node_1 = this->graph.find(id_1)->second;
    node_2 = this->graph.find(id_2)->second;

    return this->addEdge(node_1, node_2);
}

int Graph::removeEdge(Node *node_1, Node *node_2)
{
    std::list<std::pair<Node*, float> >::iterator it;
    // search for an edge connecting 1 to 2
    if (this->checkIfEdgeExists(node_1, node_2)){
        for (it = node_1->edges.begin(); it !=node_1->edges.end(); ++it){
            if (it->first == node_2){
                node_1->edges.erase(it);
                break;
            }
        }

        for (it = node_2->edges.begin(); it !=node_2->edges.end(); ++it){
            if (it->first == node_1){
                node_2->edges.erase(it);
                break;
            }
        }
        return 0;
    } else {
        std::cout << "Edge does not exist" << std::endl;
        return -1;
    }
}


int Graph::removeEdge(int id_1, int id_2){
    return this->removeEdge(this->getNode(id_1), this->getNode(id_2));
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


int Graph::calcDistance(int id_1, int id_2, float &cost)
{
    return this->calcCostToGo(
        this->getNode(id_1),
        this->getNode(id_2),
        cost
    );
}


Node* Graph::getNode(int id)
{
    return this->graph.find(id)->second;
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


int Graph::calcEdgeCost(Node *node_1, Node *node_2, float &cost)
{
    float x_sqr;
    float y_sqr;

    x_sqr = pow(node_1->x - node_2->x, 2);
    y_sqr = pow(node_1->y - node_2->y, 2);
    cost = sqrt(y_sqr + x_sqr);

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
    std::set<QueueEntry*> entries; // keep a list of all new entries created for deletion
    Node *current_node;
    Node *next_node;
    float distance;
    float cost_to_go;
    int id;

    std::list<std::pair<Node*, float> >::iterator edge_iterator;

    // set up a priority queue to hold the open set sorted on total cost to go
    // A queue Entry object is used in order to create track parents of nodes
    // without changing the underlying graph structure
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
        for (edge_iterator = current_node->edges.begin();
             edge_iterator != current_node->edges.end();
             ++edge_iterator){
            next_node = edge_iterator->first;
            id = next_node->id;
            if (visited.find(id) == visited.end()){ // the entry is not yet visited
                cost_to_go = edge_iterator->second + edge_iterator->first->node_cost;
                next_entry = new QueueEntry(next_node, cost_to_go) ;
                next_entry->setParent(current_entry);
                entries.insert(next_entry);
                queue.push(next_entry);
            }
        }
    }
    std::cout << "FAILED TO FIND PATH" << std::endl;
    return -1;
}

int Graph::replaceEdge(Node *node_1, Node *node_2, float &cost){
    // find an edge with a higher cost then node_1 and node_2 and remove it
    std::list<std::pair<Node*, float> >::iterator it;
    int replaced_node_id;
    Node *replaced_node;

    for(it = node_1->edges.begin(); it != node_1->edges.end(); it++){
        if (it->second > cost){
            replaced_node_id = it->first->id;
            it = node_1->edges.erase(it);
            node_1->edges.insert(it, std::make_pair(node_2, cost));

            this->removeEdgeSingular(this->getNode(replaced_node_id), node_1);
            break;
        }
    }
    // the replaced node needs to remove the edge corrisponding to node_1
    // for(it = replaced_node->edges.begin(); it != replaced_node->edges.end(); ++it){
    //     if (it->first->id == node_1->id){
    //         // this edge has been replaced in the node 1 and must be removed from this edge
    //         replaced_node->edges.erase(it);
    //         break;
    //     }
    // }
    return 0;
}

int Graph::removeEdgeSingular(Node *node_1, Node *node_2)
{
    std::list<std::pair<Node*, float> >::iterator it;
    for(it = node_1->edges.begin(); it != node_1->edges.end(); ++it){
        if (it->first->id == node_2->id){
            // this edge has been replaced in the node 1 and must be removed from this edge
            node_1->edges.erase(it);
            break;
        }
    }
    return 0;
}

int Graph::findNearestNodes(int number){
    std::map<int, Node*>::iterator it_1;
    std::map<int, Node*>::iterator it_2;
    float cost;
    Node *node_1;
    Node *node_2;

    for (it_1 = this->graph.begin(); it_1 != this->graph.end(); ++it_1){
        for (it_2 = this->graph.begin(); it_2 != this->graph.end(); ++it_2){
            node_1 = it_1->second;
            node_2 = it_2->second;
            if (node_1 == node_2){ // avoid having a nodes edge point to itself
                continue;
            }
            if (this->checkIfEdgeExists(node_1, node_2)){
                continue; // there is already an edge between these nodes
            }
            if (node_1->edges.size() < number){
                this->addEdge(node_1, node_2);
                this->sortEdges(node_1);
                this->sortEdges(node_2); // resort the edge lists based on cost
                continue;
            }else{
                this->calcCostToGo(node_1, node_2, cost);
                if (cost <= node_1->edges.back().second){
                    // the cost to go to this node is larger then the end of
                    // the sorted list of edges, therefore its longer then
                    // what we already have
                    this->replaceEdge(node_1, node_2, cost);
                    // find the location in the list replace a node
                    continue;
                }
            }
        }
    }
    return 0;
}

int Graph::printAllEdges(){
    std::map<int, Node*>::iterator it;
    std::list<std::pair<Node*, float> >::iterator edge_it;
    Node *current_node;

    int id_1;
    int id_2;
    float cost;
    int edge_number;
    int number_of_edges;

    edge_number = 1;
    for(it = this->graph.begin(); it != this->graph.end(); ++it){
        current_node = it->second;
        edge_it = current_node->edges.begin();
        number_of_edges = current_node->edges.size();
        printf("Edge: %i has %i edges \n", current_node->id, number_of_edges );
        for(edge_it; edge_it != current_node->edges.end(); ++edge_it){
            id_1 = current_node->id;
            id_2 = edge_it->first->id;
            cost = edge_it->second;
            printf("Edge: %i between Nodes %i and %i with a cost of %2.3f \n",
                edge_number, id_1, id_2, cost);
            ++edge_number;
        }
    }
}


int main()
{
    Graph test_graph;
    test_graph.addNode(1, 2, 20);
    test_graph.addNode(2, 10, 15, 0);
    test_graph.addNode(3, 10, 10, 20);
    test_graph.addNode(4, 10, 16, 0);
    test_graph.addNode(5, 11, 52, 0);
    test_graph.addNode(6, -20, 22, 0);
    test_graph.addNode(7, 12, -52, 0);
    test_graph.addNode(8, 0, 2, 0);
    // test_graph.addEdge(1, 2);
    // test_graph.addEdge(3, 1);
    // test_graph.addEdge(2, 3);
    // test_graph.addEdge(2, 4);
    // test_graph.addEdge(3, 4);
    // test_graph.addEdge(6, 4);
    // test_graph.removeEdge(3, 4);
    // test_graph.removeEdge(1, 2);

    Node *node;
    float cost;
    // test_graph.calcCostToGo(1,3, cost);
    std::vector<int> path;
    float total_cost;
    // test_graph.AStarSearch(1, 4, total_cost, path);
    // for (int i = 0; i < path.size(); i++){
    //     std::cout << path[i] << std::endl;
    // }
    std::cout << "link nearest Nodes" << std::endl;
    node = test_graph.getNode(1);
    test_graph.findNearestNodes(4);
    test_graph.printAllEdges();

    // std::vector<int> path2;
    // test_graph.AStarSearch(1, 4, total_cost, path2);
    // for (int i = 0; i < path2.size(); i++){
    //     std::cout << path2[i] << std::endl;
    // }

}



