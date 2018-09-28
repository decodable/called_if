#include "called_function.h"
#include <iostream>

namespace codible
{
  called_function::called_function(const std::string &name) 
    : name_(name) {
      defined_ = false;
  }

  called_function::called_function(const std::string &&name) 
    : name_(std::move(name)) {
      defined_ = false;
  }

  const std::string &called_function::name() const {
    return name_;
  }

  void called_function::define() {
    defined_ = true;
  }

  bool called_function::is_called(const std::string &name) const {
    return called_.find(name) != called_.end();
  }

  called_function::shared_t called_function::call(const std::string &name) {
    auto it = called_.find(name);
    if (it == called_.end()) {
      auto func = std::make_shared<called_function>(name);
      called_[name] = func;
      return func;
    }

    return it->second;
  }

  called_function::shared_t called_function::call(shared_t func) {
    auto name = func->name();
    auto it = called_.find(name);
    if (it == called_.end()) {
      called_[name] = func;
      return func;
    }

    return it->second;
  }

  void called_function::dump(std::ostream &os, int indent) const {
    os << '|' << std::string(indent, '-') << this->name() << std::endl;
    for (const auto &kv: called_) {
      kv.second->dump(os, indent + 1);
    }
  }
} // namespace codible
