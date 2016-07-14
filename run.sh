cd build
cmake ..
make
valgrind --leak-check=full ./graph_test


