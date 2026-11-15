#ifndef KDTREE_H
#define KDTREE_H

#include <vector>
#include <iostream>
#include <cmath>
#include <algorithm>
#include <stdexcept>

struct Point {
    std::vector<double> coords;
    int id;

    double get(int dim) const { return coords[dim]; }
};

struct Node {
    Point point;
    Node* left = nullptr;
    Node* right = nullptr;

    Node(Point p) : point(p) {}
};

class SimpleMemoryPool {
    private:
        size_t blockSize;
        size_t blockCapacity;
        char* memoryBlock;
        size_t currentOffset;

    public:
        SimpleMemoryPool(size_t capacity) : blockCapacity(capacity), currentOffset(0) {
            blockSize = sizeof(Node);
            memoryBlock = new char[blockSize * blockCapacity]; 
        }

        ~SimpleMemoryPool() {
            delete[] memoryBlock;
        }

        void* allocate() {
            if (currentOffset >= blockCapacity) {
                throw std::runtime_error("Memory Pool Exhausted");
            }
            void* ptr = memoryBlock + (currentOffset * blockSize);
            currentOffset++;
            return ptr;
        }

        size_t getUsage() const { return currentOffset; }
        size_t getCapacity() const { return blockCapacity; }
};

using PointIter = std::vector<Point>::iterator;

Node* buildTree(PointIter begin, PointIter end, int depth, int k, SimpleMemoryPool& pool);
void nearestNeighbor(Node* root, Point target, int depth, int k, Node*& best, double& bestDist);

#endif
