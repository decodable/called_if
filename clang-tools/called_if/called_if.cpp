#include "called_if.h"

using namespace std;

namespace codible {

void dump(const FunctionDeclMatched &func_decl_matched, std::ostream &os) {
  for (auto &name : func_decl_matched) {
    os << " - " << name << endl;
  }
}
} // namespace codible
