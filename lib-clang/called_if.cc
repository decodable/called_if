#include "called_if.h"

using namespace std;

namespace codible {
string get_string(const CXString &s) {
  string result = clang_getCString(s);
  clang_disposeString(s);
  return result;
}

string get_func_signature(CXCursor c) {
  auto name = clang_getCursorSpelling(c);

  return get_string(name);
}

enum CXChildVisitResult match_call_expr(CXCursor c, CXCursor parent,
                                        CXClientData client_data) {
  auto result = CXChildVisit_Continue;
  auto &call_expr_matched = *(static_cast<CallExprMatched *>(client_data));
  auto &func_decl_matched = *(std::get<1>(call_expr_matched));

  if (!clang_Location_isFromMainFile(clang_getCursorLocation(c))) {
    return CXChildVisit_Continue;
  }

  log_cursor(trace, c);

  switch (clang_getCursorKind(c)) {
  case CXCursor_CallExpr: {
    auto name = get_func_signature(c);
    if (func_decl_matched.find(name) != func_decl_matched.end()) {
      log_cursor(debug, c);
      std::get<0>(call_expr_matched) = name;
      result = CXChildVisit_Break;
    }
    break;
  }
  default:
    result = CXChildVisit_Recurse;
    break;
  }

  return result;
}

/**
 * visit all functions to find out all functions which
 * are calling the given function
 */
enum CXChildVisitResult match_function_decl(CXCursor c, CXCursor parent,
                                            CXClientData client_data) {
  auto result = CXChildVisit_Continue;
  auto &func_decl_matched = *(static_cast<FunctionDeclMatched *>(client_data));

  if (!clang_Location_isFromMainFile(clang_getCursorLocation(c))) {
    return CXChildVisit_Continue;
  }

  log_cursor(trace, c);

  switch (clang_getCursorKind(c)) {
  case CXCursor_FunctionDecl: {
    auto name = get_func_signature(c);
    if (func_decl_matched.find(name) == func_decl_matched.end()) {
      log_cursor(debug, c);
      CallExprMatched call_expr_matched("", &func_decl_matched);
      clang_visitChildren(c, match_call_expr, &call_expr_matched);
      if (!std::get<0>(call_expr_matched).empty()) {
        log_cursor(info, c);
        func_decl_matched.insert(name);
      }
    }
    break;
  }
  default:
    result = CXChildVisit_Recurse;
    break;
  }

  return result;
}

enum CXChildVisitResult match_if_statement(CXCursor c, CXCursor parent,
                                           CXClientData client_data) {
  auto result = CXChildVisit_Continue;
  auto &if_stmt_matched = *(static_cast<IfStmtMatched *>(client_data));
  auto &called_if_matched = *(std::get<0>(if_stmt_matched));
  auto &func_decl_matched = *(std::get<1>(if_stmt_matched));

  if (!clang_Location_isFromMainFile(clang_getCursorLocation(c))) {
    return CXChildVisit_Continue;
  }

  log_cursor(trace, c);

  switch (clang_getCursorKind(c)) {
  case CXCursor_IfStmt:
  case CXCursor_CaseStmt:
  case CXCursor_DefaultStmt: {
    auto name = get_func_signature(c);
    if (called_if_matched.find(name) == called_if_matched.end()) {
      log_cursor(debug, c);
      CallExprMatched call_expr_matched("", &func_decl_matched);
      clang_visitChildren(c, match_call_expr, &call_expr_matched);
      auto &call_expr_name = std::get<0>(call_expr_matched);
      if (!call_expr_name.empty()) {
        log_cursor(info, c);
        called_if_matched.insert(call_expr_name);
      }
    }
    break;
  }
  default:
    result = CXChildVisit_Recurse;
    break;
  }
}

void dump(const FunctionDeclMatched &func_decl_matched, std::ostream &os) {
  for (auto &name : func_decl_matched) {
    os << " - " << name << endl;
  }
}
} // namespace codible

ostream &operator<<(ostream &stream, const CXString &str) {
  stream << codible::get_string(str);
  return stream;
}
