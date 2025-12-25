#ifndef MTK_C_H
#define MTK_C_H

/* =============================================
 * 定数定義
 * ============================================= */
#define NULLTASKID  0
#define NUMTASK     5
#define STKSIZE     16384
#define NUMSEMAPHORE 5

#define UNDEFINED   0
#define READY       1
#define RUNNING     2
#define WAITING     3
#define FINISHED    4

/* =============================================
 * 型定義
 * ============================================= */
typedef int TASK_ID_TYPE;
typedef int SEMAPHORE_ID_TYPE;

typedef struct {
    int count;
    int nst;
    TASK_ID_TYPE task_list;
} SEMAPHORE_TYPE;

typedef struct {
    void (*task_addr)();
    void *stack_ptr;
    int priority;
    int status;
    TASK_ID_TYPE next;
} TCB_TYPE;

typedef struct {
    char ustack[STKSIZE];
    char sstack[STKSIZE];
} STACK_TYPE;

/* =============================================
 * 変数宣言の切り替えロジック (ここが重要！)
 * ============================================= */
/* * DEFINE_GLOBAL_VARS が定義されている場合 (mtk_c.c から呼ばれた時)
 * -> GLOBAL マクロは空っぽになる (実体定義になる)
 * * 定義されていない場合 (test2.c などから呼ばれた時)
 * -> GLOBAL マクロは 'extern' になる (外部宣言になる)
 */
#ifdef DEFINE_GLOBAL_VARS
  #define GLOBAL
#else
  #define GLOBAL extern
#endif

/* =============================================
 * 大域変数 (GLOBAL マクロを使用)
 * ============================================= */
GLOBAL TASK_ID_TYPE curr_task;
GLOBAL TASK_ID_TYPE new_task;
GLOBAL TASK_ID_TYPE next_task;
GLOBAL TASK_ID_TYPE ready;

GLOBAL SEMAPHORE_TYPE semaphore[NUMSEMAPHORE];
GLOBAL TCB_TYPE task_tab[NUMTASK + 1];
GLOBAL STACK_TYPE stacks[NUMTASK];

/* =============================================
 * 関数プロトタイプ宣言 (常に extern でOK)
 * ============================================= */
extern void init_kernel(void);
extern void set_task(void (*task_func)());
extern void *init_stack(TASK_ID_TYPE id);
extern void begin_sch(void);
extern void addq(TASK_ID_TYPE *queue, TASK_ID_TYPE id);
extern TASK_ID_TYPE removeq(TASK_ID_TYPE *queue);
extern void sched(void);
extern void sleep(SEMAPHORE_ID_TYPE sem_id);
extern void wakeup(SEMAPHORE_ID_TYPE sem_id);
extern void p_body(SEMAPHORE_ID_TYPE sem_id);
extern void v_body(SEMAPHORE_ID_TYPE sem_id);

extern void init_timer(void);
extern void first_task(void);
extern void swtch(void);

extern void P(SEMAPHORE_ID_TYPE sem_id);
extern void V(SEMAPHORE_ID_TYPE sem_id);
extern void pv_handler(void);
/* mtk_c.h */
extern void skipmt(void);

#endif /* MTK_C_H */
