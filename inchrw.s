.global inbyte
.equ SYSCALL_NUM_GETSTRING, 1 /* 即値の定義 */

.section .text
.even

inbyte:
  link %a6, #-8
	movem.l %D1-%D3,-(%SP) /* レジスタ退避 */

inbyte_loop:
	move.l #SYSCALL_NUM_GETSTRING, %D0 /* GETSTRINGの呼び出し */
	move.l  8(%a6), %d1 
	lea -4(%a6), %a0	
	move.l %a0, %D2 | p = #BUF
	move.l #1, %D3 | size = 256
	trap #0

	cmpi.l #0,%d0 /* 文字入力がされたか */
	beq inbyte_loop

	move.b -4(%a6), %d0 /* d0 : 戻り値 */
	
	movem.l (%SP)+,%D1-%D3 /* レジスタ復帰 */
	unlk %a6
	rts


.global inkey

inkey:
 
    link    %a6, #-8
    
    
    movem.l %d1-%d3, -(%sp)

   
    move.l  #SYSCALL_NUM_GETSTRING, %d0
    
 /* 引数 int ch を取得して D1 にセット */
    move.l  8(%a6), %d1    
    lea     -4(%a6), %a0    
    move.l  %a0, %d2       
    move.l  #1, %d3    
    
    /* 4. システムコール実行 */
    trap    #0

    
    cmpi.l #0,%d0 /* 文字入力がされたか */
	beq no_key
  

    /*入力があった場合 */
    moveq   #0, %d0         /* 上位ビットをクリア */
    move.b  -4(%a6), %d0    /* バッファから値を読み込み (0x00〜0xFF) */
    bra     inkey_exit      /* 終了処理へ */

no_key:
    /* --- 入力がなかった場合 (-1) --- */
    move.l  #-1, %d0        /* D0 = 0xFFFFFFFF (-1) */

inkey_exit:
    /* 6. 後片付け */
    movem.l (%sp)+, %d1-%d3
    unlk    %a6
    rts

