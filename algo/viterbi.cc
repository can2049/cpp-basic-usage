#include <cmath>
#include <iostream>
#include <limits>
#include <vector>

using namespace std;

// 定义HMM模型的参数
const int N = 3;  // 状态数
const int M = 2;  // 观测数
vector<vector<double>> A = {
    {0.5, 0.2, 0.3}, {0.3, 0.5, 0.2}, {0.2, 0.3, 0.5}};  // 状态转移矩阵
vector<vector<double>> B = {
    {0.5, 0.5}, {0.4, 0.6}, {0.7, 0.3}};  // 观测概率矩阵
vector<double> pi = {0.2, 0.4, 0.4};      // 初始状态概率

// 维特比算法
vector<int> viterbi(const vector<int>& obs) {
  int T = obs.size();
  vector<vector<double>> delta(T, vector<double>(N, 0.0));
  vector<vector<int>> psi(T, vector<int>(N, 0));

  // 初始化
  for (int i = 0; i < N; ++i) {
    delta[0][i] = pi[i] * B[i][obs[0]];
    psi[0][i] = 0;
  }

  // 递推
  for (int t = 1; t < T; ++t) {
    for (int j = 0; j < N; ++j) {
      double max_prob = 0.0;
      int max_state = 0;
      for (int i = 0; i < N; ++i) {
        double prob = delta[t - 1][i] * A[i][j];
        if (prob > max_prob) {
          max_prob = prob;
          max_state = i;
        }
      }
      delta[t][j] = max_prob * B[j][obs[t]];
      psi[t][j] = max_state;
    }
  }

  // 终止
  vector<int> states(T);
  double max_prob = 0.0;
  for (int i = 0; i < N; ++i) {
    if (delta[T - 1][i] > max_prob) {
      max_prob = delta[T - 1][i];
      states[T - 1] = i;
    }
  }

  // 回溯
  for (int t = T - 2; t >= 0; --t) {
    states[t] = psi[t + 1][states[t + 1]];
  }

  return states;
}

int main() {
  // 观测序列
  vector<int> obs = {0, 1, 0};  // 例如，0表示观测到"红"，1表示观测到"白"

  // 使用维特比算法找到最可能的状态序列
  vector<int> states = viterbi(obs);

  // 输出结果
  cout << "Most likely state sequence: ";
  for (int state : states) {
    cout << state << " ";
  }
  cout << endl;

  return 0;
}
