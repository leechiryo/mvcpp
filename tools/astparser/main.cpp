#include "clang/Driver/Options.h"
#include "clang/AST/AST.h"
#include "clang/AST/ASTContext.h"
#include "clang/AST/ASTConsumer.h"
#include "clang/AST/RecursiveASTVisitor.h"
#include "clang/Frontend/ASTConsumers.h"
#include "clang/Frontend/FrontendActions.h"
#include "clang/Frontend/CompilerInstance.h"
#include "clang/Tooling/CommonOptionsParser.h"
#include "clang/Tooling/Tooling.h"
#include "clang/Rewrite/Core/Rewriter.h"

#include <vector>
#include <fstream>
#include <unistd.h>

using namespace std;
using namespace clang;
using namespace clang::driver;
using namespace clang::tooling;
using namespace llvm;

Rewriter rewriter;
int numFunctions = 0;
vector<string> responseNames;
SourceLocation insertLoc;
CXXRecordDecl *controllerBase = nullptr;
string srcpath;
string targetClassName = "";
char cwd[1024];
char cwd2[1024];

class ReadFuncVisitor : public RecursiveASTVisitor<ReadFuncVisitor> {
private:
    ASTContext *astContext; // used for getting additional AST info

public:
    explicit ReadFuncVisitor(CompilerInstance *CI) 
      : astContext(&(CI->getASTContext())) // initialize private members
    {
        rewriter.setSourceMgr(astContext->getSourceManager(), astContext->getLangOpts());
    }

    virtual ~ReadFuncVisitor(){
    }

    virtual bool VisitCXXRecordDecl(CXXRecordDecl *record){
      string className = record->getNameAsString();
      if(className == "ControllerBase") controllerBase = record;

      string recordSrcPath = astContext->getSourceManager().getFilename(record->getSourceRange().getEnd());
      realpath(recordSrcPath.c_str(), cwd2);

      if( srcpath == cwd2
          && controllerBase != nullptr 
          && record->isDerivedFrom(controllerBase)
          && "/" + className + ".h" == srcpath.substr(srcpath.find_last_of("/")) )
      {
        targetClassName = className;
      }
      return true;
    }

    virtual bool VisitCXXMethodDecl(CXXMethodDecl *method){
      numFunctions++;
      string className = method->getParent()->getNameAsString();
      
      if(method->isInstance())
      {
        /*
        if(CXXConstructorDecl* constr = dyn_cast<CXXConstructorDecl>(method))
        {
          if(constr->isDefaultConstructor())
            insertLoc = constr->getSourceRange().getEnd();
        }
        */

        string funcName = method->getNameInfo().getAsString();
        unsigned numParams = method->getNumParams();
        if(numParams == 1 
           && method->getType().getAsString() == "void (const class Request &)")
        {
          responseNames.push_back(funcName);
        }
      }
      return true;
    }

/*
    virtual bool VisitFunctionDecl(FunctionDecl *func) {
        numFunctions++;
        string funcName = func->getNameInfo().getAsString();//->getQualifiedNameAsString(); //func->getNameInfo().getName().getAsString();
        unsigned numParams = func->getNumParams();
        errs() << funcName << " " << numParams << " " << func->getType().getAsString() << "\n";
        return true;
    }


    virtual bool VisitCXXRecordDecl(CXXRecordDecl *decl) {

      errs() << decl->getQualifiedNameAsString() << "\n";
      return true;
      
    }

    virtual bool VisitStmt(Stmt *st) {
        if (ReturnStmt *ret = dyn_cast<ReturnStmt>(st)) {
            rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
            errs() << "** Rewrote ReturnStmt\n";
        }        
        if (CallExpr *call = dyn_cast<CallExpr>(st)) {
            rewriter.ReplaceText(call->getLocStart(), 7, "add5");
            errs() << "** Rewrote function call\n";
        }
        return true;
    }

    virtual bool VisitReturnStmt(ReturnStmt *ret) {
        rewriter.ReplaceText(ret->getRetValue()->getLocStart(), 6, "val");
        errs() << "** Rewrote ReturnStmt\n";
        return true;
    }

    virtual bool VisitCallExpr(CallExpr *call) {
        rewriter.ReplaceText(call->getLocStart(), 7, "add5");
        errs() << "** Rewrote function call\n";
        return true;
    }
*/
};



class ReadFuncASTConsumer : public ASTConsumer {

private:
    ReadFuncVisitor *visitor; // doesn't have to be private

public:
    // override the constructor in order to pass CI
    explicit ReadFuncASTConsumer(CompilerInstance *CI)
        : visitor(new ReadFuncVisitor(CI)) // initialize the visitor
    { }

    // override this to call our ReadFuncVisitor on the entire source file
    virtual void HandleTranslationUnit(ASTContext &Context) {
        /* we can use ASTContext to get the TranslationUnitDecl, which is
             a single Decl that collectively represents the entire source file */
        visitor->TraverseDecl(Context.getTranslationUnitDecl());
    }

/*
    // override this to call our ReadFuncVisitor on each top-level Decl
    virtual bool HandleTopLevelDecl(DeclGroupRef DG) {
        // a DeclGroupRef may have multiple Decls, so we iterate through each one
        for (DeclGroupRef::iterator i = DG.begin(), e = DG.end(); i != e; i++) {
            Decl *D = *i;    
            visitor->TraverseDecl(D); // recursively visit each AST node in Decl "D"
        }
        return true;
    }
*/
};



class ReadFuncFrontendAction : public ASTFrontendAction {
public:
    virtual std::unique_ptr<ASTConsumer> CreateASTConsumer(CompilerInstance &CI, StringRef file) {
        return make_unique<ReadFuncASTConsumer>(&CI); // pass CI pointer to ASTConsumer
    }
};



int main(int argc, const char **argv) {


    // parse the command-line args passed to your code
    cl::OptionCategory myCategory("ctrlcpp tool options");
    CommonOptionsParser op(argc, argv, myCategory);        

    // create a new Clang Tool instance (a LibTooling environment)
    ClangTool Tool(op.getCompilations(), op.getSourcePathList());

    srcpath = op.getSourcePathList()[0];
    realpath(srcpath.c_str(), cwd);
    srcpath = cwd;

    // run the Clang Tool, creating a new FrontendAction (explained below)
    int result = Tool.run(newFrontendActionFactory<ReadFuncFrontendAction>().get());

    // result = Tool.run(newFrontendActionFactory<RewriteFrontendAction>());
/*
    for (auto responseName : responseNames)
    {
      rewriter.InsertTextBefore(insertLoc, "  response[\"" + responseName + "\"] = " + responseName + ";\n");
    }

    // print out the rewritten source code ("rewriter" is a global var.)
    rewriter.getEditBuffer(rewriter.getSourceMgr().getMainFileID()).write(errs());
*/

    if (targetClassName == "")
    {
      errs() << "Can't find the controller.\n"
        << "The controller class must be derived (direct or indirect) from "
        << "the ControllerBase class and\n"
        << "has the same name with the source file.\n";

      return 1;
    }

    string objName = targetClassName;
    transform(objName.cbegin(), objName.cbegin()+1, objName.begin(), ::tolower);

    ofstream cppFile;
    string cpfilepath = srcpath.substr(0, srcpath.find_last_of("/")+1) 
                        + targetClassName + "Complete.cpp";
    cppFile.open(cpfilepath);

    
    // output the complete cpp file.
    cppFile << "#include \"" << srcpath.c_str() + srcpath.find_last_of("/") + 1 << "\"\n"
            << "#include \"../ControllerFactoryBase.h\"\n"
            << "\n"
            << "class " << targetClassName << "Complete :\n"
            << "  public " << targetClassName << ",\n"
            << "  public ControllerFactory<" << targetClassName <<"Complete>\n"
            << "{\n"
            << "  public:\n"
            << "  " << targetClassName << "Complete():ControllerFactory(m_ctrlName)\n"
            << "  {\n";

    for (auto responseName : responseNames)
    {
      cppFile << "    responseTbl[\"" << responseName
              << "\"] = static_cast<PtrResponseFunction>(&" 
              << targetClassName << "::" << responseName << ");\n";
    }

    cppFile << "  }\n"
            << "};\n\n"
            << targetClassName << "Complete " << objName << ";\n";

    cppFile.close();

    errs() << "Complete cpp file is created: " << cpfilepath << "\n";

    return result;
}
