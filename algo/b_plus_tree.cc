#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

// B+树节点结构
struct Node {
  bool isLeaf;  // 是否为叶子节点
  vector<int> keys;  // 键数组：叶子节点存储所有数据，内部节点仅作为分隔键
  vector<Node*> children;  // 内部节点存储子节点指针；叶子节点暂不使用
  Node* next;              // 叶子节点链表指针，便于范围查询

  Node(bool leaf) : isLeaf(leaf), next(nullptr) {}
};

class BPlusTree {
 private:
  Node* root;
  static const int MAX_KEYS = 3;  // 每个节点最多保存的键数
  static const int MIN_KEYS = (MAX_KEYS + 1) / 2;  // 每个节点至少保存的键数

  // 内部递归插入函数，返回 {promoteKey, newNode}，若未分裂则返回 {-1, nullptr}
  pair<int, Node*> insertInternal(Node* node, int key) {
    if (node->isLeaf) {
      auto it = lower_bound(node->keys.begin(), node->keys.end(), key);
      node->keys.insert(it, key);
      if (node->keys.size() <= MAX_KEYS) return {-1, nullptr};

      // 叶子节点分裂
      Node* newLeaf = new Node(true);
      int total = node->keys.size();
      int splitIndex = (total + 1) / 2;  // 例如 total==4 时，splitIndex==2

      newLeaf->keys.assign(node->keys.begin() + splitIndex, node->keys.end());
      node->keys.resize(splitIndex);

      // 更新叶子节点链表
      newLeaf->next = node->next;
      node->next = newLeaf;

      int promoteKey = newLeaf->keys[0];
      return {promoteKey, newLeaf};
    } else {
      // 内部节点：选择合适的子节点
      int pos = 0;
      while (pos < node->keys.size() && key >= node->keys[pos]) pos++;
      auto res = insertInternal(node->children[pos], key);
      if (res.second == nullptr) return {-1, nullptr};

      int newKey = res.first;
      Node* newChild = res.second;
      node->keys.insert(node->keys.begin() + pos, newKey);
      node->children.insert(node->children.begin() + pos + 1, newChild);

      if (node->keys.size() <= MAX_KEYS) return {-1, nullptr};

      // 内部节点分裂
      Node* newInternal = new Node(false);
      int totalKeys = node->keys.size();
      int splitIndex = totalKeys / 2;
      newInternal->keys.assign(node->keys.begin() + splitIndex,
                               node->keys.end());
      newInternal->children.assign(node->children.begin() + splitIndex,
                                   node->children.end());

      node->keys.resize(splitIndex);
      node->children.resize(splitIndex);

      int promoteKey = newInternal->keys[0];
      return {promoteKey, newInternal};
    }
  }

  // 内部删除函数，parent 与 childIndex 分别表示当前节点在父节点 children
  // 中的位置（若无父节点则为 nullptr 和 0）
  bool removeInternal(Node* node, Node* parent, int childIndex, int key) {
    if (node->isLeaf) {
      auto it = find(node->keys.begin(), node->keys.end(), key);
      if (it == node->keys.end()) return false;  // 未找到

      node->keys.erase(it);

      // 如果是根节点，允许键数不足
      if (node == root) return true;

      // 如果删除后满足最小键数，则更新父节点中分隔键（非第一个叶子节点）
      if (node->keys.size() >= MIN_KEYS) {
        if (childIndex > 0 && !node->keys.empty())
          parent->keys[childIndex - 1] = node->keys[0];
        return true;
      }

      // 下溢处理：尝试从左右兄弟借或合并
      Node* leftSibling =
          (childIndex > 0) ? parent->children[childIndex - 1] : nullptr;
      Node* rightSibling = (childIndex < parent->children.size() - 1)
                               ? parent->children[childIndex + 1]
                               : nullptr;

      // 向左借
      if (leftSibling && leftSibling->keys.size() > MIN_KEYS) {
        int borrowed = leftSibling->keys.back();
        leftSibling->keys.pop_back();
        node->keys.insert(node->keys.begin(), borrowed);
        parent->keys[childIndex - 1] = node->keys[0];
        return true;
      }
      // 向右借
      else if (rightSibling && rightSibling->keys.size() > MIN_KEYS) {
        int borrowed = rightSibling->keys.front();
        rightSibling->keys.erase(rightSibling->keys.begin());
        node->keys.push_back(borrowed);
        parent->keys[childIndex] = rightSibling->keys[0];
        return true;
      }
      // 合并：优先与左兄弟合并
      else {
        if (leftSibling) {
          leftSibling->keys.insert(leftSibling->keys.end(), node->keys.begin(),
                                   node->keys.end());
          leftSibling->next = node->next;
          parent->children.erase(parent->children.begin() + childIndex);
          parent->keys.erase(parent->keys.begin() + childIndex - 1);
          delete node;
        } else if (rightSibling) {
          node->keys.insert(node->keys.end(), rightSibling->keys.begin(),
                            rightSibling->keys.end());
          node->next = rightSibling->next;
          parent->children.erase(parent->children.begin() + childIndex + 1);
          parent->keys.erase(parent->keys.begin() + childIndex);
          delete rightSibling;
        }
        return true;
      }
    } else {
      // 内部节点：定位对应的子节点继续删除
      int pos = 0;
      while (pos < node->keys.size() && key >= node->keys[pos]) pos++;
      bool result = removeInternal(node->children[pos], node, pos, key);
      if (!result) return false;

      // 【修正】可能因子节点合并导致 children 数量减少，重新调整 pos
      if (pos >= node->children.size()) pos = node->children.size() - 1;
      Node* child = node->children[pos];

      // 检查子节点是否下溢
      if (child->keys.size() < MIN_KEYS) {
        Node* leftSibling = (pos > 0) ? node->children[pos - 1] : nullptr;
        Node* rightSibling = (pos < node->children.size() - 1)
                                 ? node->children[pos + 1]
                                 : nullptr;
        // 尝试从左兄弟借
        if (leftSibling && leftSibling->keys.size() > MIN_KEYS) {
          child->keys.insert(child->keys.begin(), node->keys[pos - 1]);
          node->keys[pos - 1] = leftSibling->keys.back();
          leftSibling->keys.pop_back();
        }
        // 尝试从右兄弟借
        else if (rightSibling && rightSibling->keys.size() > MIN_KEYS) {
          child->keys.push_back(node->keys[pos]);
          node->keys[pos] = rightSibling->keys.front();
          rightSibling->keys.erase(rightSibling->keys.begin());
        }
        // 合并处理
        else {
          if (leftSibling) {
            leftSibling->keys.push_back(node->keys[pos - 1]);
            leftSibling->keys.insert(leftSibling->keys.end(),
                                     child->keys.begin(), child->keys.end());
            if (!child->isLeaf)
              leftSibling->children.insert(leftSibling->children.end(),
                                           child->children.begin(),
                                           child->children.end());
            node->children.erase(node->children.begin() + pos);
            node->keys.erase(node->keys.begin() + pos - 1);
            delete child;
          } else if (rightSibling) {
            child->keys.push_back(node->keys[pos]);
            child->keys.insert(child->keys.end(), rightSibling->keys.begin(),
                               rightSibling->keys.end());
            if (!child->isLeaf)
              child->children.insert(child->children.end(),
                                     rightSibling->children.begin(),
                                     rightSibling->children.end());
            node->children.erase(node->children.begin() + pos + 1);
            node->keys.erase(node->keys.begin() + pos);
            delete rightSibling;
          }
        }
      }
      return true;
    }
  }

  // 递归销毁节点，防止内存泄漏
  void destroy(Node* node) {
    if (!node) return;
    if (!node->isLeaf) {
      for (Node* child : node->children) destroy(child);
    }
    delete node;
  }

 public:
  BPlusTree() : root(nullptr) {}

  ~BPlusTree() { destroy(root); }

  // 插入 key
  void insert(int key) {
    if (!root) {
      root = new Node(true);
      root->keys.push_back(key);
      return;
    }
    auto res = insertInternal(root, key);
    if (res.second != nullptr) {
      Node* newRoot = new Node(false);
      newRoot->keys.push_back(res.first);
      newRoot->children.push_back(root);
      newRoot->children.push_back(res.second);
      root = newRoot;
    }
  }

  // 查找 key 是否存在
  bool search(int key) {
    if (!root) return false;
    Node* cur = root;
    while (!cur->isLeaf) {
      int pos = 0;
      while (pos < cur->keys.size() && key >= cur->keys[pos]) pos++;
      cur = cur->children[pos];
    }
    return find(cur->keys.begin(), cur->keys.end(), key) != cur->keys.end();
  }

  // 删除 key
  void remove(int key) {
    if (!root) return;
    bool res = removeInternal(root, nullptr, 0, key);
    if (!res) {
      cout << "删除 " << key << " 失败：未找到该键" << endl;
      return;
    }
    // 如果根为内部节点且只有一个孩子，则下沉根节点
    if (root && !root->isLeaf && root->children.size() == 1) {
      Node* oldRoot = root;
      root = root->children[0];
      delete oldRoot;
    }
  }

  // 层次遍历打印树结构，每层输出一行
  void printTree() {
    if (!root) {
      cout << "空树" << endl;
      return;
    }
    queue<Node*> q;
    q.push(root);
    while (!q.empty()) {
      int count = q.size();
      while (count--) {
        Node* node = q.front();
        q.pop();
        cout << "[";
        for (int k : node->keys) cout << k << " ";
        cout << "] ";
        if (!node->isLeaf) {
          for (Node* child : node->children) q.push(child);
        }
      }
      cout << endl;
    }
  }
};

int main() {
  BPlusTree tree;
  // 插入测试数据
  vector<int> keys = {10, 20, 5, 6, 12, 30, 7, 17};
  for (int k : keys) {
    tree.insert(k);
    cout << "插入 " << k << " 后的树结构:" << endl;
    tree.printTree();
    cout << "-----------------------" << endl;
  }

  // 删除测试，注意观察删除过程中合并和借操作的效果
  cout << "删除操作测试:" << endl;
  vector<int> delKeys = {6, 10, 7, 12, 20};
  for (int k : delKeys) {
    cout << "删除 " << k << " 后的树结构:" << endl;
    tree.remove(k);
    tree.printTree();
    cout << "-----------------------" << endl;
  }

  // 查找测试
  int target = 30;
  cout << "查找 " << target << ": " << (tree.search(target) ? "存在" : "不存在")
       << endl;

  return 0;
}
