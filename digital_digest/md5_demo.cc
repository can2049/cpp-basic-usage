#include <glog/logging.h>
#include <openssl/md5.h>
#include <stdint.h>
#include <stdio.h>

#include <cassert>
#include <iostream>
#include <string>

// Compute the MD5 hash of a string.
// output is a string of 32 hex digits, uppercase, representing 16 bytes.
std::string ComputeMD5(std::string_view input) {
  unsigned char md5_digest[MD5_DIGEST_LENGTH];
  MD5_CTX ctx;

  MD5_Init(&ctx);
  MD5_Update(&ctx, input.data(), input.length());
  MD5_Final(md5_digest, &ctx);

  std::string output(MD5_DIGEST_LENGTH * 2, '\0');
  for (int i = 0; i < MD5_DIGEST_LENGTH; i++) {
    sprintf(&output[i * 2], "%02X", (unsigned int)md5_digest[i]);
  }
  return output;
}

void show_md5(std::string_view input) {
  auto md5 = ComputeMD5(input);
  std::cout << input << " => " << md5 << std::endl;
}

void assert_md5_eq(std::string_view input, std::string_view expect) {
  auto md5 = ComputeMD5(input);
  CHECK_EQ(md5, expect);
}

int main() {
  std::cout << "show MD5" << std::endl;
  assert_md5_eq("china", "8A7D7BA288CA0F0EA1ECF975B026E8E1");
  assert_md5_eq("hello", "5D41402ABC4B2A76B9719D911017C592");
  assert_md5_eq("world", "7D793037A0760186574B0282F2F435E7");

  std::cout << " all test passed!\n";

  return 0;
}
