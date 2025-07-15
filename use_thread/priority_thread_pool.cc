#include <algorithm>
#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <iostream>
#include <memory>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
 public:
  explicit ThreadPool(size_t num_threads) : stop(false) {
    for (size_t i = 0; i < num_threads; ++i) {
      workers.emplace_back([this] {
        while (true) {
          std::shared_ptr<TaskBase> task;
          {
            std::unique_lock<std::mutex> lock(this->queue_mutex);
            this->condition.wait(
                lock, [this] { return this->stop || !this->tasks.empty(); });

            if (this->stop && this->tasks.empty()) return;

            // 从优先队列中取出优先级最高的任务
            task = std::move(this->tasks.top());
            this->tasks.pop();
          }
          task->execute();  // 执行任务
        }
      });
    }
  }

  template <class F, class... Args>
  auto enqueue(int priority, F&& f, Args&&... args)
      -> std::future<typename std::invoke_result_t<F, Args...>> {
    using return_type = typename std::invoke_result_t<F, Args...>;

    // 创建任务包装器
    auto task = std::make_shared<std::packaged_task<return_type()>>(
        [func = std::forward<F>(f),
         tup = std::make_tuple(std::forward<Args>(args)...)]() mutable {
          return std::apply(func, std::move(tup));
        });

    std::future<return_type> res = task->get_future();
    {
      std::lock_guard<std::mutex> lock(queue_mutex);

      // 将任务包装成带优先级的任务对象
      tasks.emplace(std::make_shared<TaskImpl<return_type>>(
          priority, [task]() { (*task)(); }));
    }
    condition.notify_one();
    return res;
  }

  ~ThreadPool() {
    {
      std::lock_guard<std::mutex> lock(queue_mutex);
      stop = true;
    }
    condition.notify_all();
    for (std::thread& worker : workers) worker.join();
  }

 private:
  // 任务基类
  class TaskBase {
   public:
    explicit TaskBase(int priority) : priority_(priority) {}
    virtual ~TaskBase() = default;
    virtual void execute() = 0;

    int priority() const { return priority_; }

   protected:
    int priority_;
  };

  // 任务实现类
  template <typename T>
  class TaskImpl : public TaskBase {
   public:
    TaskImpl(int priority, std::function<T()> func)
        : TaskBase(priority), func_(std::move(func)) {}

    void execute() override { func_(); }

   private:
    std::function<T()> func_;
  };

  // 任务比较器（优先级高的先执行）
  struct TaskCompare {
    bool operator()(const std::shared_ptr<TaskBase>& lhs,
                    const std::shared_ptr<TaskBase>& rhs) {
      return lhs->priority() < rhs->priority();  // 数值越大优先级越高
    }
  };

  std::vector<std::thread> workers;
  std::priority_queue<std::shared_ptr<TaskBase>,
                      std::vector<std::shared_ptr<TaskBase>>, TaskCompare>
      tasks;

  std::mutex queue_mutex;
  std::condition_variable condition;
  std::atomic<bool> stop;
};

// 示例使用
int main() {
  ThreadPool pool(4);  // 创建4个工作线程

  // 添加不同优先级的任务
  auto task1 = pool.enqueue(
      1,
      [](int a, int b) {
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        return a + b;
      },
      2, 3);

  auto task2 = pool.enqueue(3, []() {  // 最高优先级
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return std::string("High priority task completed");
  });

  auto task3 = pool.enqueue(
      2, [](const std::string& s) { return "Hello, " + s; }, "World"

  );

  // 获取结果
  std::cout << "Task1 result: " << task1.get() << std::endl;  // 5
  std::cout << "Task2 result: " << task2.get()
            << std::endl;  // 高优先级任务先完成
  std::cout << "Task3 result: " << task3.get() << std::endl;  // Hello, World

  return 0;
}
