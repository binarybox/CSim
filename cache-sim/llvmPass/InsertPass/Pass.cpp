#include "functions.cpp"
#include "llvm/Pass.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/Instruction.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include <iostream>
#include <fstream>
#include <string>

#include "llvm/IR/Constants.h"

#include "llvm/IR/DerivedTypes.h"

using namespace llvm;

// the granularity for the profiling data.
// G0 will just send the load address and the size
// G1 will also send a id for every variable and make a extra list where every variable is assigned to a id
// G2 saves a id for every load
static cl::opt<unsigned>
  Granularity("granularity", cl::init(0), cl::desc("the granularity of profiling data"));


// the maximum number of loads. -1 is infinity
static cl::opt<int>
  CounterInit("counter", cl::init(-1), cl::desc("maximum runs of profiles"));

  // the maximum number of loads. -1 is infinity
  static cl::opt<int>
    Iteration("iteration", cl::init(-1), cl::desc("maximum iterations of the loops"));

namespace {
static int loadID = 0;
struct ProfileFunction : public FunctionPass {
    static char ID;
    static int loopID;

    int getLoopID(){
      loopID++;
      return loopID;

    }
    ProfileFunction() : FunctionPass(ID) {
    }
    int i;
    int id;
    std::ofstream varFile;
    int loadID = 0;
    std::deque<Value*> var = {};
    std::deque<int> loop = {};

    bool doInitialization(Module &m){
    }

    bool doFinalization(Module &m){
      // if the granularity is 1 create a file where we want to store all the variables name and their id
      if(Granularity.getValue() == 1){
        std::ofstream initFile("/tmp/profile/profile.variables");
        varFile.open("/tmp/profile/profile.variables");
        for(int i = 0; i < var.size(); i++){
          varFile << i << " " << var[i]->getName().str() << "\n";
        }
      }
      return false;
    }

    void getAnalysisUsage(AnalysisUsage &AU) const override {
      AU.setPreservesCFG();
      AU.addRequired<LoopInfoWrapperPass>();
    }

    bool runOnFunction(Function &F) override {
      // TODO mangel name
      // std::string mangledName;
      // raw_string_ostream mangledNameStream(mangledName);
      // Mangler::getNameWithPrefix(mangledNameStream, "getProfile", F.getParent()->getDataLayout());
      //
      // std::cout << mangledName << "\n";

      // detect all loops in the function
      LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();

      ConstantInt * counterConst = ConstantInt::get(
        Type::getInt32Ty(F.getContext()),
        CounterInit.getValue()
      );

      ConstantInt * iterationConst = ConstantInt::get(
        Type::getInt32Ty(F.getContext()),
        Iteration.getValue()
      );

      for(auto it = F.begin(), end = F.end(); it != end; ++it){
        //BasicBlock * BB = (*bb);
        this->addFunctionCallOnLoad(&(*it));
      }

      // iterate over the loops and profile all of them
      for (LoopInfo::iterator LIT = LI.begin(); LIT != LI.end(); ++LIT) {
        Loop* L = *LIT;
        manageDepth(L, Granularity.getValue(), counterConst, iterationConst);
        loop.push_back(0);
      }
      return true;
    }

    // if the value already has a id return the id
    // otherwise create a new id, store the value and return the id.
    int manageVarList(Value * val){
      for(int i = 0; i < var.size(); i++){
        if(var[i] == val){
          return i;
        }
      }
      var.push_back(val);
      return var.size() - 1;
    }

    // detects the load functions and adds the function call for the profile function
    void addFunctionCallOnLoad(BasicBlock * BB){
      int granularity = Granularity.getValue();

       for(auto it = BB->begin(), end = BB->end(); it != end; ++it) {
         Instruction * I = &(*it);
         // cast instruction to load instruction and check if it is a load
         if(LoadInst *load = dyn_cast<LoadInst>(I)) {
           Value * val = load->getPointerOperand();

           // get the type of this value
           Type * t = val->getType();
           // check if it is a pointer type and if so get the element type
           while(t->isPointerTy()) {
                   t = t->getPointerElementType();
           }
           unsigned s;

           // get the size

           if(t->isVectorTy()) {
             s = t->getScalarSizeInBits();
           }
           else{
             s = t->getPrimitiveSizeInBits();
           }

           Value * size = ConstantInt::get(Type::getInt32Ty(BB->getContext()), s);

           Value * alignment = ConstantInt::get(Type::getInt32Ty(BB->getContext()), load->getAlignment());

           // cast value to int32 pointer for function
           Value * cast = CastInst::CreateTruncOrBitCast(val, Type::getInt32PtrTy(BB->getContext()), "", I);

           Value * IdValue = ConstantInt::get(Type::getInt32Ty(BB->getContext()), 0);

           if(granularity == 1){
             if(GetElementPtrInst * ptrInst = dyn_cast<GetElementPtrInst>(val)){

               // get the id of the variable
               IdValue = ConstantInt::get(Type::getInt32Ty(BB->getContext()), manageVarList(ptrInst->getPointerOperand()));

             }
           }

           Constant *hookFunc = BB->getModule()->getOrInsertFunction("_Z4loadPiii",
             Type::getVoidTy(BB->getContext()),
             cast->getType(),
             size->getType(),
             IdValue->getType()
           );

           Function * hook = dyn_cast<Function>(hookFunc);
           loadID++;

           // call function with variables
           CallInst * newInst = CallInst::Create(hook, {cast, size, IdValue}, "", I);
         }

         // can be uncommented for the store call
         if(StoreInst *store = dyn_cast<StoreInst>(I)){
           Value * val = store->getPointerOperand();
           // get the type of this value
           Type * t = val->getType();
           // check if it is a pointer type and if so get the element type
           while(t->isPointerTy()) {
                   t = t->getPointerElementType();
           }
           unsigned s;
           if(t->isVectorTy()) {
             s = t->getScalarSizeInBits();
           }
           else{
             s = t->getPrimitiveSizeInBits();
           }
           Value * size = ConstantInt::get(Type::getInt32Ty(BB->getContext()), s);

           Value * IdValue = ConstantInt::get(Type::getInt32Ty(BB->getContext()), 0);

           if(granularity == 1){
             if(GetElementPtrInst * ptrInst = dyn_cast<GetElementPtrInst>(val)){

               // get the id of the variable
               IdValue = ConstantInt::get(Type::getInt32Ty(BB->getContext()), manageVarList(ptrInst->getPointerOperand()));

             }
           }

           Value * cast = CastInst::CreateTruncOrBitCast(val, Type::getInt32PtrTy(BB->getContext()), "", I);

           Constant *hookFunc = BB->getModule()->getOrInsertFunction("_Z5storePiii",
                                                                     Type::getVoidTy(BB->getContext()),
                                                                     cast->getType(),
                                                                     size->getType(),
                                                                     IdValue->getType()
                                                                   );

           Function * hook = dyn_cast<Function>(hookFunc);

           // call function with variables
           CallInst * newInst = CallInst::Create(hook, {cast, size, IdValue}, "", I);
         }
       }

    }
};

}  // end of anonymous namespace

char ProfileFunction::ID = 0;

static RegisterPass<ProfileFunction> X("function", "Profile by function",
                                  false /* Only looks at CFG */,
                                  false /* Analysis Pass */);
