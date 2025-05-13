/*
 * 观察者模式
 * 情景：高数课，ABCD四位同学，A是好学生，去上课，B在寝室睡觉，C在网吧打游戏，D在学校外陪女友逛街
 * 他们约定，如果要点名了，A在QQ群里吼一声，他们立刻赶到教室去。
 * 采用观察者模式实现这个情景的应用。
 */

#include <iostream>
#include <list>
#include <string>

class Observer;

class Subject {
 public:
  virtual ~Subject(){};
  virtual void registerObsvr(Observer* obsvr) = 0;
  virtual void removeObsvr(Observer* obsvr) = 0;
  virtual void notifyObsvrs(const std::string& msg) = 0;
};
class Observer {
 public:
  virtual ~Observer(){};
  virtual void Update(const std::string& msg) = 0;
  virtual std::string getName() = 0;

 protected:
  Observer(){};
};

// -------------------------------------------------
class QQGroup : public Subject {
 public:
  QQGroup() { _observers = new std::list<Observer*>(); }
  void registerObsvr(Observer* obsvr);
  void removeObsvr(Observer* obsvr);
  void notifyObsvrs(const std::string& msg);

 private:
  std::list<Observer*>* _observers;
};

void QQGroup::registerObsvr(Observer* obsvr) { _observers->push_back(obsvr); }

void QQGroup::removeObsvr(Observer* obsvr) {
  if (!_observers->empty()) {
    _observers->remove(obsvr);
  }
}

void QQGroup::notifyObsvrs(const std::string& msg) {
  std::cout << "群消息：" << msg << std::endl;
  for (const auto itr : *_observers) {
    itr->Update(msg);
  }
}

// ------------------------------------------------
class RoomMate : public Observer {
 public:
  RoomMate(std::string name, std::string now, std::string action) {
    _name = name;
    _action = action;
    _now = now;
  };
  void Update(const std::string& msg);
  std::string getName();

 private:
  std::string _name;
  std::string _action;
  std::string _now;
};

std::string RoomMate::getName() { return _name; }

void RoomMate::Update(const std::string& msg) {
  std::cout << "This is " << _name << std::endl;
  if (msg == "点名了") {
    std::cout << "Action: " << _action << std::endl << std::endl;
  } else {
    std::cout << "Go on：" << _now << std::endl << std::endl;
  }
}

// 测试代码
int main() {
  RoomMate* B =
      new RoomMate("B", "sleeping", "get dressed and run to classroom");
  RoomMate* C =
      new RoomMate("C", "playing games", "pay the fee and run to classroom");
  RoomMate* D = new RoomMate(
      "D", "shopping with girl friend",
      "go back to school and be worried about girl friend's angry");

  QQGroup* qqgroup = new QQGroup();
  qqgroup->registerObsvr(B);
  qqgroup->registerObsvr(C);
  qqgroup->registerObsvr(D);

  qqgroup->notifyObsvrs("目前没点名");
  qqgroup->notifyObsvrs("点名了");

  return 0;
}
