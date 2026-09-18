// 参考 shared_ptr_alias_constructor.cc：用真实的 Protobuf（proto2）消息
// 演示三层 aliasing constructor，以及 shared_ptr<const> 只读链。
// Protobuf 场景的核心价值：aliasing 指针共享父消息的控制块，
// 保证只要 aliasing 指针存活，整条父消息链就不会被销毁。
#include <iostream>
#include <memory>
#include <string>

#include "alias_level.pb.h"

namespace {

void DemoSharedOwnership() {
  std::cout << "[" << __func__ << "] " << "===== 场景 1: protobuf 消息 aliasing 共享所有权 =====\n";
  auto level1 = std::make_shared<smart_pointer::Level1>();
  level1->mutable_level2()->set_name("hello");
  level1->mutable_level2()->set_value(42);

  // aliasing：共享 level1 的所有权，但指向 level1->level2 子消息
  std::shared_ptr<smart_pointer::Level2> level2_ptr(level1, level1->mutable_level2());

  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->name       = " << level2_ptr->name() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->value      = " << level2_ptr->value() << "\n";
}

void DemoThreeLevelAliasing() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 2: protobuf 三层 aliasing —— Level1 -> Level2 -> Level3 =====\n";
  auto level1 = std::make_shared<smart_pointer::Level1>();
  level1->mutable_level2()->set_name("level1");
  level1->mutable_level2()->mutable_level3()->set_tag("level3");

  // 第二层：基于 level1，aliasing 到 level2 子消息
  std::shared_ptr<smart_pointer::Level2> level2_ptr(level1, level1->mutable_level2());
  // 第三层：基于 level2_ptr，aliasing 到 level3 子消息
  std::shared_ptr<smart_pointer::Level3> level3_ptr(level2_ptr,
                                                    level2_ptr->mutable_level3());

  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr.use_count()  = " << level3_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr->tag          = " << level3_ptr->tag() << "\n";

  // 逐层 reset，观察引用计数变化
  level1.reset();
  std::cout << "[" << __func__ << "] " << "level1.reset() 后: level2_ptr.use_count() = "
            << level2_ptr.use_count() << ", level3_ptr.use_count() = "
            << level3_ptr.use_count() << "\n";
  level2_ptr.reset();
  std::cout << "[" << __func__ << "] " << "level2_ptr.reset() 后: level3_ptr.use_count() = "
            << level3_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr->tag = " << level3_ptr->tag()
            << " (整条消息链仍存活)\n";
  std::cout << "[" << __func__ << "] " << "--- 释放 level3_ptr，观察析构 ---\n";
}

void DemoThreeLevelAliasingConst() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 3: protobuf 三层 aliasing + shared_ptr<const> —— 只读链 =====\n";
  auto level1 = std::make_shared<smart_pointer::Level1>();
  level1->mutable_level2()->set_name("const_level1");
  level1->mutable_level2()->mutable_level3()->set_tag("const_level3");

  // shared_ptr<Level1> 隐式转换为 shared_ptr<const Level2>（加 const 转换）
  std::shared_ptr<const smart_pointer::Level2> level2_const_ptr(level1, &level1->level2());
  // 第三层：基于 level2_const_ptr，继续 const
  std::shared_ptr<const smart_pointer::Level3> level3_const_ptr(
      level2_const_ptr, &level2_const_ptr->level3());

  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_const_ptr.use_count() = " << level2_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr.use_count()  = " << level3_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr->tag          = " << level3_const_ptr->tag() << "\n";

  // 编译错误演示（已注释）：const 指针不能修改
  // level3_const_ptr->set_tag("modified");  // error: const 对象没有 set_tag

  // 逐层 reset，每次打印剩余指针的 use_count
  level1.reset();
  std::cout << "[" << __func__ << "] " << "level1.reset() 后: level2_const_ptr.use_count() = "
            << level2_const_ptr.use_count() << ", level3_const_ptr.use_count() = "
            << level3_const_ptr.use_count() << "\n";
  level2_const_ptr.reset();
  std::cout << "[" << __func__ << "] " << "level2_const_ptr.reset() 后: level3_const_ptr.use_count() = "
            << level3_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr->tag = " << level3_const_ptr->tag()
            << " (const 不影响所有权，只影响访问权限)\n";
  std::cout << "[" << __func__ << "] " << "--- 释放 level3_const_ptr，观察析构 ---\n";
  level3_const_ptr.reset();
  std::cout << "[" << __func__ << "] " << "level3_const_ptr.reset() 后: level3_const_ptr.use_count() = "
            << level3_const_ptr.use_count() << " (整条消息链已析构)\n";
}

void DemoClearKeepAliasingAlive() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 4: protobuf Clear() 后，aliasing 指针的语义 =====\n";
  auto level1 = std::make_shared<smart_pointer::Level1>();
  level1->mutable_level2()->set_name("hello");

  std::shared_ptr<smart_pointer::Level2> level2_ptr(level1, level1->mutable_level2());
  std::cout << "[" << __func__ << "] " << "Clear 前 level2_ptr->name = " << level2_ptr->name() << "\n";

  level1->Clear();  // 清空所有字段

  // Clear() 只重置字段值，不销毁子消息对象本身，
  // aliasing 指针依然有效（use_count 不变），但数据已被清空
  std::cout << "[" << __func__ << "] " << "Clear 后 level2_ptr->name = '" << level2_ptr->name()
            << "' (has_name = " << level2_ptr->has_name() << ")\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count()
            << " (指针仍有效，只是数据被清空)\n";
}

void DemoOutliveParentScope() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 5: 父消息离开作用域，aliasing 指针续命 =====\n";
  std::shared_ptr<const smart_pointer::Level2> level2_const_ptr;
  {
    auto level1 = std::make_shared<smart_pointer::Level1>();
    level1->mutable_level2()->set_name("hello");

    // aliasing：level2_const_ptr 与 level1 共享所有权
    level2_const_ptr = std::shared_ptr<const smart_pointer::Level2>(
        level1, &level1->level2());
    std::cout << "[" << __func__ << "] " << "内层作用域内 level2_const_ptr.use_count() = "
              << level2_const_ptr.use_count() << "\n";
    // level1 离开作用域，但 level2_const_ptr 仍持有引用计数，消息不会被销毁
  }

  std::cout << "[" << __func__ << "] " << "外层作用域中 level2_const_ptr.use_count() = "
            << level2_const_ptr.use_count() << "\n";
  // 父消息仍存活（level2_const_ptr 持有引用计数），访问安全
  std::cout << "[" << __func__ << "] " << "level2_const_ptr->name = " << level2_const_ptr->name() << "\n";
  std::cout << "[" << __func__ << "] " << "--- 释放 level2_const_ptr，观察析构 ---\n";
}

void DemoDanglingStackObject() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 6: 真正危险的悬空 —— 栈上 protobuf 消息 =====\n";
  // 栈上的 Level1，然后 aliasing 到它的子消息
  smart_pointer::Level1 stack_level1;
  stack_level1.mutable_level2()->set_name("stack_data");

  // 危险：aliasing 到栈对象，shared_ptr 的引用计数毫无意义
  // 这里只是演示，实际中绝对不要这样做
  std::shared_ptr<smart_pointer::Level2> level2_ptr(
      std::shared_ptr<smart_pointer::Level1>(), stack_level1.mutable_level2());
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count()
            << " (空控制块)\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->name = " << level2_ptr->name() << "\n";
  // stack_level1 离开作用域后，level2_ptr 立即悬空
  std::cout << "[" << __func__ << "] " << "--- 离开作用域，stack_level1 被销毁，level2_ptr 将悬空 ---\n";
}

}  // namespace

int main() {
  DemoSharedOwnership();
  DemoThreeLevelAliasing();
  DemoThreeLevelAliasingConst();
  DemoClearKeepAliasingAlive();
  DemoOutliveParentScope();
  DemoDanglingStackObject();

  std::cout << "[" << __func__ << "] " << "\n程序结束\n";
  return 0;
}
