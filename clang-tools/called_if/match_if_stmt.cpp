#include "match_if_stmt.h"

namespace codible {
bool MatchIfStmtVisitor::VisitIfStmt(IfStmt *if_stmt) {
  if (in_conditional_) {
    return true;
  }

  in_conditional_ = true;
  this->TraverseStmt(if_stmt->getThen());
  this->TraverseStmt(if_stmt->getElse());
  in_conditional_ = false;

  return true;
}

bool MatchIfStmtVisitor::VisitCaseStmt(CaseStmt *case_stmt) {
  if (in_conditional_) {
    return true;
  }

  in_conditional_ = true;
  this->TraverseStmt(case_stmt->getSubStmt());
  in_conditional_ = false;

  return true;
}

bool MatchIfStmtVisitor::VisitDefaultStmt(DefaultStmt *default_stmt) {
  if (in_conditional_) {
    return true;
  }

  in_conditional_ = true;
  this->TraverseStmt(default_stmt->getSubStmt());
  in_conditional_ = false;

  return true;
}

bool MatchIfStmtVisitor::VisitCallExpr(CallExpr *call_expr) {
  if (!in_conditional_) {
    return true;
  }

  if (matched_) {
    return true;
  }

  auto &called_if_matched = *(get<0>(if_stmt_matched_));
  auto &func_decl_matched = *(get<1>(if_stmt_matched_));
  FunctionDecl *func = call_expr->getDirectCallee();
  std::string name = func->getNameInfo().getAsString();
  if (func_decl_matched.find(name) != func_decl_matched.end()) {
    called_if_matched.insert(name);
    matched_ = true;
  }

  return true;
}
} // namespace codible
