#include <iostream>
using namespace std;

enum Color { RED, BLACK };

struct Node {
  int data;
  Color color = RED;
  Node *left = nullptr;
  Node *right = nullptr;
  Node *parent = nullptr;

  // Constructor
  Node(int data) : data(data) {}
};

// Class representing Red-Black Tree
class RedBlackTree {
 private:
  Node *root_ = nullptr;
  ;

  // Left rotate function
  void leftRotate(Node *&root, Node *&pt) {
    Node *pt_right = pt->right;
    pt->right = pt_right->left;

    if (pt->right != nullptr) pt->right->parent = pt;

    pt_right->parent = pt->parent;

    if (pt->parent == nullptr)
      root = pt_right;
    else if (pt == pt->parent->left)
      pt->parent->left = pt_right;
    else
      pt->parent->right = pt_right;

    pt_right->left = pt;
    pt->parent = pt_right;
  }

  // Right rotate function
  void rightRotate(Node *&root, Node *&pt) {
    Node *pt_left = pt->left;
    pt->left = pt_left->right;

    if (pt->left != nullptr) pt->left->parent = pt;

    pt_left->parent = pt->parent;

    if (pt->parent == nullptr)
      root = pt_left;
    else if (pt == pt->parent->left)
      pt->parent->left = pt_left;
    else
      pt->parent->right = pt_left;

    pt_left->right = pt;
    pt->parent = pt_left;
  }

  // Fix violations after insertion
  void fixViolation(Node *&root, Node *&pt) {
    Node *parent_pt = nullptr;
    Node *grand_parent_pt = nullptr;

    while ((pt != root) && (pt->color == RED) && (pt->parent->color == RED)) {
      parent_pt = pt->parent;
      grand_parent_pt = pt->parent->parent;

      // Parent of pt is left child of grandparent
      if (parent_pt == grand_parent_pt->left) {
        Node *uncle_pt = grand_parent_pt->right;

        // Case 1: Uncle is RED, only recoloring needed
        if (uncle_pt != nullptr && uncle_pt->color == RED) {
          grand_parent_pt->color = RED;
          parent_pt->color = BLACK;
          uncle_pt->color = BLACK;
          pt = grand_parent_pt;
        } else {
          // Case 2: pt is right child -> left rotation required
          if (pt == parent_pt->right) {
            leftRotate(root, parent_pt);
            pt = parent_pt;
            parent_pt = pt->parent;
          }

          // Case 3: pt is left child -> right rotation required
          rightRotate(root, grand_parent_pt);
          swap(parent_pt->color, grand_parent_pt->color);
          pt = parent_pt;
        }
      } else {
        // Parent of pt is right child of grandparent
        Node *uncle_pt = grand_parent_pt->left;

        // Case 1: Uncle is RED, only recoloring needed
        if (uncle_pt != nullptr && uncle_pt->color == RED) {
          grand_parent_pt->color = RED;
          parent_pt->color = BLACK;
          uncle_pt->color = BLACK;
          pt = grand_parent_pt;
        } else {
          // Case 2: pt is left child -> right rotation required
          if (pt == parent_pt->left) {
            rightRotate(root, parent_pt);
            pt = parent_pt;
            parent_pt = pt->parent;
          }

          // Case 3: pt is right child -> left rotation required
          leftRotate(root, grand_parent_pt);
          swap(parent_pt->color, grand_parent_pt->color);
          pt = parent_pt;
        }
      }
    }

    root->color = BLACK;
  }

 public:
  // Constructor
  RedBlackTree() = default;

  // Function to insert a new node
  void insert(const int &data) {
    Node *pt = new Node(data);

    // Perform standard BST insert
    root_ = BSTInsert(root_, pt);

    // Fix Red-Black Tree violations
    fixViolation(root_, pt);
  }

  // Inorder traversal (Utility)
  void inorder() {
    inorderHelper(root_);
    cout << endl;
  }

 private:
  // Standard BST insert
  Node *BSTInsert(Node *root, Node *pt) {
    // leaf node
    if (root == nullptr) return pt;

    if (pt->data < root->data) {
      root->left = BSTInsert(root->left, pt);
      root->left->parent = root;
    } else if (pt->data > root->data) {
      root->right = BSTInsert(root->right, pt);
      root->right->parent = root;
    }

    return root;
  }

  // Utility function for inorder traversal
  void inorderHelper(Node *root) {
    if (root == nullptr) return;

    inorderHelper(root->left);
    cout << root->data << " ";
    inorderHelper(root->right);
  }
};

int main() {
  RedBlackTree tree;

  tree.insert(10);
  tree.insert(20);
  tree.insert(30);
  tree.insert(40);
  tree.insert(50);
  tree.insert(25);

  cout << "Inorder Traversal of Created Tree\n";
  tree.inorder();

  return 0;
}
