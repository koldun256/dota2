#include <csetjmp>
#include <iostream>
#include <string>
#include <vector>

#define ALPH_SIZE 10

struct TrieNode {
  int child_count;
  int to[ALPH_SIZE];
  int depth;

  explicit TrieNode(int _depth) : depth(_depth), child_count(0) {
    for (int i = 0; i < ALPH_SIZE; ++i) {
      to[i] = 0;
    }
  }
};

void AddSnils(std::vector<TrieNode>& trie, const std::string& snils,
              std::vector<int> biggest_clusters) {
  int current_node = 0;

  for (int i = 0; i < snils.size(); i++) {
    ++trie[current_node].child_count;
    if (trie[current_node].to[snils[i] - '0'] == -1) {
      trie.push_back(TrieNode(i));
    }
  }
}

int main() { return 0; }