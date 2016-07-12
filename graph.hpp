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
    bool operator()(const QueueEntry *q1,const QueueEntry *q2){
        return q1->cost > q2->cost;
    }
};

class Graph
{
    public:
        std::map<int, Node*> graph;
        ~Graph();

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

        int reconstructPath(QueueEntry *end_node, int start_id, std::vector<int> &path);
        int cleanupEntries(std::set<QueueEntry*> &entries);
        int AStarSearch(
            int start_id,
            int end_id,
            float &total_cost,
            std::vector<int> &path
        );

};

