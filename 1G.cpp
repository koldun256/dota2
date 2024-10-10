#include <algorithm>
#include <iostream>
#include <map>
#include <string>
#include <vector>

std::vector<size_t> SVOFunction(const std::string& str) {
  std::vector<size_t> svo(str.size() + 1, 0);

  size_t l = 0;
  size_t r = 0;

  for (size_t i = 1; i < str.size(); ++i) {
    if (i < r) {
      svo[i] = std::min(svo[i - l], r - i);
    }
    while (i + svo[i] < str.size() && str[i + svo[i]] == str[svo[i]]) {
      ++svo[i];
    }
    if (i + svo[i] >= r) {
      l = i;
      r = i + svo[i];
    }
  }

  return svo;
}

int main() {
  std::map<std::string, int> lookup;
  std::vector<std::pair<int, int>> ans;

  int n;
  std::cin >> n;

  for (int i = 0; i < n; ++i) {
    std::string s;
    std::cin >> s;
    lookup[s] = i;
  }

  for (auto [s, i] : lookup) {
    std::string s_rev = s;
    std::reverse(s_rev.begin(), s_rev.end());

    std::vector<size_t> svo_suff = SVOFunction(s_rev + "$" + s);
    for (size_t j = s.size() + 1; j <= 2 * s.size() + 1; ++j) {
      if (j + svo_suff[j] == 2 * s.size() + 1) {
        std::string palindrome_complement =
            s_rev.substr(svo_suff[j], s.size() - svo_suff[j]);
        std::map<std::string, int>::iterator it =
            lookup.find(palindrome_complement);
        if (it != lookup.end() && it->second != i &&
            s.size() != it->first.size()) {
          ans.push_back({i + 1, it->second + 1});
        }
      }
    }

    std::vector<size_t> svo_pref = SVOFunction(s + "$" + s_rev);
    for (size_t j = s.size() + 1; j <= 2 * s.size() + 1; ++j) {
      if (j + svo_pref[j] == 2 * s.size() + 1) {
        std::string palindrome_complement =
            s_rev.substr(0, s.size() - svo_pref[j]);
        std::map<std::string, int>::iterator it =
            lookup.find(palindrome_complement);
        if (it != lookup.end() && it->second != i) {
          ans.push_back({it->second + 1, i + 1});
        }
      }
    }
  }

  std::cout << ans.size() << std::endl;
  for (auto [i, j] : ans) {
    std::cout << i << " " << j << std::endl;
  }

  return 0;
}