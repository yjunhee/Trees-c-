#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

// Node 구조체 정의
struct Node {
    vector<int> keys;
    vector<Node*> children;
    int order;

    Node(int order) : order(order) {}
};

// Node 생성 함수
Node* createNode(int order) {
    return new Node(order);
}

// 키 삽입 함수
Node* insertKey(Node* node, int key, int& median);

// 분할 함수
Node* splitNode(Node* node, int& median);

// 트리 출력 함수
void printTree(Node* node, int level = 0, bool isRoot = true);

// 트리 삽입 연산 함수
void insert(Node*& root, int key, int order) {
    if (!root) {
        root = createNode(order);
        root->keys.push_back(key);
        return;
    }

    int median;
    Node* splitRoot = insertKey(root, key, median);

    if (splitRoot) {
        Node* newRoot = createNode(order);
        newRoot->keys.push_back(median);
        newRoot->children.push_back(root);
        newRoot->children.push_back(splitRoot);
        root = newRoot;
    }
}

// 키 삽입 함수 정의
Node* insertKey(Node* node, int key, int& median) {
    // 리프 노드인 경우
    if (node->children.empty()) {
        auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
        node->keys.insert(it, key);

        if (node->keys.size() > node->order - 1) {
            return splitNode(node, median);
        }
        return nullptr;
    }

    // 내부 노드인 경우
    auto it = upper_bound(node->keys.begin(), node->keys.end(), key);
    int index = it - node->keys.begin();

    Node* splitChild = insertKey(node->children[index], key, median);

    if (splitChild) {
        node->keys.insert(node->keys.begin() + index, median);
        node->children.insert(node->children.begin() + index + 1, splitChild);

        if (node->keys.size() > node->order - 1) {
            return splitNode(node, median);
        }
    }
    return nullptr;
}

// 분할 함수 정의
Node* splitNode(Node* node, int& median) {
    int midIndex = node->keys.size() / 2;
    median = node->keys[midIndex];

    Node* newNode = createNode(node->order);
    newNode->keys.assign(node->keys.begin() + midIndex + 1, node->keys.end());
    node->keys.resize(midIndex);

    if (!node->children.empty()) {
        newNode->children.assign(node->children.begin() + midIndex + 1, node->children.end());
        node->children.resize(midIndex + 1);
    }

    return newNode;
}

// 트리 출력 함수 정의
void printTree(Node* node, int level, bool isRoot) {
    if (!node) return;

    cout << '<';

    // 현재 노드의 자식 및 키 출력
    for (size_t i = 0; i < node->keys.size(); ++i) {
        if (i < node->children.size()) {
            cout << node->children[i] << ' ';
        }
        cout << node->keys[i] << ' ';
    }

    
    for (Node* child : node->children) {
            cout << ' ';
            printTree(child);
        }
    if (node->children.size() > node->keys.size()) {
        cout << node->children[node->children.size() - 1];
    }
    cout << '>';

    // 자식 노드들에 대해 재귀적으로 출력
    
}

// 메모리 해제 함수
void Clear(Node* node) {
    if (!node) return;

    for (Node* child : node->children) {
        Clear(child);
    }

    delete node;
}

// 메인 함수
int main() {
    Node* Tree = nullptr;
    int size;
    int value;
    char com;
    cin >> size;
 
    while (cin >> com >> value) {
        if (com == 'i') {
            insert(Tree, value, size);
            printTree(Tree);
            cout << "\n";
        }
        else if (com == 'd') {
            printTree(Tree);
            return 0;
        }
        else {
            cerr << "err";
        }
    }
    // 메모리 해제
    Clear(Tree);

    return 0;
}


