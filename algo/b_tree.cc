#include <iostream>
using namespace std;

class BTree;

class BTreeNode {
  int *keys;  // Array of keys
  int t;      // Minimum degree (defines the range for number of keys)
 public:
  BTreeNode **C;  // Array of child pointers
  bool leaf;      // Is true when node is leaf. Otherwise false
  int n;          // Current number of keys

  // Constructor
  BTreeNode(int _t, bool _leaf) : t(_t), leaf(_leaf), n(0) {
    keys = new int[2 * t - 1];
    C = new BTreeNode *[2 * t];
  }

  // Returns the index of the first key that is greater or equal to k.
  int findKey(int k) {
    int idx = 0;
    while (idx < n && keys[idx] < k) ++idx;
    return idx;
  }

  // --- New search method implementation ---
  // Searches for key k in the subtree rooted with this node.
  // Returns a pointer to the node containing k, or nullptr if the search fails.
  BTreeNode *search(int k) {
    int i = 0;
    while (i < n && k > keys[i]) i++;
    if (i < n && keys[i] == k) return this;
    if (leaf) return nullptr;
    return C[i]->search(k);
  }
  // --- End of search method ---

  // Removes key k from the subtree rooted with this node.
  void remove(int k) {
    int idx = findKey(k);

    // If the key is present in this node.
    if (idx < n && keys[idx] == k) {
      if (leaf)
        removeFromLeaf(idx);
      else
        removeFromNonLeaf(idx);
    } else {
      // If this node is a leaf node, then the key is not present in tree.
      if (leaf) {
        cout << "The key " << k << " does not exist in the tree.\n";
        return;
      }

      // The key to be removed is present in the sub-tree rooted with this node.
      bool flag = ((idx == n) ? true : false);

      // If the child where the key is supposed to exist has less than t keys,
      // fill that child.
      if (C[idx]->n < t) fill(idx);

      if (flag && idx > n)
        C[idx - 1]->remove(k);
      else
        C[idx]->remove(k);
    }
  }

  // Remove the key at idx from a leaf node.
  void removeFromLeaf(int idx) {
    for (int i = idx + 1; i < n; ++i) keys[i - 1] = keys[i];
    n--;
  }

  // Remove the key at idx from a non-leaf node.
  void removeFromNonLeaf(int idx) {
    int k = keys[idx];
    if (C[idx]->n >= t) {
      int pred = getPredecessor(idx);
      keys[idx] = pred;
      C[idx]->remove(pred);
    } else if (C[idx + 1]->n >= t) {
      int succ = getSuccessor(idx);
      keys[idx] = succ;
      C[idx + 1]->remove(succ);
    } else {
      merge(idx);
      C[idx]->remove(k);
    }
  }

  // Get the predecessor of keys[idx].
  int getPredecessor(int idx) {
    BTreeNode *cur = C[idx];
    while (!cur->leaf) cur = cur->C[cur->n];
    return cur->keys[cur->n - 1];
  }

  // Get the successor of keys[idx].
  int getSuccessor(int idx) {
    BTreeNode *cur = C[idx + 1];
    while (!cur->leaf) cur = cur->C[0];
    return cur->keys[0];
  }

  // Fill up the child node present in the idx-th position if that child has
  // less than t-1 keys.
  void fill(int idx) {
    if (idx != 0 && C[idx - 1]->n >= t)
      borrowFromPrev(idx);
    else if (idx != n && C[idx + 1]->n >= t)
      borrowFromNext(idx);
    else {
      if (idx != n)
        merge(idx);
      else
        merge(idx - 1);
    }
  }

  // Borrow a key from C[idx-1] and insert it into C[idx].
  void borrowFromPrev(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx - 1];

    for (int i = child->n - 1; i >= 0; --i) child->keys[i + 1] = child->keys[i];

    if (!child->leaf) {
      for (int i = child->n; i >= 0; --i) child->C[i + 1] = child->C[i];
    }

    child->keys[0] = keys[idx - 1];

    if (!leaf) child->C[0] = sibling->C[sibling->n];

    keys[idx - 1] = sibling->keys[sibling->n - 1];

    child->n += 1;
    sibling->n -= 1;
  }

  // Borrow a key from C[idx+1] and insert it into C[idx].
  void borrowFromNext(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[child->n] = keys[idx];

    if (!child->leaf) child->C[child->n + 1] = sibling->C[0];

    keys[idx] = sibling->keys[0];

    for (int i = 1; i < sibling->n; ++i)
      sibling->keys[i - 1] = sibling->keys[i];

    if (!sibling->leaf) {
      for (int i = 1; i <= sibling->n; ++i) sibling->C[i - 1] = sibling->C[i];
    }

    child->n += 1;
    sibling->n -= 1;
  }

  // Merge C[idx] with C[idx+1] after pulling a key from this node.
  void merge(int idx) {
    BTreeNode *child = C[idx];
    BTreeNode *sibling = C[idx + 1];

    child->keys[t - 1] = keys[idx];

    for (int i = 0; i < sibling->n; ++i) child->keys[i + t] = sibling->keys[i];

    if (!child->leaf) {
      for (int i = 0; i <= sibling->n; ++i) child->C[i + t] = sibling->C[i];
    }

    for (int i = idx + 1; i < n; ++i) keys[i - 1] = keys[i];

    for (int i = idx + 2; i <= n; ++i) C[i - 1] = C[i];

    child->n += sibling->n + 1;
    n--;

    delete sibling;
  }

  // Insert a new key into the subtree rooted with this node (assumes node is
  // non-full).
  void insertNonFull(int k) {
    int i = n - 1;

    if (leaf) {
      while (i >= 0 && keys[i] > k) {
        keys[i + 1] = keys[i];
        i--;
      }
      keys[i + 1] = k;
      n++;
    } else {
      while (i >= 0 && keys[i] > k) i--;
      i++;
      if (C[i]->n == 2 * t - 1) {
        splitChild(i, C[i]);
        if (keys[i] < k) i++;
      }
      C[i]->insertNonFull(k);
    }
  }

  // Split the child y of this node. i is index of y in the child array C.
  void splitChild(int i, BTreeNode *y) {
    BTreeNode *z = new BTreeNode(y->t, y->leaf);
    z->n = t - 1;

    for (int j = 0; j < t - 1; j++) z->keys[j] = y->keys[j + t];

    if (!y->leaf) {
      for (int j = 0; j < t; j++) z->C[j] = y->C[j + t];
    }

    y->n = t - 1;

    for (int j = n; j >= i + 1; j--) C[j + 1] = C[j];

    C[i + 1] = z;

    for (int j = n - 1; j >= i; j--) keys[j + 1] = keys[j];

    keys[i] = y->keys[t - 1];
    n = n + 1;
  }

  // Traverse the tree and print keys in sorted order.
  void traverse() {
    int i;
    for (i = 0; i < n; i++) {
      if (!leaf) C[i]->traverse();
      cout << keys[i] << " ";
    }
    if (!leaf) C[i]->traverse();
  }

  // Print the tree structure using indentation to represent levels.
  void print(int indent = 0) {
    for (int i = 0; i < indent; i++) cout << "    ";
    cout << "[ ";
    for (int i = 0; i < n; i++) {
      cout << keys[i] << " ";
    }
    cout << "]" << endl;
    if (!leaf) {
      for (int i = 0; i <= n; i++) {
        if (C[i]) C[i]->print(indent + 1);
      }
    }
  }

  // Destructor to recursively free child nodes.
  ~BTreeNode() {
    if (!leaf) {
      for (int i = 0; i <= n; i++) {
        delete C[i];
      }
    }
    delete[] keys;
    delete[] C;
  }

  friend class BTree;
};

class BTree {
  BTreeNode *root;
  int t;  // Minimum degree
 public:
  // Constructor (Initializes tree as empty)
  BTree(int _t) : root(nullptr), t(_t) {}

  // Traverse the tree
  void traverse() {
    if (root) root->traverse();
  }

  // Search for key k in the tree by forwarding the call to the root.
  BTreeNode *search(int k) {
    return (root == nullptr) ? nullptr : root->search(k);
  }

  // Insert a new key into the B-Tree.
  void insert(int k) {
    if (root == nullptr) {
      root = new BTreeNode(t, true);
      root->keys[0] = k;
      root->n = 1;
    } else {
      if (root->n == 2 * t - 1) {
        BTreeNode *s = new BTreeNode(t, false);
        s->C[0] = root;
        s->splitChild(0, root);
        int i = 0;
        if (s->keys[0] < k) i++;
        s->C[i]->insertNonFull(k);
        root = s;
      } else {
        root->insertNonFull(k);
      }
    }
  }

  // Remove key k from the B-Tree.
  void remove(int k) {
    if (!root) {
      cout << "The tree is empty\n";
      return;
    }

    root->remove(k);

    if (root->n == 0) {
      BTreeNode *tmp = root;
      if (root->leaf)
        root = nullptr;
      else
        root = root->C[0];
      delete tmp;
    }
  }

  // Print the tree structure.
  void print() {
    if (root)
      root->print();
    else
      cout << "Empty tree." << endl;
  }

  ~BTree() { delete root; }
};

int main() {
  // Create a B-Tree with minimum degree 3.
  BTree tree(3);

  // Insert some keys.
  int keys[] = {10, 20, 5, 6, 12, 30, 7, 17};
  int nKeys = sizeof(keys) / sizeof(keys[0]);

  for (int i = 0; i < nKeys; i++) {
    tree.insert(keys[i]);
  }

  cout << "B-Tree structure after insertion:\n";
  tree.print();

  cout << "\nInorder traversal of the tree: ";
  tree.traverse();

  cout << "\n\nSearching for key 12:\n";
  BTreeNode *result = tree.search(12);
  if (result != nullptr) {
    cout << "Found key 12 in node with keys: ";
    result->print();
  } else {
    cout << "Key 12 not found.\n";
  }

  cout << "\nDeleting 6\n";
  tree.remove(6);
  cout << "B-Tree structure after deleting 6:\n";
  tree.print();

  cout << "\nDeleting 13 (non-existent key):\n";
  tree.remove(13);

  cout << "\nDeleting 7\n";
  tree.remove(7);
  cout << "B-Tree structure after deleting 7:\n";
  tree.print();

  // All allocated memory is freed when the tree goes out of scope.
  return 0;
}
