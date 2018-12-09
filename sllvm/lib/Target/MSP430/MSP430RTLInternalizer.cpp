#include "MSP430.h"
#include "MSP430InstrInfo.h"
#include "MSP430Subtarget.h"
#include "llvm/ADT/Statistic.h"
#include "llvm/CodeGen/MachineFunctionPass.h"
#include "llvm/CodeGen/MachineInstrBuilder.h"
#include "llvm/Support/MathExtras.h"
#include "llvm/Target/TargetMachine.h"

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

bool MSP430RTLInternalizer::runOnMachineFunction(MachineFunction &mf) {
  MF = &mf;
  TII = static_cast<const MSP430InstrInfo *>(MF->getSubtarget().getInstrInfo());

#if 0
  // If the pass is disabled, just bail early.
  if (!BranchSelectEnabled)
    return false;

  LLVM_DEBUG(dbgs() << "\n********** " << getPassName() << " **********\n");
#endif

  bool MadeChange = false;

  return MadeChange;
}

/// Returns an instance of the RTL Internalization Pass
FunctionPass *llvm::createMSP430RTLInternalizationPass() {
  return new MSP430RTLInternalizer();
}
