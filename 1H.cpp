#include <iostream>
#include <string>
#include <vector>

#define ALPH_SIZE 3

struct TrieNode {
  bool is_terminal;
  bool is_root;
  int depth;
  TrieNode* parent;
  int last_char;
  int last_updated;
  TrieNode* to[ALPH_SIZE];
  TrieNode* link;  // link, terminal_link and go are set in UpdateTransitions
  TrieNode* terminal_link;
  TrieNode* go[ALPH_SIZE];

  TrieNode() {
    is_root = true;
    last_updated = -1;
    depth = 0;
    parent = link = this;
    terminal_link = nullptr;
    is_terminal = false;
    for (int i = 0; i < ALPH_SIZE; ++i) {
      to[i] = nullptr;
      go[i] = this;
    }
  }

  TrieNode(int d, TrieNode* p, char lc) : depth(d), parent(p), last_char(lc) {
    is_root = false;
    last_updated = -1;
    is_terminal = false;
    for (int i = 0; i < ALPH_SIZE; ++i) {
      to[i] = nullptr;
    }
  }

  bool IsValid(const std::vector<int>& invalidations) const {
    return (last_updated == -1 ? false : depth < invalidations[last_updated]);
  }

  void UpdateTransitions(const std::vector<int>& invalidations) {
    last_updated = invalidations.size() - 1;
    if (!is_root && !parent->IsValid(invalidations)) {
      parent->UpdateTransitions(invalidations);
    }
    link = (parent->is_root ? parent : parent->link->go[last_char]);
    if (!is_root && !link->IsValid(invalidations)) {
      link->UpdateTransitions(invalidations);
    }
    terminal_link = (link->is_terminal ? link : link->terminal_link);

    for (int j = 0; j < ALPH_SIZE; ++j) {
      go[j] = (to[j] != nullptr ? to[j] : link->go[j]);
    }
  }

  int CalcTerminal() {
    int ans = 0;
    TrieNode* node = this;

    while (node != nullptr) {
      TrieNode* next_terminal = node->terminal_link;
      while (next_terminal != nullptr && !next_terminal->is_terminal) {
        next_terminal = next_terminal->terminal_link;
      }
      node->terminal_link = next_terminal;
      if (node->is_terminal) {
        ++ans;
      }
      node = next_terminal;
    }

    return ans;
  }

  ~TrieNode() {
    for (int i = 0; i < ALPH_SIZE; ++i) {
      if (to[i] != nullptr) {
        delete to[i];
      }
    }
  }
};

struct Trie {
  TrieNode* root;
  TrieNode* state;
  std::vector<int> invalidations;

  Trie() { root = new TrieNode(); }

  void InvalidateLayers(int start_layer) {
    for (int i = invalidations.size() - 1;
         i >= 0 && invalidations[i] > start_layer; i++) {
      invalidations[i] = start_layer;
    }
    invalidations.push_back(start_layer);
  }

  void AddWord(const std::string& word, int offset) {
    // offset = 0;
    TrieNode* current_node = root;
    size_t i = 0;

    while (i < word.size() &&
           current_node->to[word[(i + offset) % word.size()] - 'a'] !=
               nullptr) {
      std::cout << word[(i + offset) % word.size()];
      current_node = current_node->to[word[(i++ + offset) % word.size()] - 'a'];
    }

    invalidations.push_back(i + 1);

    for (; i < word.size(); ++i) {
      std::cout << word[(i + offset) % word.size()];
      current_node->go[word[(i + offset) % word.size()] - 'a'] =
          current_node->to[word[(i + offset) % word.size()] - 'a'] =
              new TrieNode(i + 1, current_node,
                           word[(i + offset) % word.size()] - 'a');
      current_node = current_node->to[word[(i + offset) % word.size()] - 'a'];
    }
    std::cout << "[]" << std::endl;

    current_node->is_terminal = true;
  }

  void DeleteWord(const std::string& word, int offset) const {
    TrieNode* current_node = root;
    for (size_t i = 0; i < word.size(); ++i) {
      current_node = current_node->to[word[(i + offset) % word.size()] - 'a'];
      if (current_node == nullptr) {
        return;
      }
    }
    current_node->is_terminal = false;
  }

  int SearchWords(const std::string& text, int offset) {
    // offset = 0;
    state = root;
    int ans = 0;
    for (size_t i = 0; i < text.size(); ++i) {
      std::cout << text[(i + offset) % text.size()];
      ans += Step(text[(i + offset) % text.size()]);
    }
    std::cout << "|";
    return ans;
  }

  int Step(char c) {
    state = state->go[c - 'a'];
    if (!state->IsValid(invalidations)) {
      state->UpdateTransitions(invalidations);
    }
    return state->CalcTerminal();
  }

  ~Trie() { delete root; }
};

int main() {
  Trie trie;
  int n;
  std::cin >> n;
  int last_result = 0;

  for (int i = 0; i < n; ++i) {
    std::string command;
    std::string content;

    std::cin >> command >> content;

    switch (command[0]) {
      case '+':
        trie.AddWord(content, last_result);
        break;
      case '-':
        trie.DeleteWord(content, last_result);
        break;
      case '?':
        last_result = trie.SearchWords(content, last_result);
        std::cout << last_result << std::endl;
        break;
    }
  }
  return 0;
}