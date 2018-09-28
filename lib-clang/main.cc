#include "called_if.h"

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/trivial.hpp>

namespace logging = boost::log;
using namespace std;
using namespace codible;

enum CXChildVisitResult visit(CXCursor c, CXCursor parent,
                              CXClientData client_data);

void init(const string &severity) {
  auto log_severity = logging::trivial::warning;
  if (severity == "trace") {
    log_severity = logging::trivial::trace;
  } else if (severity == "debug") {
    log_severity = logging::trivial::debug;
  } else if (severity == "info") {
    log_severity = logging::trivial::info;
  }
  logging::core::get()->set_filter(logging::trivial::severity >= log_severity);
}

CXIndex get_index() {
  // context for creating translation units
  int excludeDeclarationsFromPCH = 1;
  int displayDiagnostics = 0;
  return clang_createIndex(excludeDeclarationsFromPCH, displayDiagnostics);
}

CXTranslationUnit get_translation_unit(const char *source_filename,
                                       CXIndex index) {
  const char *const *command_line_args = nullptr;
  int num_command_line_args = 0;
  struct CXUnsavedFile *unsaved_files = nullptr;
  unsigned num_unsaved_files = 0;
  unsigned options = CXTranslationUnit_None;

  auto unit = clang_parseTranslationUnit(
      index, source_filename, command_line_args, num_command_line_args,
      unsaved_files, num_unsaved_files, options);

  if (nullptr == unit) {
    cerr << "Unable to parse translation unit " << source_filename
         << ". Quitting." << endl;
    exit(-1);
  }

  return unit;
}

void run_match_func_decl(int count, const char *filenames[], CXIndex index,
                         FunctionDeclMatched &func_decl_matched) {
  // repeat until no change in run_match_func_decls
  // this could be optimized to reduce the times to repeat
  auto nr_of_calling = 0;
  do {
    // iterate all source files
    for (auto i = 0; i < count; ++i) {
      const char *source_filename = filenames[i];

      auto unit = get_translation_unit(source_filename, index);
      auto cursor = clang_getTranslationUnitCursor(unit);

      nr_of_calling = func_decl_matched.size();
      clang_visitChildren(cursor, match_function_decl, &func_decl_matched);

      clang_disposeTranslationUnit(unit);
    }
  } while (nr_of_calling != func_decl_matched.size());
}

void run_match_if_stmt(int count, const char *filenames[], CXIndex index,
                       IfStmtMatched &func_decl_matched) {
  for (auto i = 0; i < count; ++i) {
    const char *source_filename = filenames[i];

    auto unit = get_translation_unit(source_filename, index);
    auto cursor = clang_getTranslationUnitCursor(unit);

    clang_visitChildren(cursor, match_if_statement, &func_decl_matched);

    clang_disposeTranslationUnit(unit);
  }
}

int run_clang(const char *func_name, int count, const char *filenames[]) {
  auto index = get_index();

  FunctionDeclMatched func_decl_matched;
  func_decl_matched.insert(func_name);
  run_match_func_decl(count, filenames, index, func_decl_matched);

  cout << "matched functions ..." << endl;
  dump(func_decl_matched, cout);

  FunctionDeclMatched called_if_matched;
  IfStmtMatched if_stmt_matched(&called_if_matched, &func_decl_matched);
  run_match_if_stmt(count, filenames, index, if_stmt_matched);

  clang_disposeIndex(index);

  cout << "called if ..." << endl;
  dump(called_if_matched, cout);

  return 0;
}

int main(int argc, char *argv[]) {
  if (argc < 3) {
    return 1;
  }
  int count = argc - 2;
  string severity;

  char **pargv = argv;
  if (strcmp(argv[1], "--debug") == 0) {
    severity = argv[2];
    count -= 2;
    pargv += 2;
  }
  init(severity);

  if (count < 1) {
    return 1;
  }
  const char *func_name = pargv[1];
  const char **filenames = const_cast<const char **>(&pargv[2]);

  return run_clang(func_name, count, filenames);
}
