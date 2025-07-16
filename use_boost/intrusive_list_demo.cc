#include <boost/intrusive/list.hpp>
#include <vector>

class MyClass
    : public boost::intrusive::list_base_hook<>  // This is a derivation hook
{
  int age_;

 public:
  int age() const { return age_; }

  // This is a member hook
  boost::intrusive::list_member_hook<> member_hook_;

  MyClass(int i) : age_(i) {}
};

// Define a list that will store MyClass using the public base hook
typedef boost::intrusive::list<MyClass> BaseList;

// Define a list that will store MyClass using the public member hook
typedef boost::intrusive::list<
    MyClass,
    boost::intrusive::member_hook<MyClass, boost::intrusive::list_member_hook<>,
                                  &MyClass::member_hook_> >
    MemberList;

int main() {
  // Create several MyClass objects, each one with a different value
  std::vector<MyClass> values;
  for (int i = 0; i < 100; ++i) {
    values.push_back(MyClass(i));
  }

  BaseList baselist;
  MemberList memberlist;

  for (auto it = values.begin(); it != values.end(); ++it) {
    // Insert them in the base hook list
    baselist.push_front(*it);
    memberlist.push_back(*it);
  }

  // Now test lists
  {
    auto rbit(baselist.rbegin());
    auto mit(memberlist.begin());

    // Test the objects inserted in the base hook list
    for (auto it = values.begin(); it != values.end(); ++it, ++rbit) {
      if (&*rbit != &*it) {
        return 1;
      }
      if (rbit->age() != it->age()) {
        return 2;
      }
    }

    // Test the objects inserted in the member hook list
    for (auto it = values.begin(); it != values.end(); ++it, ++mit) {
      if (&*mit != &*it) {
        return 3;
      }
      if (mit->age() != it->age()) {
        return 4;
      }
    }
  }

  return 0;
}
