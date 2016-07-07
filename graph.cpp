#include <iostream>
#include <vector>
#include <map>
#include <math.h>


class Node
{
    public:
        int id;
        std::vector<std::pair<Node*, float> > edges;
        float x;
        float y;
        Node(int id, float x, float y){
            this->id = id;
            this->x = x;
            this->y = y;
        }
};

class Graph
{
    public:
        std::map<int, Node*> graph;

    public:
        int addNode(Node &Node);
        int addNode(int id, float x, float y);
        int calcEdgeCost(Node *node_1, Node *node_2, float &cost);
        int addEdge(int id_1, int id_2);
        Node* getNode(int id);
};

int Graph::addNode(int id, float x, float y)
{
    Node *new_node = new Node(id, x, y);
    this->graph[id] = new_node;
}

int Graph::calcEdgeCost(Node *node_1, Node *node_2, float &cost)
{
    float x_sqr;
    float y_sqr;

    x_sqr = pow(node_1->x - node_2->x, 2);
    y_sqr = pow(node_1->y - node_2->y, 2);
    cost = y_sqr + x_sqr;
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
}

Node* Graph::getNode(int id)
{
    return this->graph.find(id)->second;
}

int main()
{
    Graph test_graph;
    test_graph.addNode(1, 20, 20);
    test_graph.addNode(2, 0, 0);
    test_graph.addEdge(1, 2);

    Node *node;
    node = test_graph.getNode(2);
    std::cout << node->edges[0].second << std::endl;
}



