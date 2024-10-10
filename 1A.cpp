#include <iostream>
#include <string>
#include <vector>

char CharAt(const std::string& new_word, const std::string& acc, size_t i) {
    if (i == new_word.size()) {
        return '#';
    }
    return i < new_word.size()
               ? new_word[i]
               : acc[i - new_word.size() - 1 +
                     std::max(static_cast<int>(acc.size()) - static_cast<int>(new_word.size()), 0)];
}

size_t LongestCommonPart(const std::string& new_word, const std::string& acc) {
    size_t n = new_word.size() + std::min(new_word.size(), acc.size()) + 1;
    std::vector<size_t> dp(n);
    for (size_t i = 1; i < n; ++i) {
        size_t j = dp[i - 1];
        while (j > 0 && CharAt(new_word, acc, i) != CharAt(new_word, acc, j)) {
            j = dp[j - 1];
        }
        if (CharAt(new_word, acc, i) == CharAt(new_word, acc, j)) {
            ++j;
        }
        dp[i] = j;
    }
    return dp[n - 1];
}

void Merge(std::string& acc, const std::string& new_word) {
    for (size_t i = LongestCommonPart(new_word, acc); i < new_word.size();
         ++i) {
        acc += new_word[i];
    }
}

int main() {
    std::string acc;
    std::string new_word;
    size_t n;
    std::cin >> n;
    for (size_t i = 0; i < n; i++) {
        std::cin >> new_word;
        Merge(acc, new_word);
    }
    std::cout << acc;
    return 0;
}