#include "KDTree.h"
#include <future>

Node* newNode(Point p, SimpleMemoryPool& pool) {
    void* mem = pool.allocate();
    return new (mem) Node(p);
}

Node* buildTree(PointIter begin, PointIter end, int depth, int k, SimpleMemoryPool& pool) {
    if (std::distance(begin, end) <= 0) return nullptr;

    int axis = depth % k;
    size_t len = std::distance(begin, end);
    PointIter medianIter = begin + len / 2;

    std::nth_element(begin, medianIter, end,
        [axis](const Point& a, const Point& b) {
            return a.get(axis) < b.get(axis);
        });

    Node* node = newNode(*medianIter, pool);

    if (len > 1000) { 
        auto leftFuture = std::async(std::launch::async, 
            [&]() { return buildTree(begin, medianIter, depth + 1, k, pool); });
        
        Node* rightNode = buildTree(medianIter + 1, end, depth + 1, k, pool);
        
        node->left = leftFuture.get();
        node->right = rightNode;
    } else {
        node->left = buildTree(begin, medianIter, depth + 1, k, pool);
        node->right = buildTree(medianIter + 1, end, depth + 1, k, pool);
    }

    return node;
}

void nearestNeighbor(Node* root, Point target, int depth, int k, Node*& best, double& bestDist) {
    if (!root) return;

    int axis = depth % k;
    
    double d = 0;
    for(int i=0; i<k; ++i) d += pow(root->point.coords[i] - target.coords[i], 2);
    d = sqrt(d);

    if (d < bestDist) {
        bestDist = d;
        best = root;
    }

    double diff = target.get(axis) - root->point.get(axis);
    Node* near = (diff < 0) ? root->left : root->right;
    Node* far = (diff < 0) ? root->right : root->left;

    nearestNeighbor(near, target, depth + 1, k, best, bestDist);

    if (std::abs(diff) < bestDist) {
        nearestNeighbor(far, target, depth + 1, k, best, bestDist);
    }
}