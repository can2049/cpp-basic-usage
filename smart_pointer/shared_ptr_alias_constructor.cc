#include <iostream>
#include <memory>
#include <string>

// 第三层：Level2 的成员，用于演示三层 aliasing
struct Level3 {
  std::string tag = "level3_data";

  Level3() { std::cout << "[" << __func__ << "] 构造\n"; }
  ~Level3() { std::cout << "[" << __func__ << "] 析构\n"; }
};

// 第二层：Level1 的内层成员，模拟类似 Protobuf 的子消息
struct Level2 {
  std::string name;
  int value = 0;
  Level3 level3;  // 第三层成员，用于三层 aliasing 演示

  Level2() { std::cout << "[" << __func__ << "] 构造\n"; }
  ~Level2() { std::cout << "[" << __func__ << "] 析构\n"; }
};

// 第一层：顶层消息，持有 Level2 成员
struct Level1 {
  Level2 level2;
  std::string other = "other_data";

  Level1() { std::cout << "[" << __func__ << "] 构造\n"; }
  ~Level1() { std::cout << "[" << __func__ << "] 析构\n"; }

  // 模拟 Protobuf 的 Clear()：重置内部字段
  void Clear() {
    std::cout << "[" << __func__ << "] 被调用\n";
    level2.name.clear();
    level2.value = 0;
    other.clear();
  }
};

void DemoSharedOwnership() {
  std::cout << "[" << __func__ << "] " << "===== 场景 1: aliasing constructor 共享所有权 =====\n";
  auto level1 = std::make_shared<Level1>();
  level1->level2.name = "hello";
  level1->level2.value = 42;

  // aliasing：共享 level1 的所有权，但指向 level1->level2
  std::shared_ptr<Level2> level2_ptr(level1, &level1->level2);

  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->name       = " << level2_ptr->name << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->value      = " << level2_ptr->value << "\n";

  std::cout << "[" << __func__ << "] " << "--- 离开作用域，观察析构顺序 ---\n";
}

void DemoDanglingAfterClear() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 2: 父对象 Clear() 后，aliasing 指针悬空 =====\n";
  auto level1 = std::make_shared<Level1>();
  level1->level2.name = "hello";
  level1->level2.value = 42;

  std::shared_ptr<Level2> level2_ptr(level1, &level1->level2);
  std::cout << "[" << __func__ << "] " << "Clear 前 level2_ptr->name = " << level2_ptr->name << "\n";

  level1->Clear();  // 内部数据被重置/释放

  // 此时 level2_ptr 仍然“认为”自己有效（use_count 依然 > 0）
  // 但 level2 内部的数据已经被清空，逻辑上已失效
  std::cout << "[" << __func__ << "] " << "Clear 后 level2_ptr->name = '" << level2_ptr->name << "' (长度 "
            << level2_ptr->name.size() << ")\n";
  std::cout << "[" << __func__ << "] " << "Clear 后 level2_ptr->value = " << level2_ptr->value << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count() << "\n";
}

void DemoDanglingAfterParentDestroyed() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 3: 父对象被销毁，aliasing 指针成为悬空指针 =====\n";
  std::shared_ptr<Level2> level2_ptr;
  {
    auto level1 = std::make_shared<Level1>();
    level1->level2.name = "hello";

    // aliasing：level2_ptr 与 level1 共享所有权
    level2_ptr = std::shared_ptr<Level2>(level1, &level1->level2);
    std::cout << "[" << __func__ << "] " << "内层作用域内 level2_ptr.use_count() = "
              << level2_ptr.use_count() << "\n";
    // level1 离开作用域，但 level2_ptr 仍持有引用计数，所以 Level1 不会被销毁
  }

  std::cout << "[" << __func__ << "] " << "外层作用域中 level2_ptr.use_count() = " << level2_ptr.use_count()
            << "\n";
  // 此时 Level1 对象仍存活（因为 level2_ptr 还持有引用计数），
  // 所以访问 level2_ptr 是安全的
  std::cout << "[" << __func__ << "] " << "level2_ptr->name = " << level2_ptr->name << "\n";

  std::cout << "[" << __func__ << "] " << "--- 现在释放 level2_ptr，观察析构 ---\n";
}

void DemoAccessAfterParentReset() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 4: 错误用法 —— 父对象被手动 reset 后访问 =====\n";
  auto level1 = std::make_shared<Level1>();
  level1->level2.name = "hello";

  std::shared_ptr<Level2> level2_ptr(level1, &level1->level2);

  // 只保留 aliasing 指针，释放 level1
  // 注意：因为 level2_ptr 与 level1 共享所有权，
  // 所以 level1.reset() 不会销毁 Level1，只是减少引用计数
  level1.reset();
  std::cout << "[" << __func__ << "] " << "level1.reset() 后 level2_ptr.use_count() = "
            << level2_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->name = " << level2_ptr->name
            << " (仍然安全，因为 Level1 还活着)\n";
}

void DemoDanglingStackObject() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 5: 真正危险的悬空 —— 手动 delete 或栈上对象 =====\n";
  // 用一个栈上的 Level1，然后 aliasing 到它的成员
  Level1 stack_level1;
  stack_level1.level2.name = "stack_data";

  // 危险：aliasing 到一个栈对象，shared_ptr 的引用计数毫无意义
  // 这里只是演示，实际中绝对不要这样做
  std::shared_ptr<Level2> level2_ptr(std::shared_ptr<Level1>(), &stack_level1.level2);
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count()
            << " (空控制块)\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr->name = " << level2_ptr->name << "\n";
  // stack_level1 离开作用域后，level2_ptr 立即悬空
  std::cout << "[" << __func__ << "] " << "--- 离开作用域，stack_level1 被销毁，level2_ptr 将悬空 ---\n";
}

void DemoThreeLevelAliasing() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 6: 三层 aliasing —— Level1 -> Level2 -> Level3 =====\n";
  // 第一层：完整对象
  auto level1 = std::make_shared<Level1>();
  level1->level2.name = "level1";
  level1->level2.level3.tag = "level3";
  // 第二层：基于 level1 的 shared_ptr，aliasing 到 level1->level2
  std::shared_ptr<Level2> level2_ptr(level1, &level1->level2);

  // 第三层：基于 level2_ptr（上一层的结果），aliasing 到 level2_ptr->level3
  // 注意：第一个参数是 level2_ptr 而不是 level1，所有权链是 Level3 <- Level2 <- Level1
  std::shared_ptr<Level3> level3_ptr(level2_ptr, &level2_ptr->level3);

  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_ptr.use_count() = " << level2_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr.use_count()  = " << level3_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr->tag         = " << level3_ptr->tag << "\n";

  // 关键验证：只保留最深层指针，释放上面两层，整条链仍然存活
  level1.reset();
  level2_ptr.reset();
  std::cout << "[" << __func__ << "] " << "level1/level2_ptr reset 后 level3_ptr.use_count() = "
            << level3_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_ptr->tag = " << level3_ptr->tag
            << " (Level1 仍存活，因为 level3_ptr 持有控制块)\n";
  std::cout << "[" << __func__ << "] " << "--- 释放 level3_ptr，观察 Level1 -> Level2 -> Level3 析构顺序 ---\n";
}

void DemoThreeLevelAliasingConst() {
  std::cout << "[" << __func__ << "] " << "\n===== 场景 7: 三层 aliasing + shared_ptr<const> —— 只读访问链 =====\n";
  // 第一层：完整对象
  auto level1 = std::make_shared<Level1>();
  level1->level2.name = "const_level1";
  level1->level2.level3.tag = "const_level3";

  // 第二层：aliasing 到 level1->level2，但声明为 shared_ptr<const Level2>
  // shared_ptr<Level1> 可以隐式转换为 shared_ptr<const Level2>（加 const 的 upcast）
  std::shared_ptr<const Level2> level2_const_ptr(level1, &level1->level2);
  std::cout << "[" << __func__ << "] " << "level2_const_ptr->name = " << level2_const_ptr->name
            << " (只读，不能修改)\n";

  // 第三层：基于 level2_const_ptr，aliasing 到 level2_const_ptr->level3，
  // 继续保持 const：shared_ptr<const Level3>
  std::shared_ptr<const Level3> level3_const_ptr(level2_const_ptr,
                                                 &level2_const_ptr->level3);
  std::cout << "[" << __func__ << "] " << "level1.use_count()       = " << level1.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level2_const_ptr.use_count() = " << level2_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr.use_count()  = " << level3_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr->tag          = " << level3_const_ptr->tag << "\n";

  // 编译错误演示（已注释）：const 指针不能修改所指对象
  // level3_const_ptr->tag = "modified";  // error: passing const Level3

  // 关键验证：只保留最深层 const 指针，释放上面两层，整条链仍然存活
  level1.reset();
  level2_const_ptr.reset();
  std::cout << "[" << __func__ << "] " << "level1/level2_const_ptr reset 后 level3_const_ptr.use_count() = "
            << level3_const_ptr.use_count() << "\n";
  std::cout << "[" << __func__ << "] " << "level3_const_ptr->tag = " << level3_const_ptr->tag
            << " (Level1 仍存活，const 不影响所有权，只影响访问权限)\n";
  std::cout << "[" << __func__ << "] " << "--- 释放 level3_const_ptr，观察析构顺序 ---\n";
}

int main() {
  DemoSharedOwnership();
  DemoDanglingAfterClear();
  DemoDanglingAfterParentDestroyed();
  DemoAccessAfterParentReset();
  DemoDanglingStackObject();
  DemoThreeLevelAliasing();
  DemoThreeLevelAliasingConst();

  std::cout << "[" << __func__ << "] " << "\n程序结束\n";
  return 0;
}
