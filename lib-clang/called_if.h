#ifndef CALLED_IF_H_
#define CALLED_IF_H_

#include <clang-c/Index.h>
#include <boost/log/trivial.hpp>
#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>

#define log_cursor(l, c)                                                       \
  BOOST_LOG_TRIVIAL(l) << clang_getCursorKindSpelling(clang_getCursorKind(c))  \
                       << ": " << clang_getCursorSpelling(c) << endl

std::ostream &operator<<(std::ostream &stream, const CXString &str);

namespace codible {
std::string get_func_signature(CXCursor c);

enum CXChildVisitResult match_function_decl(CXCursor c, CXCursor parent,
                                            CXClientData client_data);
enum CXChildVisitResult match_if_statement(CXCursor c, CXCursor parent,
                                           CXClientData client_data);

// We want to find out whether the named function is called in any conditional
// statement.

// all functions which call the named function
using FunctionDeclMatched = std::unordered_set<std::string>;

// find out one call expression which call the named function, indirectly or
// directly.
using CallExprMatched = std::tuple<std::string, FunctionDeclMatched *>;

// all if statements which call the named function
using IfStmtMatched = std::tuple<FunctionDeclMatched *, FunctionDeclMatched *>;

void dump(const FunctionDeclMatched &func_decl_matched, std::ostream &os);

} // namespace codible

#endif
