.global outbyte
.equ SYSCALL_NUM_PUTSTRING, 2
	
.section .text
.even

outbyte:
      link %a6, #-8
	    movem.l %d1-%d3, -(%sp)  /*レジスタ退避 */
	   move.l  12(%a6), %d0   /*   引数全体をD0へ */
    move.b  %d0, -8(%a6)     /*下位1バイトをバッファ(-8(%a6))へ書き込む */

outbyte_loop:
                move.l #SYSCALL_NUM_PUTSTRING, %D0
                move.l  8(%a6), %d1 
		lea -8(%a6), %a0
		move.l %a0, %D2 | p = #BUF
		/*move.l %sp, %d2  sp + 4*4+3 */
		/*addi.l #19, %d2*/
		move.l #1, %D3 | size = #1
		trap #0

		cmpi.l #0,%d0
		beq outbyte_loop
               
		movem.l (%SP)+,%D1-%D3 /* レジスタ復帰 */
                    unlk   %a6
		rts
