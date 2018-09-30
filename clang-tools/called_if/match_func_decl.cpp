#include "match_func_decl.h"

namespace codible {
bool MatchFuncDeclVisitor::VisitFunctionDecl(FunctionDecl *func) {
  string name = func->getNameInfo().getName().getAsString();

  func_name_ = name;

  return true;
}

bool MatchFuncDeclVisitor::VisitCallExpr(CallExpr *call_expr) {
  if (func_name_.empty()) {
    return true;
  }

  FunctionDecl *func = call_expr->getDirectCallee();
  std::string name = func->getNameInfo().getAsString();
  if (func_decl_matched_.find(name) != func_decl_matched_.end()) {
    func_decl_matched_.insert(func_name_);
    func_name_ = "";
  }

  return true;
}
} // namespace codible
