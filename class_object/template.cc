class Widget {
 public:
  template <typename T>
  void processPointer(T* p) {
    (void)p;
  }

  //  private:
  // template <>
  // void processPointer<void>(void* p) {
  //   (void)p;
  // }
};

int main() {
  Widget w;
  w.processPointer(new int());
  return 0;
}
