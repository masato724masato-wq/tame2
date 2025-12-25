.include "equdefs.inc"
.global end_task_entry
.global P
.global V
.global waitP
.global pv_handler
.global swtch	
.global hard_clock
.global init_timer
.global first_task	

.extern p_body
.extern v_body
.extern addq
.extern sched
.extern ready
.extern curr_task
	
.text
.even

/* =========================================================
 * pv_handler
 * 概要: TRAP #1 割り込み処理ルーチン (P/V命令の入り口)
 * 入力: %d0 = システムコール番号 (0:P, 1:V, 2:waitP)
 * %d1 = セマフォID
 * ========================================================= */
pv_handler:
	/* 1. 実行中のタスクのレジスタ退避 */
	/* 全レジスタをスーパーバイザスタックに退避 */
	movem.l %d0-%d7/%a0-%a6, -(%sp)

	/* 2. 割り込み禁止 (カーネル再入防止) */
	/* 現在のSRを保存してから、レベル7 (0x2700) に設定 */
	move.w  %sr, -(%sp)
	move.w  #0x2700, %sr

	/* 3. C関数の呼び出し準備 */
	/* p_body(sem_id) 等の引数として、%d1 (sem_id) をスタックに積む */
	move.l  %d1, -(%sp)

	/* 4. システムコール番号 (%d0) による分岐 */
	cmpi.l  #SYSCALL_NUM_P, %d0
	beq     CALL_P_BODY
    
	cmpi.l  #SYSCALL_NUM_V, %d0
	beq     CALL_V_BODY
	
	cmpi.l #SYSCALL_NUM_WAITP, %d0
	beq     CALL_WAITP
	
	cmpi.l #SYSCALL_ENDTASK, %d0
	beq     CALL_END_TASK
    
	/* 該当なしなら何もしないで終了へ (またはエラー処理) */
	bra     PV_EXIT

CALL_P_BODY:
	jsr     p_body          /* p_body(sem_id) */
	bra     PV_EXIT

CALL_V_BODY:
	jsr     v_body          /* v_body(sem_id) */
	bra     PV_EXIT
CALL_WAITP:
        jsr     waitp_body
        bra     PV_EXIT
CALL_END_TASK:
        jsr     end_task_entry
        bra     PV_EXIT
PV_EXIT:
	/* 5. 引数の後始末 */
	/* スタックに積んだ sem_id (4バイト) を破棄 */
	addq.l  #4, %sp

	/* 6. 割り込み禁止の解除 */
	/* 保存していた SR を復帰 */
	move.w  (%sp)+, %sr

	/* 7. レジスタの復帰 */
	movem.l (%sp)+, %d0-%d7/%a0-%a6

	/* 8. 割り込み処理を終了 (例外復帰) */
	rte

/*
 * void P(int sem_id);
 * 概要: P命令のシステムコールを発行する
 * 引数: sem_id -> スタックの 4(%sp) にある (jsrの戻り番地の奥)
 */
P:
	/* 1. 引数 sem_id を取得して %d1 にセット */
	/* スタック構造: [戻り番地(4B)] [sem_id(4B)] ... */
	/* よって、現在の %sp から +4 の位置にある */
	move.l  4(%sp), %d1

	/* 2. システムコール番号を %d0 にセット */
	move.l  #SYSCALL_NUM_P, %d0

	/* 3. OS (pv_handler) を呼び出す */
	trap    #1

	/* 4. 戻ってきたらそのままリターン */
	rts

	/* V(int sem_id) -- C から呼ばれる入口 (D0 := 1, D1 := sem_id) */
V:
        move.l  4(%sp), %d1        | 引数 sem_id を D1 に
        moveq   #1, %d0           | システムコール番号 1 = V (あなたの仕様)
        trap    #1
        rts

waitP:
	move.l  4(%sp), %d1     /* 引数 sem_id を %d1 へ */
	move.l  #SYSCALL_NUM_WAITP, %d0
	trap    #1
	rts


/***************************************/
.equ TCB_SIZE, 20
	
swtch:
**1. SR をスタックに積んで，RTE で復帰できるようにする．
        move.w %SR, -(%SP)

**2. 実行中のタスクのレジスタの退避
        movem.l %d0-%d7/%a0-%a6, -(%sp) /* レジスタ退避 */
        move.l %USP, %a1
	move.l %a1, -(%sp)

**3. SSP の保存
***このタスクのTCB の位置を求め
        move.l curr_task, %d0 /* currtaskを取得 */
        mulu.w #TCB_SIZE, %d0 /* d0=タスクid+サイズ */
        move.l #task_tab, %a0
        add.l %d0, %a0 /* tcb=task_tabの基底+d0 */
***SSP を正しい位置に記録する
        move.l %sp, 4(%a0)

**4.curr task を変更
        move.l next_task, curr_task

**5. 次のタスクのSSP の読み出し
***新たなcurr task の値を元にTCB の位置を割り出して
        move.l curr_task, %d0 
        mulu.w #TCB_SIZE, %d0 
        move.l #task_tab, %a0
        add.l %d0, %a0 

***その中に記録されているSSP の値を回復する
        move.l 4(%a0), %sp

** 6. 次のタスクのレジスタの読み出し
        move.l (%sp)+, %a1
	move.l %a1, %USP
        movem.l (%sp)+, %d0-%d7/%a0-%a6

        rte
/************************************************/
	

hard_clock:
	/* レジスタを使わないので退避なし */
	move.l curr_task, -(%sp) /* 引数2を積む */
	move.l #ready, -(%sp) /* 引数1を積む */
	jsr addq /* addq実行 */
	addq.l #8, %sp /* 2引数分戻す */
	jsr sched /* sched起動 */
	jsr swtch /* swrch起動 */
	rts /* 復帰 */

init_timer:
	move.l #SYSCALL_NUM_SET_TIMER, %d0 /* システムコール種別を設定 */
	move.l #10000, %d1 /* 周期を設定（10000=1秒） */
	move.l #hard_clock, %d2 /* 割り込み時タスクのアドレスを設定 */
	trap #0 /* システムコール */
	rts /* 復帰 */

/********************************************/
first_task:
        move.l %sp, SINGLE_SSP

	move.l  curr_task, %d0      | %d0 = ID

	mulu.w  #20, %d0            | %d0 = ID 

	lea     task_tab, %a0       
	add.l   %d0, %a0    
	move.l 4(%a0), %sp
	move.l  (%sp)+, %a0
	move.l   %a0, %usp
	movem.l (%sp)+, %d0-%d7/%a0-%a6
	rte
BACKTO_SINGLE:
       rts
 /******************************************/
 skipmt:
     move.l SYSCALL_NUM_SKIPMT,%D0
     trap #0
     rts/*復帰*/
 /******************************************/      
       
/******************************************/
end_task:
       move.w #0x2700, %sr
       jsr sched
       move.l next_task, %d0
       cmpi #0, %d0
       beq  end_ready
       /*------------------ */
    /* curr_task = next_task */
    move.l %d0, curr_task
    
    /* %d0 にタスクIDが入っている状態で、TCBのアドレス計算 */
    /* アドレス計算ロジックは first_task からコピーしてくる */
    /* 例: TCBサイズが24バイトなら mulu #24, %d0 など */
    
    /* task_tab のアドレス + オフセット を %a0 に入れる */
    mulu.w  #20, %d0            | %d0 = ID 

	lea     task_tab, %a0       
	add.l   %d0, %a0    
	move.l 4(%a0), %sp
	move.l  (%sp)+, %a0
	move.l   %a0, %usp
  
    /* コンテキスト(レジスタ)の復元 */
    movem.l (%sp)+, %d0-%d7/%a0-%a6
    
 
    rte
       
       
       
end_ready:
       move.l #SYSCALL_NUM_RESET_TIMER,%D0
       trap #0
       move.w #0x2700, %sr
       move.l SINGLE_SSP, %sp
       jmp BACKTO_SINGLE
/******************************************/
end_task_entry:
    move.l SYSCALL_ENDTASK, %d0
    trap #1
    rts

/**************************************************/
 .section .data
    .even           /* ★これが絶対に必要 */
SINGLE_SSP:
    .ds.l 1

