#ifndef ASTAR_GRAPH_BASE_H_
#define ASTAR_GRAPH_BASE_H_

#include <iostream>
#include <vector>
#include <map>
#include <math.h>
#include <set>
#include <queue>
#include <algorithm>
#include <list>
#include <stdio.h>
class Node
{
    public:
        int id;
        float x;
        float y;
        float node_cost;
        std::list<std::pair<Node*, float> > edges; //list is used as erase is a much faster operation

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

struct compareQueueEntry
{
    bool operator()(const QueueEntry *q1,const QueueEntry *q2)
    {
        return q1->cost > q2->cost;
    }
};

static bool sortEdgesByCost (
    const std::pair<Node*, float> &edge_1,
    const std::pair<Node*, float> &edge_2)
{
    return edge_1.second < edge_2.second; // desencing order
}

class Graph
{
    public:
        std::map<int, Node*> graph;
        ~Graph();

    public:
        int addNode(int id, float x, float y);
        int addNode(int id, float x, float y, float cost);

        int checkIfEdgeExists(Node *node_1, Node *node_2);
        int checkIfEdgeExists(int id_1, int id_2);

        int sortEdges(Node *node);
        int sortEdges(int it);

        int addEdge(Node *node_1, Node *node_2);
        int addEdge(int id_1, int id_2);

        int removeEdge(Node *node_1, Node *node_2);
        int removeEdge(int id_1, int id_2);

        int calcCostToGo(Node *node_1, Node *node_2, float &cost);
        int calcCostToGo(int id_1, int id_2, float &cost);

        int calcDistance(Node *node_1, Node *node_2, float &cost);
        int calcDistance(int id_1, int id_2, float &cost);

        Node* getNode(int id);
        int calcEdgeCost(Node *node_1, Node *node_2, float &cost);

        int replaceEdge(Node *node_1, Node *node_2, float &cost);
        int removeEdgeSingular(Node *node_1, Node *node_2);

        int reconstructPath(QueueEntry *end_node, int start_id, std::vector<int> &path);
        int cleanupEntries(std::set<QueueEntry*> &entries);
        int AStarSearch(
            int start_id,
            int end_id,
            float &total_cost,
            std::vector<int> &path
        );
        int findNearestNodes(int number);
        int printAllEdges();


};

#endif
