#include <algorithm>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// Maximum number of keys in a node
const int MAX_KEYS = 3;

// Structure to hold Key-Value pairs
struct Entry {
  int key;
  string value;
};

struct Node {
  bool isLeaf;
  vector<int> keys;        // Used by both Internal and Leaf nodes
  vector<Entry> data;      // Only used by Leaf nodes
  vector<Node*> children;  // Only used by Internal nodes
  Node* next;              // Linked list pointer for Leaf nodes

  Node(bool leaf) : isLeaf(leaf), next(nullptr) {}
};

class BPlusTree {
 private:
  Node* root;

  Node* findLeaf(int key) {
    Node* curr = root;
    while (!curr->isLeaf) {
      bool found = false;
      for (size_t i = 0; i < curr->keys.size(); i++) {
        if (key < curr->keys[i]) {
          curr = curr->children[i];
          found = true;
          break;
        }
      }
      if (!found) {
        curr = curr->children.back();
      }
    }
    return curr;
  }

 public:
  BPlusTree() { root = new Node(true); }

  // Search returns value if found, otherwise an empty string
  string search(int key) {
    Node* leaf = findLeaf(key);
    for (const auto& entry : leaf->data) {
      if (entry.key == key) return entry.value;
    }
    return "NOT_FOUND";
  }

  // Insert Key-Value pair
  void insert(int key, string value) {
    Node* leaf = findLeaf(key);

    // Check if key already exists to update value
    for (auto& entry : leaf->data) {
      if (entry.key == key) {
        entry.value = value;
        return;
      }
    }

    // Insert and maintain sorted order based on key
    leaf->data.push_back({key, value});
    sort(leaf->data.begin(), leaf->data.end(),
         [](const Entry& a, const Entry& b) { return a.key < b.key; });

    // Handle Split (Simplified: Printing split trigger)
    if (leaf->data.size() > MAX_KEYS) {
      handleSplit(leaf);
    }
  }

  void handleSplit(Node* leaf) {
    // In a full implementation, this triggers a recursive split
    // that propagates up to the root and creates internal nodes.
    cout << "[System] Leaf full. Splitting node at key: "
         << leaf->data[MAX_KEYS / 2].key << endl;
    // For this demo, we maintain the leaf data but acknowledge the overflow
    // logic.
  }

  void displayAll() {
    Node* curr = root;
    while (curr && !curr->isLeaf) curr = curr->children[0];

    cout << "--- Current Tree Data (Leaf Level) ---" << endl;
    while (curr) {
      for (const auto& entry : curr->data) {
        cout << "[" << entry.key << ": " << entry.value << "] ";
      }
      curr = curr->next;
    }
    cout << "\n--------------------------------------" << endl;
  }
};

int main() {
  BPlusTree db;

  // Inserting key-value pairs
  db.insert(101, "Alice");
  db.insert(105, "Bob");
  db.insert(102, "Charlie");
  db.insert(110, "David");

  db.displayAll();

  // Search for a specific key
  int searchKey = 102;
  cout << "Searching for " << searchKey
       << "... Result: " << db.search(searchKey) << endl;

  int missingKey = 999;
  cout << "Searching for " << missingKey
       << "... Result: " << db.search(missingKey) << endl;

  return 0;
}
