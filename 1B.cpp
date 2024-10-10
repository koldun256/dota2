#include <iostream>
#include <string>
#include <vector>

std::vector<size_t> Z(const std::string& s) {
  std::vector<size_t> ans(s.size());
  size_t left = 0;
  size_t right = 0;
  for (size_t i = 0; i < s.size(); ++i) {
    ans[i] =
        std::max(static_cast<size_t>(0), std::min(right - i, ans[i - left]));
    while (i + ans[i] < s.size() && s[ans[i]] == s[i + ans[i]]) {
      ans[i]++;
    }
    if (right < i + ans[i]) {
      right = i + ans[i];
      left = i;
    }
  }
  return ans;
}

int main() {
  std::string s;
  std::cin >> s;

  size_t ans = 1;
  for (size_t i = 0; i < s.size(); ++i) {
    std::vector<size_t> z = Z(s.substr(i));
    for (size_t j = 1; j < s.size() - i; ++j) {
      ans = std::max(ans, (z[j] / j) + 1);
    }
  }

  std::cout << ans;

  return 0;
}