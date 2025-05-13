#include <iostream>
#include <memory>
#include <vector>

using namespace std;

struct Data {
  int age;
};

int main() {
  vector<shared_ptr<Data>> vv{};
  for (int i = 1; i <= 5; i++) {
    auto d = make_shared<Data>();
    d->age = i;
    vv.push_back(d);
  }
  auto& data = vv[4];
  cout << "addr. vv: " << (void*)vv.data() << " data: " << (void*)&data
       << " data-value: " << data->age << endl;
  for (int i = 6; i < 20; i++) {
    auto d = make_shared<Data>();
    d->age = i;
    vv.push_back(d);
    cout << "addr. vv: " << (void*)vv.data() << " data: " << (void*)&data
         << " data-value: " << data->age << endl;
  }
  return 0;
}
