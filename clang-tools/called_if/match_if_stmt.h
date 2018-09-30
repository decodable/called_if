#ifndef MATCH_IF_STMT_H_
#define MATCH_IF_STMT_H_

#include "called_if.h"

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

namespace codible {
class MatchIfStmtVisitor : public RecursiveASTVisitor<MatchIfStmtVisitor> {
public:
  explicit MatchIfStmtVisitor(CompilerInstance *CI,
                              IfStmtMatched &if_stmt_matched)
      : if_stmt_matched_(if_stmt_matched), in_conditional_(false),
        matched_(false) {}

  virtual bool VisitIfStmt(IfStmt *if_stmt);
  virtual bool VisitCaseStmt(CaseStmt *case_stmt);
  virtual bool VisitDefaultStmt(DefaultStmt *default_stmt);

  virtual bool VisitCallExpr(CallExpr *call_expr);

private:
  IfStmtMatched &if_stmt_matched_;
  bool in_conditional_;
  bool matched_;
};
} // namespace codible

#endif
