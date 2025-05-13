

class Data {
 public:
  Data(int d) : d_(d){};
  Data(const Data& other) = default;
  Data& operator=(const Data& other) {
    if (this != &other) {
      //   d_ = other.d_;
    }
    return *this;
  };
  Data(Data&& other) = default;
  ~Data() = default;

 private:
  const int d_;
};

int main() {
  Data d1(1);
  Data d2 = d1;
}
