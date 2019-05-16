#include "functions.h"



void createExitBlock(Loop * L){
  SmallVectorImpl<BasicBlock*> * exitBlocks = new SmallVector<BasicBlock*, 15>();

  L->getExitBlocks(*exitBlocks);
  
  SmallVectorImpl<BasicBlock*> * list = new SmallVector<BasicBlock*, 15>();
  L->getExitingBlocks(*list);
  // add decrease Depth on the exit of the loop
  // iterate over all exit blocks
  for (auto iter = list->begin(), End = list->end(); iter != End; ++iter){
    BasicBlock * bb = *iter;

    for(Instruction &I : *bb){
      if(BranchInst *jump = dyn_cast<BranchInst>(&I)){

        for(int i = 0; i < jump->getNumSuccessors(); i++){

          for(auto exitsIter = exitBlocks->begin(), exitEnd = exitBlocks->end(); exitsIter != exitEnd; ++exitsIter){
            BasicBlock * exitBlock = * exitsIter;
            if(jump->getSuccessor(i) == exitBlock){

              // decrement at the end of the loop
              BasicBlock * insertBB = BasicBlock::Create(bb->getContext(), "callDecrement");

              //create function call
              // llvm function mangling
              Constant *exitFunc = exitBlock->getModule()->getOrInsertFunction("_Z8loopExitv", Type::getVoidTy(exitBlock->getContext()));


              Function * func = dyn_cast<Function>(exitFunc);

              // call function with variables
              CallInst * newInst = CallInst::Create(func,  "", insertBB);

              // jump from the current exit block to the new block
              jump->setSuccessor(i, insertBB);

              // jump from the new block to the old exit block
              insertBB->insertInto(bb->getParent(), exitBlock);
              BranchInst::Create(exitBlock, insertBB);

              for(auto it = exitBlock->phis().begin(), end = exitBlock->phis().end(); it != end; ++it){
                if(llvm::PHINode * phiNode = dyn_cast<llvm::PHINode>(&(*it))){
                  int phiNodeIndex = phiNode->getBasicBlockIndex(bb);
                  if(phiNodeIndex != -1){
                    phiNode->setIncomingBlock(phiNodeIndex, insertBB);
                  }
                }
              }
            };
          }
        }
      }
    }
  }
}

void createEntryBlock(Loop * L, int id, ConstantInt * counterConst, ConstantInt* iteration){

  // get loop preheader (entry)
  BasicBlock * preheader = L->getLoopPreheader();
  // search every instruction in this basic block
  for(Instruction &I : * preheader){
    // if instruction is a jump check all successors
    // and if the branch goes to the loop we insert a BB
    if(BranchInst *jump = dyn_cast<BranchInst>(&I)){
      for(int i = 0; i < jump->getNumSuccessors(); i++){
        if(L->contains(jump->getSuccessor(i))){
          // get the loop id
          ConstantInt * loopId = ConstantInt::get(
            Type::getInt32Ty(L->getHeader()->getContext()),
            id
          );

          // in jump which will go to loop
          BasicBlock * loopBlock = jump->getSuccessor(i);

          // increment at the beginning of the loop
          BasicBlock * insertBB = BasicBlock::Create(preheader->getContext(), "callIncrement");
          //create function call
          // llvm function mangling
          Constant *entryFunc = preheader->getModule()->getOrInsertFunction("_Z9loopStartiii",
                                                                  Type::getVoidTy(preheader->getContext()),
                                                                  loopId->getType(),
                                                                  counterConst->getType(),
                                                                  iteration->getType()
                                                                );

          Function * func = dyn_cast<Function>(entryFunc);

          // call function with variables
          CallInst::Create(func, {loopId, counterConst, iteration}, "", insertBB);

          // jump from the current entry block to the new block
          jump->setSuccessor(i, insertBB);

          // jump from the new block to loop entry
          insertBB->insertInto(preheader->getParent(), loopBlock);
          BranchInst::Create(loopBlock, insertBB);

          //TODO phi node list iteration
          for(auto it = loopBlock->phis().begin(), end = loopBlock->phis().end(); it != end; ++it){
            if(llvm::PHINode * phiNode = dyn_cast<llvm::PHINode>(&(*it))){
              int phiNodeIndex = phiNode->getBasicBlockIndex(preheader);
              if(phiNodeIndex != -1){
                phiNode->setIncomingBlock(phiNodeIndex, insertBB);
              }
            }
          }
          // llvm::PHINode * phiNode = dyn_cast<llvm::PHINode>(&loopBlock->front());
          // int phiNodeIndex = phiNode->getBasicBlockIndex(preheader);
          // phiNode->setIncomingBlock(phiNodeIndex, insertBB);

          // for(auto it = loopBlock->phis().begin(), end = loopBlock->phis().end(); it != end; ++it){
          //   (*it).setIncomingBlock(phiNodeIndex, insertBB);
          // }
        }
      }
    }
  }
}
void addLoopStart(Loop * L){

  Instruction * beginInstruction = L->getHeader()->getFirstNonPHI();
  Constant *entryFunc = beginInstruction->getModule()->getOrInsertFunction("_Z9iterationv",
                                                          Type::getVoidTy(beginInstruction->getContext()));

  Function * func = dyn_cast<Function>(entryFunc);

  // call function with variables
  CallInst::Create(func, "", beginInstruction);
}

void manageDepth(Loop * L, int id, ConstantInt * counter, ConstantInt * iteration){
  createExitBlock(L);
  createEntryBlock(L, id, counter, iteration);
  addLoopStart(L);
  std::vector<Loop*> subloops = L->getSubLoops();
  for(Loop * lp : subloops){
    createExitBlock(lp);
    createEntryBlock(lp, id, counter, iteration);
    addLoopStart(lp);
    // increase Depth function_Z17increaseLoopDepthv

  }
}
