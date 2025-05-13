#include <cstddef>  // std::size_t
#include <cstdio>   // std::snprintf
#include <cstdlib>  // std::stoul
#include <cstring>  // std::snprintf
#include <iomanip>  // 格式化输出（虽然简化版未直接使用，保留以防后续修改）
#include <iostream>  // 输入输出流
#include <random>    // 随机数生成相关功能
#include <sstream>  // 字符串流（虽然简化版未直接使用，保留以防后续修改）
#include <string>  // 字符串操作

/**
 * @brief 生成指定长度的随机ID字符串
 *
 * 该函数生成一个由十六进制表示的随机字节序列组成的字符串，
 * 每个字节(0-255)被表示为2个十六进制字符。
 * 例如：GenerateId(4)可能返回类似"4a1f0b9e"的结果。
 *
 * @param length 要生成的字节数（返回字符串长度将为length*2）
 * @return std::string 生成的随机ID字符串
 */
std::string GenerateId(size_t length) {
  // 初始化随机数生成器（static保证在多次调用间保持状态）
  // std::random_device{}() 创建临时random_device并立即调用生成种子
  // std::mt19937 使用梅森旋转算法的高质量伪随机数生成器
  static std::mt19937 gen(std::random_device{}());

  // 创建均匀分布，生成0-255之间的随机整数（对应一个字节的所有可能值）
  std::uniform_int_distribution<> dis(0, 255);

  // 预分配结果字符串空间（每个字节需要2个十六进制字符表示）
  std::string id;
  id.reserve(length * 2);  // reserve避免多次内存重新分配

  // 循环生成每个字节的十六进制表示
  for (size_t i = 0; i < length; ++i) {
    char buf[3];  // 缓冲区：2个字符+1个null终止符

    // 将随机数格式化为2位十六进制，不足两位前面补零
    // %02x 表示：
    //   %x - 十六进制格式
    //   02 - 至少2位宽度，不足补零
    snprintf(buf, sizeof(buf), "%02x", dis(gen));

    // 将格式化后的字符串追加到结果中
    id += buf;
  }

  return id;
}

// 测试函数
int main(int argc, char* argv[]) {
  // 检查命令行参数
  if (argc != 2) {
    std::cerr << "Usage: " << argv[0] << " <length>" << std::endl;
    return 1;
  }
  // 将命令行参数转换为整数
  size_t length = std::stoul(argv[1]);
  // 检查长度是否合理
  if (length == 0 || length > 32) {
    std::cerr << "Length must be between 1 and 32." << std::endl;
    return 1;
  }
  // 生成一个长度为指定值的随机ID
  std::string random_id = GenerateId(length);

  // 输出生成的随机ID
  std::cout << "Generated ID: " << random_id << std::endl;

  return 0;
}
