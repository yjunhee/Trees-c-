#include <iostream>
#include <fstream>
#include <stack>


using namespace std;

struct Node {
public:
    Node* left;
    Node* right;
    int key;
    int height;
    int size;
    int bf;
};

Node* getNodeAVL(int newkey) {
    Node* newNode = new Node();
    newNode->key = newkey;
    newNode->left = nullptr;
    newNode->right = nullptr;
    newNode->height = 1;
    newNode->size = 1;
    newNode->bf = 0;
    return newNode;
}

int Size(Node* p) {
    if (p == nullptr) {
        return 0;
    }
    p->size = 1 + Size(p->left) + Size(p->right);
    return p->size;
}

int Height(Node* p) {
    return (p != nullptr) ? p->height : 0;
}

Node* rotateTree(string com, Node* x) {
    if (com == "LL") {
        Node* y = x->left;
        x->left = y->right;
        y->right = x;

        x->height = 1 + max(Height(x->left), Height(x->right));
        x->bf = Height(x->left) - Height(x->right);

        y->height = 1 + max(Height(y->left), Height(y->right));
        y->bf = Height(y->left) - Height(y->right);
        return y;
    }
    else if (com == "RR") {
        Node* y = x->right;  
        x->right = y->left;
        y->left = x;

        x->height = 1 + max(Height(x->left), Height(x->right));
        x->bf = Height(x->left) - Height(x->right);

        y->height = 1 + max(Height(y->left), Height(y->right));
        y->bf = Height(y->left) - Height(y->right);
        return y;
    }
    else if (com == "LR") {
        x->left = rotateTree("RR", x->left);
        return rotateTree("LL", x);
    }
    else if (com == "RL") {   // RL
        x->right = rotateTree("LL", x->right);
        return rotateTree("RR", x);
    }
}

Node* checkBalance(Node* x) {
    if (1 < x->bf) {
        if (x->left->bf > 0) {
            return rotateTree("LL", x);
        }
        else {
            return rotateTree("LR", x);
        }
    }
    else {
        if (x->right->bf < 0) {
            return rotateTree("RR", x);
        }
        else {
            return rotateTree("RL", x);
        }
    }
    return x;
}

int insertAVL(Node*& T, int newKey) {
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

    Node* newNode = getNodeAVL(newKey);
    //insertBst()
    //루트노드가 비었을때

    if (T == nullptr)
        T = newNode;

    else if (newKey < q->key)
        q->left = newNode;
    else
        q->right = newNode;
    Node* x = nullptr;
    Node* f = nullptr;
    //Height(), bf로 밸런스 체크
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();

        int leftHeight = (q->left == nullptr) ? 0 : q->left->height;
        int rightHeight = (q->right == nullptr) ? 0 : q->right->height;
        q->height = 1 + max(leftHeight, rightHeight);
        q->bf = leftHeight - rightHeight;

        if (q->bf > 1 or q->bf < -1) {
            x = q;
            if (!stack.empty()) {
                f = stack.top();
            }
            break;
        }
    }
    if (x == nullptr) {
        return 1;
    }
    if (f == nullptr) {
        T = checkBalance(x);
    }
    else if (f->left == x) {
        f->left = checkBalance(x);
    }
    else if (f->right == x) {
        f->right = checkBalance(x);
    }

    return 1;
}

int deleteAVL(Node*& T, int deleteKey) {
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
    Node* x = nullptr;
    Node* f = nullptr;
    while (!stack.empty()) {
        q = stack.top();
        stack.pop();
        int leftHeight = (q->left == nullptr) ? 0 : q->left->height;
        int rightHeight = (q->right == nullptr) ? 0 : q->right->height;

        q->height = 1 + max(leftHeight, rightHeight);
        q->bf = leftHeight - rightHeight;

        if (q->bf > 1 or q->bf < -1) {
            x = checkBalance(q);
            if (!stack.empty()) {
                f = stack.top();
            }
            if (q == T) {
                T = x;
            }
            else if (q == f->left) {
                f->left = x;
            }
            else {
                f->right = x;
            }
        }
    }

    return 1;
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
            if (insertAVL(T, key)) {
                InOrder(T);
                cout << "\n";
            }
        }
        else if (com == 'd') {
            if (deleteAVL(T, key)) {
                InOrder(T);
                cout << "\n";
            }
        }
    }
    Clear(T);
    return 0;
}
