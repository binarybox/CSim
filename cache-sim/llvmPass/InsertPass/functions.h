#include "llvm/IR/Instruction.h"
#include "llvm/Analysis/LoopPass.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"

#include "llvm/IR/Constants.h"

using namespace llvm;

void createExitBlock(Loop * L);
void createEntryBlock(Loop * L);
void mangageDepth(Loop * L);
