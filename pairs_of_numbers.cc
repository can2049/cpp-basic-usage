#include <algorithm>
#include <iostream>
using namespace std;
long long ans = 1e6 + 6;
int n;
long long rec;
void dfs(int a, int b) {
  if (b == 0) {
    rec = n;
    return;
  }
  if (b == 1) {
    rec += a - 1;
    return;
  }
  rec += a / b;
  dfs(b, a % b);
}

int main() {
  cin >> n;

  for (int i = 1; i < n; i++) {
    rec = 0;
    dfs(n, i);
    ans = min(ans, rec);
  }

  cout << ans << endl;
  return 0;
}
