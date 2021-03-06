#ifndef GDBSERVER_H
#define GDBSERVER_H

enum amd64_regnum
{
  AMD64_RAX_REGNUM,		/* %rax */
  AMD64_RBX_REGNUM,		/* %rbx */
  AMD64_RCX_REGNUM,		/* %rcx */
  AMD64_RDX_REGNUM,		/* %rdx */
  AMD64_RSI_REGNUM,		/* %rsi */
  AMD64_RDI_REGNUM,		/* %rdi */
  AMD64_RBP_REGNUM,		/* %rbp */
  AMD64_RSP_REGNUM,		/* %rsp */
  AMD64_R8_REGNUM,		/* %r8 */
  AMD64_R9_REGNUM,		/* %r9 */
  AMD64_R10_REGNUM,		/* %r10 */
  AMD64_R11_REGNUM,		/* %r11 */
  AMD64_R12_REGNUM,		/* %r12 */
  AMD64_R13_REGNUM,		/* %r13 */
  AMD64_R14_REGNUM,		/* %r14 */
  AMD64_R15_REGNUM,		/* %r15 */
  AMD64_RIP_REGNUM,		/* %rip */
  AMD64_EFLAGS_REGNUM,		/* %eflags */
  AMD64_CS_REGNUM,		/* %cs */
  AMD64_SS_REGNUM,		/* %ss */
  AMD64_DS_REGNUM,		/* %ds */
  AMD64_ES_REGNUM,		/* %es */
  AMD64_FS_REGNUM,		/* %fs */
  AMD64_GS_REGNUM,		/* %gs */
  AMD64_ST0_REGNUM = 24,	/* %st0 */
  AMD64_ST1_REGNUM,		/* %st1 */
  AMD64_FCTRL_REGNUM = AMD64_ST0_REGNUM + 8,
  AMD64_FSTAT_REGNUM = AMD64_ST0_REGNUM + 9,
  AMD64_FTAG_REGNUM = AMD64_ST0_REGNUM + 10,
  AMD64_XMM0_REGNUM = 40,	/* %xmm0 */
  AMD64_XMM1_REGNUM,		/* %xmm1 */
  AMD64_MXCSR_REGNUM = AMD64_XMM0_REGNUM + 16,
  AMD64_YMM0H_REGNUM,		/* %ymm0h */
  AMD64_YMM15H_REGNUM = AMD64_YMM0H_REGNUM + 15,
  AMD64_BND0R_REGNUM = AMD64_YMM15H_REGNUM + 1,
  AMD64_BND3R_REGNUM = AMD64_BND0R_REGNUM + 3,
  AMD64_BNDCFGU_REGNUM,
  AMD64_BNDSTATUS_REGNUM,
  AMD64_XMM16_REGNUM,
  AMD64_XMM31_REGNUM = AMD64_XMM16_REGNUM + 15,
  AMD64_YMM16H_REGNUM,
  AMD64_YMM31H_REGNUM = AMD64_YMM16H_REGNUM + 15,
  AMD64_K0_REGNUM,
  AMD64_K7_REGNUM = AMD64_K0_REGNUM + 7,
  AMD64_ZMM0H_REGNUM,
  AMD64_ZMM31H_REGNUM = AMD64_ZMM0H_REGNUM + 31,
  AMD64_PKRU_REGNUM,
  AMD64_FSBASE_REGNUM,
  AMD64_GSBASE_REGNUM
};

enum amd64_extendend_regnum
{
  AMD64_CR4_REGNUM,
  AMD64_CR3_REGNUM,
  AMD64_CR2_REGNUM,
  AMD64_CR0_REGNUM,
  AMD64_EFER_REGNUM
};

void gdbserver_handle_exception(int sigval);
unsigned long gdbserver_get_register(amd64_regnum name);
void gdbserver_set_register(amd64_regnum name, unsigned long value);
void gdbserver_set_custom_register(amd64_extendend_regnum name, unsigned long value);
bool gdbserver_start(const char* ip_addr, unsigned short port);
void gdbserver_stop();

#endif