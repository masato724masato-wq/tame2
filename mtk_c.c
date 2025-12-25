#define DEFINE_GLOBAL_VARS
#include <mtk_c.h>
extern void end_task_entry(void);
/* mtk_c.h */


/*
 * void p_body(SEMAPHORE_ID_TYPE sem_id);
 * 概要: P命令 (資源獲得) の処理を行う
 * 引数: sem_id (操作対象のセマフォID)
 */
void p_body(SEMAPHORE_ID_TYPE sem_id) {
    /* 安全のため ID の範囲チェックを行う */
    if (sem_id < 0 || sem_id >= NUMSEMAPHORE) {
        return;
    }

    /* 1. セマフォの値を減らす */
    semaphore[sem_id].count--;

    /* 2. セマフォが獲得できなければ (負の値なら) sleep する */
    /* count < 0 ということは、資源数を超えて要求が来たことを意味する */
    if (semaphore[sem_id].count < 0) {
        /* 現在のタスクをこのセマフォの待ち行列に入れて、タスクスイッチする */
        sleep(sem_id);
    }
    
    /* count >= 0 なら、資源を獲得できたのでそのままリターンする */
    /* (呼び出し元の pv_handler -> rte -> ユーザタスク へ戻る) */
}

void v_body(SEMAPHORE_ID_TYPE sem_id)
{
    semaphore[sem_id].count++;

    if (semaphore[sem_id].count <= 0) {
        // セマフォを待っているタスクがいる → 起床させる
        wakeup(sem_id);
    }
}

void sleep(SEMAPHORE_ID_TYPE sem_id) {
    /* 1. 現在のタスクをセマフォの待ち行列につなぐ */
    /* (curr_task を semaphore[sem_id].task_list の末尾に追加) */
    addq(&semaphore[sem_id].task_list, curr_task);
    
    /* タスクの状態を「待ち」にする */
    task_tab[curr_task].status = WAITING;

    /* 2. スケジューラを起動して、次に実行するタスク(next_task)を決める */
    sched();

    /* 3. タスクスイッチ関数を呼び出し、タスクを切り替える */
    /* ここで実行が中断し、CPUは別のタスクへ移る */
    swtch();
}

void wakeup(int sem_id)
{
  int ID=removeq(&semaphore[sem_id].task_list);
  if(!ID) return;
    task_tab[ID].status=READY;
    addq(&ready,ID);
/*semaphore[sem_id].task_listはsleepされたタスクが保存されているキューだという認識で書いてる*/
}

//*************************************************************************
//**addq
//*************************************************************************
void addq(TASK_ID_TYPE *queue, TASK_ID_TYPE curr_task){
  TASK_ID_TYPE last_id; /* キュー末尾 */

  /* ★★★ 追加: キューが空の場合の処理 ★★★ */
    if (*queue == NULLTASKID) {
        *queue = curr_task;    /* 先頭 (ready) をこのタスクに書き換える */
        return;         /* これで完了 */
    }
    
  last_id=*queue;

//**TCBのnextが0以外のときループ
  while(task_tab[last_id].next!=0){
    last_id=task_tab[last_id].next;
  }
//**末尾を新しいタスクに
  task_tab[last_id].next=curr_task;
//**新しいタスクを新たな末尾に
  task_tab[curr_task].next=0;
}

//*************************************************************************
//**removeq
//*************************************************************************
//**qhead_id=>キューのヘッドID
int removeq(TASK_ID_TYPE *queue){
  TASK_ID_TYPE current_id; 

//**キュー先頭のタスクIDを取得
  current_id=*queue;

//**キューが空のとき
  if(current_id==0){
    return 0;
  }

//**先頭タスクの取り出し
  *queue = task_tab[current_id].next;

  task_tab[current_id].next=NULLTASKID;
  return current_id;
}


//*************************************************************************
//**sched
//*************************************************************************
void sched(void)
{
    next_task = removeq(&ready); /* Readyキュー (ready) の先頭からタスクIDを取り出す */

    if(next_task == NULLTASKID) {
        while(1){
        }
    }
}

void set_task(void (*task_func)()){
  for(int i=0;i<NUMTASK;i++){
    if (task_tab[i+1].status==UNDEFINED){
      new_task=i+1;
      task_tab[i+1].status=READY;
      task_tab[i+1].task_addr=task_func;//引数を入れる
      break;
    }
  }
  task_tab[new_task].stack_ptr=init_stack(new_task);
  //readyきゅーにnew_task問録
  addq(&ready, new_task);
}


void *init_stack(TASK_ID_TYPE id){
  int *ssp = (int *)(stacks[id-1].sstack + STKSIZE);
  *(--ssp) = (int)task_tab[id].task_addr;
  unsigned short *ssp_short = (unsigned short *)ssp;
  *(--ssp_short) = 0x0000;  
  ssp = (int *)ssp_short;

  for (int i = 0; i < 15; i++){
    *(--ssp) = 0;
  }
  
  *(--ssp)=(int)(stacks[id - 1].ustack + STKSIZE);
  return (void *)ssp;
}


/*void *init_stack(TASK_ID_TYPE id){
  int *ssp = (int *)(stacks[id-1].sstack + STKSIZE);
  *(--ssp) = (int)task_tab[id].task_addr;
  unsigned short *ssp_short = (unsigned short *)ssp;
  *(--ssp_short) = 0x0000;  
  ssp = (int *)ssp_short;
  
  

  for (int i = 0; i < 15; i++){
    *(--ssp) = 0;
  }
  
  int *usp = (int *)(stacks[id-1].ustack + STKSIZE);
  *(--usp) = (int)end_task_entry;
  
 
   *(--ssp) = (int)usp;
  return (void *)ssp;
}*/

/* mtk_c.c */


void init_kernel(){
  for (int i=0; i < NUMTASK;i++){
    task_tab[i+1].status=UNDEFINED;
    task_tab[i+1].next=NULLTASKID;
    task_tab[i+1].priority=UNDEFINED;
    for(int j=0; j<STKSIZE; j++){
        stacks[i].ustack[j] = 0;
        stacks[i].sstack[j] = 0;
    }
  }
  ready=NULLTASKID;
  for (int i=0; i < NUMSEMAPHORE;i++){
    semaphore[i].task_list=NULLTASKID;
    semaphore[i].count=1;
    semaphore[i].nst = 0;

  }
  *(void **)0x00000084 = (void *)pv_handler;
}

void begin_sch(){
  curr_task = removeq(&ready);
  
  if (curr_task == NULLTASKID){
    while(1);
  }
  init_timer();
  first_task();
}



void waitp_body(int sem_id) {
    SEMAPHORE_TYPE *sp = &semaphore[sem_id];

    // 今のカウント値が -(N-1) より大きい（まだ0や-1）なら、自分は「最後の人」ではない
    // つまり、待ち合わせ人数に達していない場合
    if (sp->count > -(sp->nst - 1)) {
        // [自分は待つ側]
        // p_body() を呼んで、カウントを1減らし(count--), スリープする
        p_body(sem_id); 
    }
    else {
        // [自分は最後の人] (count == -(N-1) の状態で来た)
        // 全員揃ったので、寝ている仲間を起こす

        // 寝ている人数分 (N-1回)、v_body() を呼んで起こしてあげる
        int i;
        for (i = 0; i < sp->nst - 1; i++) {
            v_body(sem_id); 
            // v_body内で count++ され、wakeup() される
        }
        
        // 自分自身は寝ずにそのまま関数を抜けて実行継続する
        // (注: v_bodyをN-1回呼ぶことで count は 0 に戻り、次の待ち合わせに再利用できる)
    }
}
void end_task(){
    
}
