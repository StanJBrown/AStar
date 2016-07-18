#include <gtest/gtest.h>
#include <AStar.hpp>


TEST(addNodeAndGetNodeTEST, addAndGetNodes){
    Graph test_graph;
    ASSERT_EQ(0, test_graph.addNode(1, 0, 0));
    ASSERT_EQ(-1, test_graph.addNode(1, 0, 0));
    ASSERT_EQ(1, test_graph.getNode(1)->id);
    ASSERT_EQ(NULL, test_graph.getNode(5));

}

TEST(EdgeTests, EdgeTesting){
    Graph test_graph;
    test_graph.addNode(1, 0, 0);
    test_graph.addNode(2, 1, 1);
    test_graph.addNode(3, 30, 40);

    ASSERT_EQ(0, test_graph.addEdge(1, 2));
    ASSERT_EQ(1, test_graph.getNode(1)->edges.size());

    //test make sure a duplicate edge is not added
    ASSERT_EQ(0, test_graph.addEdge(1, 2));

    ASSERT_EQ(1, test_graph.checkIfEdgeExists(1,2));
    ASSERT_EQ(0, test_graph.checkIfEdgeExists(1,3));

    // check for cases where node is not in the graph at all
    ASSERT_EQ(-1, test_graph.checkIfEdgeExists(1,10));
    ASSERT_EQ(-1, test_graph.removeEdge(2, 10));
    ASSERT_EQ(-1, test_graph.sortEdges(5));
    ASSERT_EQ(-1, test_graph.addEdge(1, 10));

    // check size of edges in node 1
    ASSERT_EQ(1, test_graph.getNode(1)->edges.size());
}




