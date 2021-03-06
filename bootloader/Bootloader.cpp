#include "Bootloader.h"
#include <iomanip>
#include "../backend/ConsoleLog.h"
extern uint64_t estrai_segmento(char *fname, void *dest, uint64_t dest_size);

/* CR0 bits */
#define CR0_PE 1
#define CR0_MP (1 << 1)
#define CR0_EM (1 << 2)
#define CR0_TS (1 << 3)
#define CR0_ET (1 << 4)
#define CR0_NE (1 << 5)
#define CR0_WP (1 << 16)
#define CR0_AM (1 << 18)
#define CR0_NW (1 << 29)
#define CR0_CD (1 << 30)
#define CR0_PG (1 << 31)

/* CR4 bits */
#define CR4_VME 1
#define CR4_PVI (1 << 1)
#define CR4_TSD (1 << 2)
#define CR4_DE (1 << 3)
#define CR4_PSE (1 << 4)
#define CR4_PAE (1 << 5)
#define CR4_MCE (1 << 6)
#define CR4_PGE (1 << 7)
#define CR4_PCE (1 << 8)
#define CR4_OSFXSR (1 << 8)
#define CR4_OSXMMEXCPT (1 << 10)
#define CR4_UMIP (1 << 11)
#define CR4_VMXE (1 << 13)
#define CR4_SMXE (1 << 14)
#define CR4_FSGSBASE (1 << 16)
#define CR4_PCIDE (1 << 17)
#define CR4_OSXSAVE (1 << 18)
#define CR4_SMEP (1 << 20)
#define CR4_SMAP (1 << 21)

#define EFER_SCE 1
#define EFER_LME (1 << 8)
#define EFER_LMA (1 << 10)
#define EFER_NXE (1 << 11)

/* 32-bit page directory entry bits */
#define PDE32_PRESENT 1
#define PDE32_RW (1 << 1)
#define PDE32_USER (1 << 2)
#define PDE32_PS (1 << 7)

/* 64-bit page * entry bits */
#define PDE64_PRESENT 1
#define PDE64_RW (1 << 1)
#define PDE64_USER (1 << 2)
#define PDE64_ACCESSED (1 << 5)
#define PDE64_DIRTY (1 << 6)
#define PDE64_PS (1 << 7)
#define PDE64_G (1 << 8)

// global logger
extern ConsoleLog& logg;

extern uint8_t bootloader_code[];

Bootloader::Bootloader(int vcpu_fd, uint8_t *guest_mem, uint64_t guest_mem_size, uint64_t entry_point, uint64_t start_stack)
{
	vcpu_fd_ = vcpu_fd;
	guest_mem_ = guest_mem;
	entry_point_ = entry_point;
	start_stack_ = start_stack;
	guest_mem_size_ = guest_mem_size;
}	

void Bootloader::fill_segment_descriptor(uint64_t *dt, struct kvm_segment *seg)
{
	uint16_t index = seg->selector >> 3;
	uint32_t limit = seg->g ? seg->limit >> 12 : seg->limit;
	dt[index] = (limit & 0xffff) /* Limit bits 0:15 */
		| (seg->base & 0xffffff) << 16 /* Base bits 0:23 */
		| (uint64_t)seg->type << 40
		| (uint64_t)seg->s << 44 /* system or code/data */
		| (uint64_t)seg->dpl << 45 /* Privilege level */
		| (uint64_t)seg->present << 47
		| ((limit & 0xf0000ULL) >> 16) << 48 /* Limit bits 16:19 */
		| (uint64_t)seg->avl << 52 /* Available for system software */
		| (uint64_t)seg->l << 53 /* 64-bit code segment */
		| (uint64_t)seg->db << 54 /* 16/32-bit segment */
		| (uint64_t)seg->g << 55 /* 4KB granularity */
		| (seg->base & 0xff000000ULL) << 56; /* Base bits 24:31 */

	/* logg << "filling gdt[" << std::dec << index << "]: " << std::hex << std::setfill('0') << std::setw(2) <<(unsigned long)dt[index]
	<< " selector " << seg->selector << "\n";*/
}

void Bootloader::setup_protected_mode(kvm_sregs *sregs)
{
	kvm_segment seg;
	memset(&seg, 0, sizeof(seg));
	seg.base = 0;
	seg.limit = 0xffffffff;
	seg.present = 1;
	seg.dpl = 0;
	seg.db = 1;
	seg.s = 1; /* Code/data */
	seg.l = 0; /* not a 64 bit segment */
	seg.g = 1; /* 4KB granularity */

	uint64_t *gdt;

	sregs->cr0 = CR0_ET | CR0_PE;
	sregs->gdt.base = 0x10000;
	sregs->gdt.limit = 3 * 8 - 1;

	gdt = (uint64_t *)(guest_mem_ + sregs->gdt.base);
	/* gdt[0] is the null segment */

	seg.type = 10; /* Code: execute, read */
	seg.selector = 1 << 3;
	fill_segment_descriptor(gdt, &seg);
	sregs->cs = seg;

	seg.type = 3; /* Data: read/write, accessed */
	seg.selector = 2 << 3;
	seg.limit = 0xffffffff;
	fill_segment_descriptor(gdt, &seg);
	sregs->ds = sregs->es = sregs->fs = sregs->gs = sregs->ss = seg;
}

void Bootloader::setup_page_tables(kvm_sregs *sregs)
{
	uint64_t pml4_addr = 0x20000;
	uint64_t *pml4 = reinterpret_cast<uint64_t *>(reinterpret_cast<uint64_t>(guest_mem_) + pml4_addr);

	uint64_t pdpt_addr = 0x30000;
	uint64_t *pdpt = reinterpret_cast<uint64_t *>(reinterpret_cast<uint64_t>(guest_mem_) + pdpt_addr);

	uint64_t pd_addr = 0x40000;
	uint64_t *pd = reinterpret_cast<uint64_t *>(reinterpret_cast<uint64_t>(guest_mem_) + pd_addr);

	// every entry maps 1GiB. 
	uint32_t last_index_lev3 = guest_mem_size_ / (1 << 30);
	// # entry on the last lev2 table
	uint32_t n_entry_lev2 = (guest_mem_size_ % (1 << 30)) / (1 << 21); 

	uint64_t virt_addr;

	//level 4 table
	pml4[0] = PDE64_PRESENT | PDE64_RW | PDE64_USER | (pdpt_addr);
	

	for( uint32_t i_lev3=0; i_lev3<=last_index_lev3; i_lev3++)
	{
		pdpt[i_lev3] = PDE64_PRESENT | PDE64_RW | PDE64_USER | (pd_addr);
		for(uint32_t i_lev2=0; i_lev2<512; i_lev2++)
		{
			if(i_lev3 != (last_index_lev3)  // the last lev2 table must map only n_entry_liv2 others must map every entry
				 || i_lev2<n_entry_lev2 )
			{
				virt_addr = ((uint64_t)i_lev3 << 30) + (((uint64_t)i_lev2) << 21);
				// we set PS bit to address 2MiB pages
				pd[i_lev2] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | virt_addr;
			} else {
				// the page is not present
				pd[i_lev2] = pd[i_lev2] & (~PDE64_PRESENT);
			}
		//	logg << "pd[" << std::dec << i_lev2 << "]=" << std::hex << pd[i_lev2] << std::endl;
		}
		pd_addr += 0x10000;
		pd = reinterpret_cast<uint64_t *>(reinterpret_cast<uint64_t>(guest_mem_) + pd_addr);
	}

	// init liv3 for APIC
	// TODO: 0xfec00000 must be user settable and not fixed
	pdpt[3] = PDE64_PRESENT | PDE64_RW | PDE64_USER | (pd_addr);
	pd[502] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | 0xfec00000;

	pdpt[3] = PDE64_PRESENT | PDE64_RW | PDE64_USER | (pd_addr);
	pd[503] = PDE64_PRESENT | PDE64_RW | PDE64_USER | PDE64_PS | 0xfee00000;

	sregs->cr3 = pml4_addr;

	/*sregs->cr3 = pml4_addr;
	sregs->cr4 = CR4_PAE;
	sregs->cr0 = CR0_PE | CR0_ET;
	sregs->efer = EFER_LME;

	// We don't set cr0.pg here, because that causes a vm entry
	// failure. It's not clear why. Instead, we set it in the VM
	// code.
	setup_64bit_code_segment(guest_mem_, sregs);*/
}

int Bootloader::run_protected_mode()
{
	struct kvm_sregs sregs;
	struct kvm_regs regs;

	printf("Testing protected mode\n");

    if (ioctl(vcpu_fd_, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	setup_protected_mode(&sregs);

    if (ioctl(vcpu_fd_, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	memset(&regs, 0, sizeof(regs));
	// Clear all FLAGS bits, except bit 1 which is always set.
	regs.rflags = 2;
	regs.rip = entry_point_;
	regs.rsp = start_stack_;
	logg << "rip=" << std::hex <<(unsigned long) regs.rip << " rsp=" << (unsigned long)regs.rsp << std::endl;



	if (ioctl(vcpu_fd_, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}

	return 0;
}


int Bootloader::run_long_mode()
{
	/* ====== QEMU RISULTA AVERE QUESTI VALORI NEI REGISTRI ======
	EAX=2badb002 EBX=00009500 ECX=00100100 EDX=00000511
	ESI=00000000 EDI=0010a000 EBP=00000000 ESP=00006f00
	EIP=00100101 EFL=00000006 [-----P-] CPL=0 II=0 A20=1 SMM=0 HLT=1
	ES =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
	CS =0008 00000000 ffffffff 00cf9a00 DPL=0 CS32 [-R-]
	SS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
	DS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
	FS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
	GS =0010 00000000 ffffffff 00cf9300 DPL=0 DS   [-WA]
	LDT=0000 00000000 0000ffff 00008200 DPL=0 LDT
	TR =0000 00000000 0000ffff 00008b00 DPL=0 TSS32-busy
	GDT=     000caa68 00000027
	IDT=     00000000 000003ff
	CR0=00000011 CR2=00000000 CR3=00000000 CR4=00000000
	DR0=0000000000000000 DR1=0000000000000000 DR2=0000000000000000 DR3=0000000000000000 
	DR6=00000000ffff0ff0 DR7=0000000000000400
	EFER=0000000000000000
	*/

	struct kvm_sregs sregs;
	struct kvm_regs regs;

	printf("Testing 64-bit mode\n");

    if (ioctl(vcpu_fd_, KVM_GET_SREGS, &sregs) < 0) {
		perror("KVM_GET_SREGS");
		exit(1);
	}

	// switch to protected mode
	setup_protected_mode(&sregs);

	// create 1GiB memory window
	setup_page_tables(&sregs);

	// send the segments registers changes to KVM
	if (ioctl(vcpu_fd_, KVM_SET_SREGS, &sregs) < 0) {
		perror("KVM_SET_SREGS");
		exit(1);
	}

	// registers initialization
	memset(&regs, 0, sizeof(regs));
	regs.rflags = 2;

	// we load bootloader code starting to address 0
	char bootloader_elf[] = "build/boot64";
	// bootloader entry point
	regs.rip = estrai_segmento(bootloader_elf, guest_mem_, 0x10000);

	// target program entry point is in %rdi
	regs.rdi = entry_point_;
	// va sistemato, E' COMPETENZA DEL BOOTLOADER
	//regs.rsp = _start_stack__;

	if (ioctl(vcpu_fd_, KVM_SET_REGS, &regs) < 0) {
		perror("KVM_SET_REGS");
		exit(1);
	}

	return 0;
}

