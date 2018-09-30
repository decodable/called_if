#ifndef CALLED_IF_H_
#define CALLED_IF_H_

#include "clang/Frontend/FrontendActions.h"
#include "clang/Tooling/Tooling.h"

#include "clang/AST/ASTConsumer.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/CompilerInstance.h"

#include <iostream>
#include <string>
#include <tuple>
#include <unordered_set>

using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

namespace codible {

// We want to find out whether the named function is called in any conditional
// statement.
//
// 1. find out all functions which call the named function
// 2. from function in 1st step, find out all called in conditional

// all functions which call the named function
using FunctionDeclMatched = std::unordered_set<std::string>;

// find out one call expression which call the named function, indirectly or
// directly.
using CallExprMatched = std::tuple<std::string, FunctionDeclMatched *>;

// all if statements which call the named function
using IfStmtMatched = std::tuple<FunctionDeclMatched *, FunctionDeclMatched *>;

void dump(const FunctionDeclMatched &func_decl_matched, std::ostream &os);

template <class CustomData, class CustomVisitor>
class CustomASTConsumer : public ASTConsumer {
public:
  explicit CustomASTConsumer(CompilerInstance *CI, CustomData &custom_data)
      : visitor_(new CustomVisitor(CI, custom_data)) {}

  // called for each source file
  virtual void HandleTranslationUnit(ASTContext &Context) override {
    visitor_->TraverseDecl(Context.getTranslationUnitDecl());
  }

private:
  CustomVisitor *visitor_;
};

template <class CustomData, class CustomVisitor>
class CustomFrontendAction : public ASTFrontendAction {
public:
  CustomFrontendAction(CustomData &custom_data) : custom_data_(custom_data) {}

  virtual std::unique_ptr<ASTConsumer>
  CreateASTConsumer(CompilerInstance &CI, StringRef file) override {
    return make_unique<CustomASTConsumer<CustomData, CustomVisitor>>(
        &CI, custom_data_);
  }

private:
  CustomData &custom_data_;
};

template <class CustomData, class CustomVisitor>
class CustomFrontendActionFactory : public FrontendActionFactory {
public:
  CustomFrontendActionFactory(CustomData &custom_data)
      : custom_data_(custom_data) {}

  FrontendAction *create() override {
    return new CustomFrontendAction<CustomData, CustomVisitor>(custom_data_);
  }

private:
  CustomData &custom_data_;
};

} // namespace codible

#endif
