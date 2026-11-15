#include "KDTree.h"
#include <iostream>
#include <vector>
#include <cstdlib>
#include <chrono>

int main() {
    int N = 100000;
    int K = 3;
    
    std::cout << "Initializing Memory Pool for " << N << " nodes" << std::endl;
    SimpleMemoryPool pool(N);

    std::cout << "Generating " << N << " random 3D points" << std::endl;
    std::vector<Point> points(N);
    for(int i=0; i<N; ++i) {
        points[i] = { {rand()%1000/1.0, rand()%1000/1.0, rand()%1000/1.0}, i };
    }

    std::cout << "Building Tree" << std::endl;
    auto start = std::chrono::high_resolution_clock::now();
    
    Node* root = buildTree(points.begin(), points.end(), 0, K, pool);
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - start;

    std::cout << "Build Complete" << std::endl;
    std::cout << "Time Taken: " << diff.count() << " seconds" << std::endl;
    std::cout << "Memory Pool Usage: " << pool.getUsage() << " / " << pool.getCapacity() << " blocks" << std::endl;

    Point target = { {500.0, 500.0, 500.0}, -1 };
    Node* best = nullptr;
    double bestDist = 1e9;

    nearestNeighbor(root, target, 0, K, best, bestDist);

    std::cout << "Nearest Neighbor to (500, 500, 500): " << std::endl;
    std::cout << "Point ID: " << best->point.id << std::endl;
    std::cout << "Coordinates: (" << best->point.coords[0] << ", " 
              << best->point.coords[1] << ", " << best->point.coords[2] << ")" << std::endl;
    std::cout << "Distance: " << bestDist << std::endl;

    return 0;
}
