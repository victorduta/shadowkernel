



#include "pad_pass.h"
#include "X86Subtarget.h"
#include "X86InstrInfo.h"


using namespace llvm;
using namespace std;


namespace llvm 
{

  class PadCallInstructionImpl : public DynamicMachinePass {
public:
    PadCallInstructionImpl() : DynamicMachinePass()
                   , STI(nullptr), TII(nullptr) {}

    bool runOnMachineFunction(MachineFunction &MF) override;

    MachineFunctionProperties getRequiredProperties() const override {
      return MachineFunctionProperties().set(
          MachineFunctionProperties::Property::NoVRegs);
    }

    StringRef getPassName() const override {
      return "X86 pad call instructions till size(call)+nops == 8 ";
    }
    FunctionPass* createX86DynamicMachinePass() override; //Override virtual method from parent class

    const X86Subtarget *STI;
   	const TargetInstrInfo *TII;

private:

    void getCallInstructionList(MachineFunction &MF, vector<MachineInstr *>& instructions);
    void printCallInstructionList(MachineFunction &MF, vector<MachineInstr *> &instructions);
    unsigned int getInstructionSize(MachineFunction &MF, MachineInstr *);

    void padCallInstruction(MachineBasicBlock::instr_iterator it);
    bool needsRexPrefixRegister(unsigned int register);
    unsigned int getOperandSize(int64_t operand);

    /* TODO eliminate these functions as they mostly do nothing so
     *  don't use stack for them.
     */
    void addPadding(MachineBasicBlock *MBB, MachineBasicBlock::instr_iterator &MBBI, unsigned int NOOPsToAdd);
    void addPadding(MachineBasicBlock *MBB,	DebugLoc DL, unsigned int NOOPsToAdd);
  };
}


/*
 * addPadding - add pading after call instruction
 */
void PadCallInstructionImpl::addPadding(MachineBasicBlock *MBB,
		                      MachineBasicBlock::instr_iterator &MBBI,
                              unsigned int NOOPsToAdd)
{
  DebugLoc DL = MBBI->getDebugLoc();
  while (NOOPsToAdd-- > 0) {
    BuildMI(*MBB, MBBI, DL, TII->get(X86::NOOP));
  }
}

/*
 *  addPadding - Add padding just after the basic block
 */

void PadCallInstructionImpl::addPadding(MachineBasicBlock *MBB,
							  DebugLoc DL,
                              unsigned int NOOPsToAdd)
{
    while (NOOPsToAdd-- > 0) {
     BuildMI(MBB, DL, TII->get(X86::NOOP));
    }
}

void PadCallInstructionImpl::padCallInstruction(MachineBasicBlock::instr_iterator it)
{
	unsigned int size = 0;
	MachineBasicBlock::instr_iterator next = it;
	MachineBasicBlock* parent= it->getParent();

	size = getInstructionSize(*(parent->getParent()), &(*(it)));

	printf("Size of the call is %d\n", size);

	size = PADDED_CALL_SIZE - size;
	++next;
	if (next != parent->end())
	{
	   /* Pad after the  call instruction  */
	   addPadding(parent, next, size);
	}
	else
	{
		/* Pad after the basic block when call is the last instruction in the block */
	   addPadding(parent, it->getDebugLoc(), size);

	}
}

unsigned int PadCallInstructionImpl::getOperandSize(int64_t operand)
{
	if ((-0x80 <= operand) && (0x80 > operand))
	{
		return 1;
	}
	/* Else sign extend to 32 bits */
	return 4;
}

bool PadCallInstructionImpl::needsRexPrefixRegister(unsigned int regval)
{
	switch(regval)
	{
	   case X86::R8:
	   case X86::R9:
	   case X86::R10:
	   case X86::R11:
	   case X86::R12:
	   case X86::R13:
	   case X86::R14:
	   case X86::R15:
		   return true;
	}
	return false;
}

/*  In 64bit mode we only have 3 types of calls
 * - CALL64pcrel32: Call near, relative, displacement represented as 32 bit operand
 * - CALL64r:  Call near, absolute indirect, address given in register
 * - CALL64m:  Call near, absolute indirect, address given in memory + register
 */
unsigned int PadCallInstructionImpl::getInstructionSize(MachineFunction &MF, MachineInstr *MI)
{
	unsigned int size = 1; // All calls have an opcode of 1 byte
    unsigned int OpCode = MI->getOpcode();
	MI->print(errs());
	errs() << "\n\n\n";


	if(OpCode == X86::CALL64pcrel32)
	{
		/* We are in 65 bit mode so the displacement will be extended to 32 bits */
		return 5;
	}

	if(OpCode == X86::CALL64r)
	{
	    /* In this case we have:
	     * - 1 byte instruction code
	     * - 1 byte R/M mod
	     * - an optional 1 byte in case we use an extended register
	     */
		MachineOperand reg = MI->getOperand(0);
		if(reg.isReg())
		{
			unsigned int regval = reg.getReg();
			if (regval == 0)
			{
				printf("getInstructionSize: CALL64r has the %noreg as first element");
				return MIN_CALL_SIZE;
			}

			size++; // We have R/M byte in this case

			/* Some registers require a REX prefix byte
			 * to be adressed in a call instruction
			 */
			if(needsRexPrefixRegister(regval))
			{
				size++;
			}
			return size;
		}
		else
		{
			/* Just log error here so we further check this issue */
			printf("getInstructionSize: CALL64r doesn't have register as first element\n");
			return MIN_CALL_SIZE;
		}
	}

	if(OpCode != X86::CALL64m)
	{
		/*
		 *  We don't support this case yet, but in any case
		 *  the call is at least 2 bytes in size
		 */
		printf("getInstructionSize: Unknown type of call\n");
		return MIN_CALL_SIZE;
	}

	/* We treat the CALL64m call instruction */
	int numOperands = 0;
	for (const MachineOperand &MO : MI->operands())
	{
		if(MO.isRegMask())
		{
			break;
		}
		numOperands++;
	}
	if (numOperands < CALL64_M_USED_OPERANDS)
	{
		printf("getInstructionSize: CALL64m doesn't have all operands set\n");
		return MIN_CALL_SIZE;
	}

	/* Some checks here, for testing purposes only */
	if(!MI->getOperand(0).isReg() || !MI->getOperand(2).isReg() ||
			   !MI->getOperand(4).isReg())
	{
		printf("getInstructionSize: CALL64m doesn't have operands 0,2,4 set to registers\n");
		return MIN_CALL_SIZE;
	}
	if(!MI->getOperand(1).isImm())
	{
		printf("getInstructionSize: CALL64m doesn't have operand 1 set to imm\n");
		return MIN_CALL_SIZE;
	}
    unsigned int reg1 = MI->getOperand(0).getReg();
    unsigned int reg2 = MI->getOperand(2).getReg();
    unsigned int reg3 = MI->getOperand(4).getReg();
    int64_t imm = MI->getOperand(1).getImm();

    if (reg1 == 0 && reg2 == 0)
    {
    	/* Memory adressing and while in 64 bit mode the instruction
    	 * size will be opcode+R/m+  sibil + adress_32 = 7
    	 */

    	return 7;
    }
    /* We always have R/M byte in CALL64m */
    size++;
    if (reg2 != 0)
    {
    	/* We have a sibil byte in this case */
    	size++;
    }

    if(reg2 == 0)
    {
    	if(reg1 == X86::R12)
    	{
    		/* Another special case, all others default to
    		 * CALL64_r but when this happens on R12
    		 * it seems that a R/M extension is used
    		 */
    		size++;
    	}
    }


    if(needsRexPrefixRegister(reg1) || needsRexPrefixRegister(reg2))
    {
    	/* Also add REX prefix in this case */
    	size++;
    }

    unsigned int operand_size = 0;
    if(!MI->getOperand(3).isImm())
    {
    	/* In 64 bit all symbols are extended to 32 bits */
    	printf("getInstructionSize: Signal symbol\n");
    	operand_size = 4;
    }
    else if(reg1 == X86::RIP)
    {
    	/* RIP relative adressing has the offset extended to 32 bits*/
    	printf("getInstructionSize: Signal RIP addressing\n");
    	operand_size = 4;
    }
    else
    {
    	int64_t im = MI->getOperand(3).getImm();
    	/* TODO find out why this case happens */
    	if (im == 0)
    	{
    		/* We default to normal CALL64_r case here*/
    		if(reg2 == 0)
    		{
    			/*  callq *(%r13) is encoded
    			 *  callq *0x0(%r13) so add
    			 *  one byte if this happens
    			 */
    			if(reg1 == X86::R13)
    			{
    				return size+1;
    			}
    			return size;
    		}
    		else
    		{
    			return size;
    		}
    	}
    	/* Not sure why this case happens, must investigate further */
    	operand_size = getOperandSize(MI->getOperand(3).getImm());
    }
    size += operand_size;

    if (size >= PADDED_CALL_SIZE)
    {
    	printf("getInstructionSize: Signal operation trim\n");
    	size = PADDED_CALL_SIZE;
    }

    if(reg3 != 0)
    {
    	/* This shouldn't happen in 64 bit mode */
    	printf("getInstructionSize: Signal segment register\n");
    }

	return size;
}

void PadCallInstructionImpl::printCallInstructionList(MachineFunction &MF, vector<MachineInstr *>  &instructions)
{
	errs() << MF.getName() << "==================\n";
	for(vector<MachineInstr *>::const_iterator it = instructions.begin(),
		    		                    ie = instructions.end(); it != ie; ++it)
	{
		 (*(it))->print(errs());
		 errs() << "\n";
		 errs() << "\n";
	}
	errs() << MF.getName() << "==================\n";
}

void PadCallInstructionImpl::getCallInstructionList(MachineFunction &MF, vector<MachineInstr *>& instructions)
  {
	  for(MachineFunction::iterator start_block = MF.begin(), end_block = MF.end(); start_block != end_block;
	  			  ++start_block)
	  	  {

	  		  for(MachineBasicBlock::instr_iterator start_instr = start_block->instr_begin(), end_instr = start_block->instr_end();
	  				  start_instr != end_instr; ++start_instr)
	  		  {
	  			   if (start_instr->isCall())
	  			   {
	  				 MachineInstr *call =  &(*start_instr) ;
	  				 instructions.push_back(call);
	  				 padCallInstruction(start_instr);
	  			   }

	  		  }
	  	  }

  }

bool PadCallInstructionImpl::runOnMachineFunction(MachineFunction &MF) {
      unsigned int size;
	  STI = &MF.getSubtarget<X86Subtarget>();
	  TII = STI->getInstrInfo();

      if(!STI->is64Bit())
      {
    	  printf("Error:Only instrument in 64bit mode %s\n", MF.getName());
    	  return false;
      }

	  vector<MachineInstr *> call_vector;
	  getCallInstructionList(MF, call_vector);
	  //printCallInstructionList(MF, call_vector);

	  //errs() << MF.getName() << " ==================\n";
	  //MF.print(errs());
	  //errs() << MF.getName() << " ==================\n";

	  return false;
  }

FunctionPass* PadCallInstructionImpl::createX86DynamicMachinePass() {
    return this;
  }


extern "C" DynamicMachinePass* create() {
    return new PadCallInstructionImpl();
 }

extern "C" void destroy(DynamicMachinePass* p) {
   // delete p;
 }
