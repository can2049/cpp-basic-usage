
#include <iostream>

class Person {
 public:
  Person();
  ~Person();

  int getAge() const;          /*调用方法*/
  int getCallingTimes() const; /*获取上面的getAge()方法被调用了多少次*/
 private:
  int age;
  int mutable m_nums; /*用于统计次数*/
};

Person::Person() { m_nums = 0; }

Person::~Person() {}

int Person::getAge() const {
  std::cout << "Calling the method" << std::endl;
  m_nums++;
  // age = 4; 仍然无法修改该成员变量
  return age;
}

int Person::getCallingTimes() const { return m_nums; }

int main() {
  Person *person = new Person();
  for (int i = 0; i < 10; i++) {
    person->getAge();
  }
  std::cout << "getAge()方法被调用了" << person->getCallingTimes() << "次"
            << std::endl;
  delete person;

  return 0;
}
