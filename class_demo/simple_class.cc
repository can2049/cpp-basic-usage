

class baseA {
 public:
  int _ma = 1;
  int _mb = 2;
};

class baseB {
 public:
  int _mc = 3;
  int _md = 4;
};

// derived class
class deriveA : public baseA, public baseB {
 public:
  virtual void print() {}
  int _me = 3;
  int _mf = 4;
};

int main() {
  deriveA a;
  return 0;
}
