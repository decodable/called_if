#ifndef CALLED_FUNCTION_H_
#define CALLED_FUNCTION_H_

#include <string>
#include <map>
#include <memory>

namespace codible
{
  class called_function
  {
  public:
    using shared_t = std::shared_ptr<called_function>;

    called_function(const std::string &name);
    called_function(const std::string &&name);

    const std::string &name() const;

    void define();

    bool is_called(const std::string &name) const;
    shared_t call(const std::string &name);
    shared_t call(shared_t func);

    void dump(std::ostream &os, int indent = 0) const;

  private:
    std::string name_;
    bool defined_;
    std::map<std::string, shared_t> called_;
  };

}  // namespace codible

#endif // CALLED_FUNCTION_H_


