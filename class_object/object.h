#pragma once

#include <string>

#include "log.h"

class Object {
 public:
  Object() : id_{get_id()} {
    LOG(INFO) << __func__ << " default constructor. id: " << id_;
  };
  explicit Object(std::string _name) : name{_name}, id_{get_id()} {
    LOG(INFO) << __func__ << " constructor. id: " << id_ << " name: " << name;
  };
  Object(const Object& rhs) : name{rhs.name}, id_{get_id()} {
    LOG(INFO) << __func__ << " copy constructor. from: " << rhs.id_
              << " get: " << id_;
  };
  Object& operator=(const Object& rhs) {
    name = rhs.name;
    id_ = get_id();
    LOG(INFO) << __func__ << " copy assign. from: " << rhs.id_
              << " get: " << id_;
    return *this;
  };
  Object(Object&& rhs) noexcept {
    name = std::move(rhs.name);
    id_ = get_id();
    LOG(INFO) << __func__ << " move constructor. from: " << rhs.id_
              << " get: " << id_;
  };
  Object& operator=(Object&& rhs) noexcept {
    name = std::move(rhs.name);
    id_ = get_id();
    LOG(INFO) << __func__ << " move assign. from: " << rhs.id_
              << " get: " << id_;
    return *this;
  };

  // without this virutal, the object hold by base pointer will not destruct
  virtual ~Object() { LOG(INFO) << __func__ << " destructor. id: " << id_; }

  int id() { return id_; }
  std::string name = "object default";

 private:
  int get_id() {
    static int id = -1;
    id += 1;
    return id;
  }

  int id_;
};
