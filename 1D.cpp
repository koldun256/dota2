#include <iostream>
#include <vector>

bool eq(char a, char b) { return (a == b || a == '?') /* && b != '?' */; }

std::vector<int> prefix_function(std::string s) {
  int n = static_cast<int>(s.size());
  std::vector<int> p(n, 0);
  for (int i = 1; i < n; i++) {
    // префикс функция точно не больше этого значения + 1
    int cur = p[i - 1];
    // уменьшаем cur значение, пока новый символ не сматчится
    while (!eq(s[i], s[cur]) && cur > 0) {
      cur = p[cur - 1];
    }
    // здесь либо s[i] == s[cur], либо cur == 0
    if (eq(s[i], s[cur])) p[i] = cur + 1;
  }
  return p;
}

int main() {
  prefix_function("??ab??#ccababcc");

  return 0;
}