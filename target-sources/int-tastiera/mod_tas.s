#include "libce.s"
#****************************************************************************************************
# file mod_tas.s
.text
.extern       c_leggi_linea
.global       a_leggi_linea                # dichiarazione necessaria per la funzione ini()
a_leggi_linea:                             # routine INT $230
              call      c_leggi_linea
              iretq
.extern       c_driv_tasint
.global       a_driv_tasint                # dichiarazione necessaria per la funzione ini()
a_driv_tasint:                             # routine associata al tipo 30
	      salva_registri
              call      c_driv_tasint
	      movabsq  $0xFEE000B0, %rax
              movl      $0, (%rax)               # invio di End Of Interrupt
	      carica_registri
              iretq
#***************************************************************************************************
