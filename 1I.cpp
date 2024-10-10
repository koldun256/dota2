#include <iostream>
#include <queue>
#include <stack>
#include <string>
#include <unordered_set>

struct TrieNode {
  int id;
  TrieNode* to[2];
  TrieNode* link;
  TrieNode* go[2];
  bool is_terminal;

  explicit TrieNode(int i) : id(i) {
    to[0] = go[0] = to[1] = go[1] = nullptr;
    is_terminal = false;
  }

  ~TrieNode() {
    if (to[0] != nullptr) {
      delete to[0];
    }
    if (to[1] != nullptr) {
      delete to[1];
    }
  }
};

struct Trie {
  TrieNode* root;
  int cur_id;
  Trie() : cur_id(0) { root = new TrieNode(0); }
  void AddWord(const std::string& word) {
    TrieNode* current_node = root;

    for (char letter : word) {
      if (current_node->to[letter - '0'] == nullptr) {
        current_node->to[letter - '0'] = new TrieNode(++cur_id);
      }
      current_node = current_node->to[letter - '0'];
    }

    current_node->is_terminal = true;
  }

  void ToDFA() {
    root->link = root;
    for (int i = 0; i < 2; ++i) {
      root->go[i] = (root->to[i] != nullptr ? root->to[i] : root);
    }

    std::queue<TrieNode*> just_visited;
    just_visited.push(root);

    while (!just_visited.empty()) {
      TrieNode* visited = just_visited.front();
      just_visited.pop();
      for (int i = 0; i < 2; ++i) {
        TrieNode* visiting = visited->to[i];
        if (visiting == nullptr) {
          continue;
        }

        visiting->link = (visited == root ? root : visited->link->go[i]);
        if (visiting->link->is_terminal) {
          visiting->is_terminal = true;
        }
        for (int j = 0; j < 2; ++j) {
          visiting->go[j] =
              (visiting->to[j] != nullptr ? visiting->to[j]
                                          : visiting->link->go[j]);
        }

        just_visited.push(visiting);
      }
    }
  }

  ~Trie() { delete root; }
};

bool HasNonTerminalCycle(Trie& t) {
  std::stack<TrieNode*> stack;
  std::unordered_set<TrieNode*> path;
  std::unordered_set<TrieNode*> visited;

  stack.push(t.root);

  while (!stack.empty()) {
    TrieNode* cur = stack.top();
    if (path.find(cur->go[0]) != path.end() ||
        path.find(cur->go[1]) != path.end()) {
      return true;
    }

    if (visited.find(cur->go[0]) == visited.end() && !cur->go[0]->is_terminal) {
      path.insert(cur);
      stack.push(cur->go[0]);
      continue;
    }

    if (visited.find(cur->go[1]) == visited.end() && !cur->go[1]->is_terminal) {
      path.insert(cur);
      stack.push(cur->go[1]);
      continue;
    }
    // Если все пути пройдены
    visited.insert(cur);
    stack.pop();
    path.erase(stack.top());
  }
  return false;
}

int main() {
  std::ios_base::sync_with_stdio(false);
  std::cin.tie(nullptr);
  int n;
  std::cin >> n;
  Trie t;
  for (int i = 0; i < n; ++i) {
    std::string s;
    std::cin >> s;
    t.AddWord(s);
  }
  t.ToDFA();
  std::cout << (HasNonTerminalCycle(t) ? "TAK" : "NIE") << std::endl;
  return 0;
}