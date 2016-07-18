cd build
cmake ..
make
valgrind --leak-check=full ./AStar_test


