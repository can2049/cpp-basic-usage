#include <algorithm>
#include <cmath>
#include <iostream>
#include <limits>
#include <random>
#include <vector>

class TDigest {
 public:
  explicit TDigest(double compression = 100.0)
      : compression_(compression), total_count_(0) {}

  void add(double x, double w = 1.0) {
    // if no centroid, add directly
    if (centroids_.empty()) {
      centroids_.push_back({x, w});
      total_count_ += w;
      return;
    }

    // find the nearest centroid
    auto it =
        std::min_element(centroids_.begin(), centroids_.end(),
                         [x](const Centroid &a, const Centroid &b) {
                           return std::abs(a.mean - x) < std::abs(b.mean - x);
                         });

    // check if merge
    double q = (cumulative_count_before(it) + it->count / 2.0) / total_count_;
    double k = 4 * total_count_ * q * (1 - q) / compression_;

    if (it->count + w <= k) {
      // merge
      double new_count = it->count + w;
      it->mean = (it->mean * it->count + x * w) / new_count;
      it->count = new_count;
    } else {
      // new centroid
      centroids_.push_back({x, w});
      std::sort(
          centroids_.begin(), centroids_.end(),
          [](const Centroid &a, const Centroid &b) { return a.mean < b.mean; });
    }
    total_count_ += w;
  }

  double quantile(double q) const {
    if (centroids_.empty()) return std::numeric_limits<double>::quiet_NaN();
    if (centroids_.size() == 1) return centroids_[0].mean;

    double target_rank = q * total_count_;
    double cumulative = 0.0;
    double prev_mean = centroids_[0].mean;
    double prev_cum = 0.0;

    for (size_t i = 0; i < centroids_.size(); ++i) {
      double m = centroids_[i].mean;
      double c = centroids_[i].count;
      double start = cumulative;
      double end = cumulative + c;

      if (target_rank < end) {
        if (i == 0) {
          return prev_mean + (m - prev_mean) * (target_rank - start) / c;
        } else {
          double left = prev_mean;
          double right = m;
          return left +
                 (right - left) * (target_rank - prev_cum) / (end - prev_cum);
        }
      }
      prev_cum = cumulative;
      cumulative += c;
      prev_mean = m;
    }
    return centroids_.back().mean;
  }

  double size() const { return total_count_; }

 private:
  struct Centroid {
    double mean;
    double count;
  };

  double compression_;
  double total_count_;
  std::vector<Centroid> centroids_;

  double cumulative_count_before(
      typename std::vector<Centroid>::iterator it) const {
    double sum = 0.0;
    for (auto iter = centroids_.begin(); iter != it; ++iter) {
      sum += iter->count;
    }
    return sum;
  }
};

class OnlineVariance {
 public:
  OnlineVariance() : n_(0), mean_(0.0), M2_(0.0) {}

  void add(double x) {
    ++n_;
    double delta = x - mean_;
    mean_ += delta / n_;
    double delta2 = x - mean_;
    M2_ += delta * delta2;
  }

  int count() const { return n_; }

  double mean() const { return mean_; }

  // sample variance
  double variance() const { return (n_ > 1) ? (M2_ / (n_ - 1)) : 0.0; }

  // sample standard deviation
  double stddev() const { return std::sqrt(variance()); }

 private:
  int n_ = 0;
  double mean_ = 0.0;
  double M2_ = 0.0;
};

class StreamingStats {
 public:
  explicit StreamingStats(double t_digest_compression = 100.0)
      : t_digest_(t_digest_compression), online_variance_() {}

  void add(double x) {
    t_digest_.add(x);
    online_variance_.add(x);
  }

  int count() const { return online_variance_.count(); }

  double mean() const { return online_variance_.mean(); }

  double median() const { return quantile(0.5); }

  double variance() const { return online_variance_.variance(); }

  double stddev() const { return online_variance_.stddev(); }

  // q in [0,1]
  double quantile(double q) const { return t_digest_.quantile(q); }

 private:
  TDigest t_digest_;
  OnlineVariance online_variance_;
};

void print_stats(StreamingStats &stats) {
  std::cout << "count: " << stats.count() << "\n";
  std::cout << "mean: " << stats.mean() << "\n";
  std::cout << "variance: " << stats.variance() << "\n";
  std::cout << "stddev: " << stats.stddev() << "\n";

  std::cout << "median: " << stats.median() << "\n";

  std::cout << "1% quantile: " << stats.quantile(0.01) << "\n";
  std::cout << "5% quantile: " << stats.quantile(0.05) << "\n";
  std::cout << "20% quantile: " << stats.quantile(0.2) << "\n";
  std::cout << "35% quantile: " << stats.quantile(0.3) << "\n";
  std::cout << "40% quantile: " << stats.quantile(0.4) << "\n";
  std::cout << "50% quantile: " << stats.quantile(0.5) << "\n";
  std::cout << "60% quantile: " << stats.quantile(0.6) << "\n";
  std::cout << "70% quantile: " << stats.quantile(0.7) << "\n";
  std::cout << "80% quantile: " << stats.quantile(0.8) << "\n";
  std::cout << "90% quantile: " << stats.quantile(0.9) << "\n";
  std::cout << "95% quantile: " << stats.quantile(0.95) << "\n";
  std::cout << "100% quantile: " << stats.quantile(1.0) << "\n";
}

void test_streaming_stats_uniform(int count = 10000) {
  std::cout << "====>" << __func__ << "  begin " << std::endl;
  StreamingStats stats;

  for (int i = 0; i <= count; ++i) {
    stats.add(i);
  }

  print_stats(stats);
}

void test_streaming_stats_normal(int count = 10000) {
  std::cout << "====>" << __func__ << "  begin " << std::endl;
  StreamingStats stats;

  std::random_device rd;
  std::mt19937 gen(rd());
  std::normal_distribution<> d(5.0, 2.0);

  for (int i = 0; i <= count; ++i) {
    stats.add(d(gen));
  }

  print_stats(stats);
}

int main() {
  test_streaming_stats_uniform();
  test_streaming_stats_normal();
}
