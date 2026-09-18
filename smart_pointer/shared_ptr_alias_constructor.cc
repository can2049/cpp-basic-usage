#include <iostream>
#include <memory>
#include <string>

// 第三层：Inner 的成员，用于演示三层 aliasing
struct Deep {
  std::string tag = "deep_data";

  Deep() { std::cout << "  [Deep] 构造\n"; }
  ~Deep() { std::cout << "  [Deep] 析构\n"; }
};

// 模拟一个类似 Protobuf 消息的类
struct Inner {
  std::string name;
  int value = 0;
  Deep deep;  // 第三层成员，用于三层 aliasing 演示

  Inner() { std::cout << "  [Inner] 构造\n"; }
  ~Inner() { std::cout << "  [Inner] 析构\n"; }
};

struct Message {
  Inner inner;
  std::string other = "other_data";

  Message() { std::cout << "  [Message] 构造\n"; }
  ~Message() { std::cout << "  [Message] 析构\n"; }

  // 模拟 Protobuf 的 Clear()：重置内部字段
  void Clear() {
    std::cout << "  [Message] Clear() 被调用\n";
    inner.name.clear();
    inner.value = 0;
    other.clear();
  }
};

void DemoSharedOwnership() {
  std::cout << "===== 场景 1: aliasing constructor 共享所有权 =====\n";
  auto msg = std::make_shared<Message>();
  msg->inner.name = "hello";
  msg->inner.value = 42;

  // aliasing：共享 msg 的所有权，但指向 msg->inner
  std::shared_ptr<Inner> inner_ptr(msg, &msg->inner);

  std::cout << "msg.use_count()       = " << msg.use_count() << "\n";
  std::cout << "inner_ptr.use_count() = " << inner_ptr.use_count() << "\n";
  std::cout << "inner_ptr->name       = " << inner_ptr->name << "\n";
  std::cout << "inner_ptr->value      = " << inner_ptr->value << "\n";

  std::cout << "--- 离开作用域，观察析构顺序 ---\n";
}

void DemoDanglingAfterClear() {
  std::cout << "\n===== 场景 2: 父对象 Clear() 后，aliasing 指针悬空 =====\n";
  auto msg = std::make_shared<Message>();
  msg->inner.name = "hello";
  msg->inner.value = 42;

  std::shared_ptr<Inner> inner_ptr(msg, &msg->inner);
  std::cout << "Clear 前 inner_ptr->name = " << inner_ptr->name << "\n";

  msg->Clear();  // 内部数据被重置/释放

  // 此时 inner_ptr 仍然“认为”自己有效（use_count 依然 > 0）
  // 但 inner 内部的数据已经被清空，逻辑上已失效
  std::cout << "Clear 后 inner_ptr->name = '" << inner_ptr->name << "' (长度 "
            << inner_ptr->name.size() << ")\n";
  std::cout << "Clear 后 inner_ptr->value = " << inner_ptr->value << "\n";
  std::cout << "inner_ptr.use_count() = " << inner_ptr.use_count() << "\n";
}

void DemoDanglingAfterParentDestroyed() {
  std::cout << "\n===== 场景 3: 父对象被销毁，aliasing 指针成为悬空指针 =====\n";
  std::shared_ptr<Inner> inner_ptr;
  {
    auto msg = std::make_shared<Message>();
    msg->inner.name = "hello";

    // aliasing：inner_ptr 与 msg 共享所有权
    inner_ptr = std::shared_ptr<Inner>(msg, &msg->inner);
    std::cout << "内层作用域内 inner_ptr.use_count() = "
              << inner_ptr.use_count() << "\n";
    // msg 离开作用域，但 inner_ptr 仍持有引用计数，所以 Message 不会被销毁
  }

  std::cout << "外层作用域中 inner_ptr.use_count() = " << inner_ptr.use_count()
            << "\n";
  // 此时 Message 对象仍存活（因为 inner_ptr 还持有引用计数），
  // 所以访问 inner_ptr 是安全的
  std::cout << "inner_ptr->name = " << inner_ptr->name << "\n";

  std::cout << "--- 现在释放 inner_ptr，观察析构 ---\n";
}

void DemoAccessAfterParentReset() {
  std::cout << "\n===== 场景 4: 错误用法 —— 父对象被手动 reset 后访问 =====\n";
  auto msg = std::make_shared<Message>();
  msg->inner.name = "hello";

  std::shared_ptr<Inner> inner_ptr(msg, &msg->inner);

  // 只保留 aliasing 指针，释放 msg
  // 注意：因为 inner_ptr 与 msg 共享所有权，
  // 所以 msg.reset() 不会销毁 Message，只是减少引用计数
  msg.reset();
  std::cout << "msg.reset() 后 inner_ptr.use_count() = "
            << inner_ptr.use_count() << "\n";
  std::cout << "inner_ptr->name = " << inner_ptr->name
            << " (仍然安全，因为 Message 还活着)\n";
}

void DemoDanglingStackObject() {
  std::cout << "\n===== 场景 5: 真正危险的悬空 —— 手动 delete 或栈上对象 =====\n";
  // 用一个栈上的 Message，然后 aliasing 到它的成员
  Message stack_msg;
  stack_msg.inner.name = "stack_data";

  // 危险：aliasing 到一个栈对象，shared_ptr 的引用计数毫无意义
  // 这里只是演示，实际中绝对不要这样做
  std::shared_ptr<Inner> inner_ptr(std::shared_ptr<Message>(), &stack_msg.inner);
  std::cout << "inner_ptr.use_count() = " << inner_ptr.use_count()
            << " (空控制块)\n";
  std::cout << "inner_ptr->name = " << inner_ptr->name << "\n";
  // stack_msg 离开作用域后，inner_ptr 立即悬空
  std::cout << "--- 离开作用域，stack_msg 被销毁，inner_ptr 将悬空 ---\n";
}

void DemoThreeLevelAliasing() {
  std::cout << "\n===== 场景 6: 三层 aliasing —— Message -> Inner -> Deep =====\n";
  // 第一层：完整对象
  auto msg = std::make_shared<Message>();
  msg->inner.name = "level1";
  msg->inner.deep.tag = "level3";

  // 第二层：基于 msg 的 shared_ptr，aliasing 到 msg->inner
  std::shared_ptr<Inner> inner_ptr(msg, &msg->inner);

  // 第三层：基于 inner_ptr（上一层的结果），aliasing 到 inner_ptr->deep
  // 注意：第一个参数是 inner_ptr 而不是 msg，所有权链是 Deep <- Inner <- Message
  std::shared_ptr<Deep> deep_ptr(inner_ptr, &inner_ptr->deep);

  std::cout << "msg.use_count()       = " << msg.use_count() << "\n";
  std::cout << "inner_ptr.use_count() = " << inner_ptr.use_count() << "\n";
  std::cout << "deep_ptr.use_count()  = " << deep_ptr.use_count() << "\n";
  std::cout << "deep_ptr->tag         = " << deep_ptr->tag << "\n";

  // 关键验证：只保留最深层指针，释放上面两层，整条链仍然存活
  msg.reset();
  inner_ptr.reset();
  std::cout << "msg/inner_ptr reset 后 deep_ptr.use_count() = "
            << deep_ptr.use_count() << "\n";
  std::cout << "deep_ptr->tag = " << deep_ptr->tag
            << " (Message 仍存活，因为 deep_ptr 持有控制块)\n";
  std::cout << "--- 释放 deep_ptr，观察 Message -> Inner -> Deep 析构顺序 ---\n";
}

int main() {
  DemoSharedOwnership();
  DemoDanglingAfterClear();
  DemoDanglingAfterParentDestroyed();
  DemoAccessAfterParentReset();
  DemoDanglingStackObject();
  DemoThreeLevelAliasing();

  std::cout << "\n程序结束\n";
  return 0;
}
