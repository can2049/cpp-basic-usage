

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

class baseC {
 public:
  virtual void func_from_c() {}
  int _me = 5;
  int _mf = 6;
};

class baseD {
 public:
  virtual void func_from_d() {}
  int _mg = 7;
  int _mh = 8;
};

// derived class
class deriveA : public baseA, public baseB, public baseC, public baseD {
 public:
  virtual void final_func() {}
  virtual void func_from_d() override {
    // do something
  }
  virtual void func_from_c() override {
    // do something
  }
  int _me = 3;
  int _mf = 4;
};

int main() {
  deriveA a;
  return 0;
}
