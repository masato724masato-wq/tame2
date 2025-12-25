.section .text
    .even
    .global inbyte
    .equ SYSCALL_NUM_GETSTRING, 1

inbyte:
    /* 1. レジスタの退避 */
    /* システムコールで使用する d1-d3 を保存 */
    movem.l %d1-%d3, -(%sp)

 
    subq.l #2, %sp

    
inbyte_loop:

    move.l #SYSCALL_NUM_GETSTRING, %d0
    move.l #0, %d1          /* チャネル 0 */
    move.l %sp, %a0         /* バッファアドレス = 現在のSP */
    move.l %a0, %d2
    
    move.l #1, %d3          /* サイズ 1バイト */
    trap #0                 /* モニタ呼び出し */

    /* ポーリング (入力待ち) */
    cmp.l #0, %d0           /* 戻り値が0なら入力なし */
    beq inbyte_loop         /* ループして待機 */

    /* 3. 戻り値の取得 */
    moveq #0, %d0           /* 戻り値用レジスタをゼロクリア */
    move.b (%sp), %d0       /* 確保したバッファ(SPの位置)から1文字読む */

    /* 4. 後始末 */
    addq.l #2, %sp          /* バッファ領域の解放 (SP += 2) */
    movem.l (%sp)+, %d1-%d3 /* レジスタの復帰 */
    
    rts

	
