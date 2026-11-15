all:
	g++ -std=c++17 -pthread -O3 main.cpp KDTree.cpp -o kdtree_demo

clean:
	rm kdtree_demo