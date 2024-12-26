#include <iostream>
#include <fstream>
#include <stack>

using namespace std;

struct Node {
public:
    Node* left, * right;
    int key;
    int height;
    int size;
};

Node* getNodeBST(int newkey) {
    Node* newNode = new Node();
    newNode->key = newkey;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    newNode->size = 1;
    return newNode;
}

int Size(Node* p) {
    p->size = 0;

    if (p->left != nullptr) {
        p->size++;
        Size(p->left);
    }
    if (p->right != nullptr) {
        p->size++;
        Size(p->right);
    }
    return p->size;
}

int insertBST(Node*& T, int newKey) {
    Node* p = T; //루트노드 받아옴
    Node* q = nullptr; //부모노드 임시저장 
    stack<Node*> stack; //부모노드 계속 저장

    while (p != nullptr) { //searchNode()
        //이미 같은 키의 노드가 존재
        if (newKey == p->key) {
            cerr << "i " << newKey << ": The key already exists\n";
            return 0;
        }

        q = p;
        stack.push(q);

        if (newKey < p->key) {
            p = p->left;
        }
        else {
            p = p->right;
        }
    }
    //새로운 노드 생성

    Node* newNode = getNodeBST(newKey);

    //루트노드가 비었을때

    if (T == nullptr)
        T = newNode;

    else if (newKey < q->key)
        q->left = newNode;
    else
        q->right = newNode;

    //Height()
    while (!stack.empty()) {
        int leftHeight;
        int rightHeight;
        q = stack.top();
        stack.pop();
        if (q->left)
            leftHeight = q->left->height;
        else
            leftHeight = 0;
        if (q->right)
            rightHeight = q->right->height;
        else
            rightHeight = 0;

        q->height = 1 + max(leftHeight, rightHeight);
    }
}

int deleteBST(Node*& T, int deleteKey) {
    Node* p = T;
    Node* q = nullptr;
    stack<Node*> stack;

    while (p != nullptr and deleteKey != p->key) { //searchNode
        q = p;
        stack.push(q);

        if (deleteKey < p->key)
            p = p->left;
        else
            p = p->right;
    }
    //삭제할 노드가 없음
    if (p == nullptr) {
        cerr << "d " << deleteKey << ": The key does not exist\n";
        return 0;
    }
    //타겟노드 양옆 확인
    if (p->left != nullptr and p->right != nullptr) {// 2개
        stack.push(p);
        Node* tempNode = p;
        //minNode(), maxNode()
        if (p->left->height < p->right->height or (p->left->height == p->right->height and Size(p->left) < Size(p->right)))
        {
            p = p->right;
            while (p->left != nullptr) {
                stack.push(p);
                p = p->left;
            }
        }
        else {
            p = p->left;
            while (p->right != nullptr) {
                stack.push(p);
                p = p->right;
            }
        }

        tempNode->key = p->key;
        q = stack.top();
    }

    if (p->left == nullptr and p->right == nullptr) {  // 0개
        if (q == nullptr)
            T = nullptr;
        else if (q->left == p)
            q->left = nullptr;
        else
            q->right = nullptr;
    }
    else { // 1개
        if (p->left != nullptr) {
            if (q == nullptr)
                T = T->left;
            else if (q->left == p)
                q->left = p->left;
            else
                q->right = p->left;
        }
        else {
            if (q == nullptr)
                T = T->right;
            else if (q->left == p)
                q->left = p->right;
            else
                q->right = p->right;
        }
    }
    //메모리 누수 방지
    delete p;

    // Height()
    while (!stack.empty()) {
        int leftHeight;
        int rightHeight;
        q = stack.top();
        stack.pop();
        if (q->left)
            leftHeight = q->left->height;
        else
            leftHeight = 0;
        if (q->right)
            rightHeight = q->right->height;
        else
            rightHeight = 0;

        q->height = 1 + max(leftHeight, rightHeight);
    }
}


// 순회 출력
void InOrder(Node* T) {
    // 빈트리
    if (T == nullptr) {
        cout << "";
        return;
    }
    cout << "<"; // 시작
    // 왼쪽
    if (T->left != nullptr)
        InOrder(T->left);
    // 현재 노드 출력
    cout << " " << T->key << " ";
    // 오른쪽
    if (T->right != nullptr)
        InOrder(T->right);
    cout << ">"; // 끝
}

void Clear(Node* T) {
    if (T == nullptr)
        return;
    Clear(T->left);
    Clear(T->right);

    delete T;

}

//테스트
int main() {
    char com;
    int key;
    Node* T = nullptr;
    while (cin >> com >> key) {
        if (com == 'i') {
            if (insertBST(T, key)) {
                InOrder(T);
                cout << "\n";
            }
        }
        else if (com == 'd') {
            if (deleteBST(T, key)) {
                InOrder(T);
                cout << "\n";
            }
        }
    }
    Clear(T);
    return 0;
}


