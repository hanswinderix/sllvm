#include "MSP430.h"
#include "MSP430InstrInfo.h"
#include "MSP430Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/SLLVM.h"

using namespace llvm;

#if 0
#define DEBUG_TYPE "msp430-branch-select"

static cl::opt<bool>
    BranchSelectEnabled("msp430-branch-select", cl::Hidden, cl::init(true),
                        cl::desc("Expand out of range branches"));
#endif

namespace {
class MSP430RTLInternalizer : public MachineFunctionPass {

  MachineFunction *MF;
  const MSP430InstrInfo *TII;

  void internalize(MachineInstr &MI);

public:
  static char ID;
  MSP430RTLInternalizer() : MachineFunctionPass(ID) {}

  bool runOnMachineFunction(MachineFunction &MF) override;

#if 0
  MachineFunctionProperties getRequiredProperties() const override {
    return MachineFunctionProperties().set(
        MachineFunctionProperties::Property::NoVRegs);
  }
#endif

  StringRef getPassName() const override { return "MSP430 RTL Internalizer"; }
};
char MSP430RTLInternalizer::ID = 0;
}

void MSP430RTLInternalizer::internalize(MachineInstr &MI) {
  MachineOperand *MO = MI.operands_begin();
  if (MO->isSymbol()) {
    if (StringRef(MO->getSymbolName()).startswith("__mspabi")) {
      // TODO: Fix mem leak
      std::string * N = new std::string();
      N->append(MO->getSymbolName());
      N->append("_");
      N->append(sllvm::getPMName(MI.getMF()->getFunction().getParent()));
      *MO = MachineOperand::CreateES(N->c_str());
    }
  }
}

bool MSP430RTLInternalizer::runOnMachineFunction(MachineFunction &mf) {
  MF = &mf;
  TII = static_cast<const MSP430InstrInfo *>(MF->getSubtarget().getInstrInfo());

#if 0
  // If the pass is disabled, just bail early.
  if (!BranchSelectEnabled)
    return false;

  LLVM_DEBUG(dbgs() << "\n********** " << getPassName() << " **********\n");
#endif

  // TODO: Move the logic in the condition to sllvm.h in a method 
  //             isPMInternal() or so...
  //       and refactor also the other lib/Target/MSP430 files
  if (! (    sllvm::isPM(mf.getFunction().getParent()) 
          && mf.getFunction().hasLocalLinkage()) ) {
    return false;
  }

  for (auto MBB = MF->begin(), E = MF->end(); MBB != E; ++MBB) {
    for (auto MI = MBB->begin(), EE = MBB->end(); MI != EE; ++MI) {
      //TII->getInstSizeInBytes(*MI);
      //if (MI->isCall())
      if (MI->getOpcode() == MSP430::CALLi) {
        internalize(*MI);
      }
    }
  }

  bool MadeChange = false;

  return MadeChange;
}

/// Returns an instance of the RTL Internalization Pass
FunctionPass *llvm::createMSP430RTLInternalizationPass() {
  return new MSP430RTLInternalizer();
}
