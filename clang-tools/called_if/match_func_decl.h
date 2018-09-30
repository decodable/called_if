#ifndef MATCH_FUNC_DECL_H_
#define MATCH_FUNC_DECL_H_

#include "called_if.h"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;
using namespace codible;

namespace codible {
class MatchFuncDeclVisitor : public RecursiveASTVisitor<MatchFuncDeclVisitor> {
public:
  explicit MatchFuncDeclVisitor(CompilerInstance *CI,
                                FunctionDeclMatched &func_decl_matched)
      : func_decl_matched_(func_decl_matched) {}

  virtual bool VisitFunctionDecl(FunctionDecl *func);

  virtual bool VisitCallExpr(CallExpr *call_expr);

private:
  FunctionDeclMatched &func_decl_matched_;
  std::string func_name_;
};
} // namespace codible

#endif
