#include "called_if.h"
#include "match_func_decl.h"
#include "match_if_stmt.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "llvm/Support/CommandLine.h"

using namespace std;
using namespace clang;
using namespace codible;

int run_match_func_decl(ClangTool &tool,
                        FunctionDeclMatched &func_decl_matched) {
  CustomFrontendActionFactory<FunctionDeclMatched, MatchFuncDeclVisitor>
      factory(func_decl_matched);

  // repeat until no change in run_match_func_decls
  // this could be optimized to reduce the times to repeat
  int result = 0;
  auto nr_of_calling = func_decl_matched.size();
  do {
    nr_of_calling = func_decl_matched.size();
    result = tool.run(&factory);
    if (result != 0) {
      break;
    }
  } while (nr_of_calling != func_decl_matched.size());

  return result;
}

int run_match_if_stmt(ClangTool &tool, IfStmtMatched &if_stmt_matched) {
  CustomFrontendActionFactory<IfStmtMatched, MatchIfStmtVisitor> factory(
      if_stmt_matched);
  auto result = tool.run(&factory);

  return result;
}

int run_clang(const string &func_name, ClangTool &tool) {
  int result = 0;

  FunctionDeclMatched func_decl_matched;
  func_decl_matched.insert(func_name);
  result = run_match_func_decl(tool, func_decl_matched);
  if (result != 0) {
    return result;
  }

  cout << "functions calling " << func_name << " [" << func_decl_matched.size()
       << "]:" << endl;
  dump(func_decl_matched, cout);

  FunctionDeclMatched called_if_matched;
  IfStmtMatched if_stmt_matched(&called_if_matched, &func_decl_matched);
  result = run_match_if_stmt(tool, if_stmt_matched);
  if (result != 0) {
    return result;
  }

  cout << "functions calling " << func_name << " and called in conditional ["
       << called_if_matched.size() << "]:" << endl;
  dump(called_if_matched, cout);

  return 0;
}

int main(int argc, const char **argv) {
  static cl::OptionCategory MyToolCategory(
      "called_if options",
      "To find out whether a function is called in conditional.");
  static cl::extrahelp CommonHelp(CommonOptionsParser::HelpMessage);
  static cl::opt<std::string> Filename(
      "f", cl::Required, cl::desc("function name"), cl::cat(MyToolCategory));

  CommonOptionsParser OptionsParser(argc, argv, MyToolCategory);
  ClangTool tool(OptionsParser.getCompilations(),
                 OptionsParser.getSourcePathList());

  return run_clang(Filename, tool);
}
