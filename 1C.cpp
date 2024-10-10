#include <cstddef>
#include <iostream>
#include <queue>
#include <string>
#include <vector>

#define ALPH_SIZE 26

struct TrieNode {
  std::vector<int> terminal_ids;
  TrieNode* to[ALPH_SIZE];
  TrieNode* link;
  TrieNode* terminal_link;
  TrieNode* go[ALPH_SIZE];

  TrieNode() {
    for (int i = 0; i < ALPH_SIZE; ++i) {
      to[i] = go[i] = nullptr;
    }
  }

  std::pair<TrieNode*, std::vector<int>> Transition(int letter) {
    std::vector<int> found_words;

    TrieNode* node = go[letter];
    while (node != nullptr) {
      found_words.insert(found_words.end(), node->terminal_ids.begin(),
                         node->terminal_ids.end());
      node = node->terminal_link;
    }

    return {go[letter], found_words};
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
  Trie() { root = new TrieNode(); }
  void AddWord(const std::string& word, int id) const {
    TrieNode* current_node = root;

    for (char letter : word) {
      if (current_node->to[letter - 'a'] == nullptr) {
        current_node->to[letter - 'a'] = new TrieNode();
      }
      current_node = current_node->to[letter - 'a'];
    }

    current_node->terminal_ids.push_back(id);
  }

  void ToDFA() {
    root->link = root;
    root->terminal_link = nullptr;
    for (int i = 0; i < ALPH_SIZE; ++i) {
      root->go[i] = (root->to[i] != nullptr ? root->to[i] : root);
    }

    std::queue<TrieNode*> just_visited;
    just_visited.push(root);

    while (!just_visited.empty()) {
      TrieNode* visited = just_visited.front();
      just_visited.pop();
      for (int i = 0; i < ALPH_SIZE; ++i) {
        TrieNode* visiting = visited->to[i];
        if (visiting == nullptr) {
          continue;
        }

        visiting->link = (visited == root ? root : visited->link->go[i]);
        visiting->terminal_link = (visiting->link->terminal_ids.empty()
                                       ? visiting->link->terminal_link
                                       : visiting->link);
        for (int j = 0; j < ALPH_SIZE; ++j) {
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

std::vector<std::vector<int>> FindAllOccurences(
    const std::string& text, const std::vector<std::string>& words) {
  std::vector<std::vector<int>> ans(words.size());

  Trie trie;
  for (size_t i = 0; i < words.size(); ++i) {
    trie.AddWord(words[i], i);
  }

  trie.ToDFA();

  TrieNode* state = trie.root;
  for (size_t i = 0; i < text.size(); ++i) {
    std::pair<TrieNode*, std::vector<int>> transition_result =
        state->Transition(text[i] - 'a');

    for (int found_word : transition_result.second) {
      ans[found_word].push_back(i - words[found_word].size() + 2);
    }

    state = transition_result.first;
  }

  return ans;
}

int main() {
  std::string text;
  std::cin >> text;

  int words_count;
  std::cin >> words_count;

  std::vector<std::string> words(words_count);
  for (int i = 0; i < words_count; ++i) {
    std::cin >> words[i];
  }

  std::vector<std::vector<int>> ans = FindAllOccurences(text, words);
  for (std::vector<int>& word_result : ans) {
    std::cout << word_result.size() << " ";
    for (int occurence : word_result) {
      std::cout << occurence << " ";
    }
    std::cout << std::endl;
  }

  return 0;
}