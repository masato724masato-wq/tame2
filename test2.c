#include "mtk_c.h"
#include "stdio.h"
extern void waitP(int sem_id);
extern void outbyte(int i,char c);
extern int inbyte(void);
volatile int count1 = 0;
volatile int count2 = 0;
volatile int count3 = 0;
FILE *com0in;   /* UART1 (Port 0) Input  */
FILE *com0out;  /* UART1 (Port 0) Output */
FILE *com1in;   /* UART2 (Port 1) Input  */
FILE *com1out;  /* UART2 (Port 1) Output */


/* * ユーザ定義 exit() (テキスト 1.4.6節)
 * 万が一 main やタスクが終了した場合の暴走止め
 */
void exit(int value) {
    *(volatile char *)0x00d00039 = 'H'; /* LEDに 'H' を表示 */
    while(1);
}

/* =============================================
 * ユーザタスク定義
 * 何もしない無限ループ (Busy Loop)
 * ============================================= */

void task1(void) {
    while (1) {
    waitP(1);
        count1++;
        printf("1");       /* カウンタ更新 */
        /* 処理が速すぎると画面が埋め尽くされるのでウェイトを入れる */
        for (volatile int i = 0; i < 50000; i++);
        
        
    }
      
}

void task2(void) {
    while (1) {
    waitP(1);
        count2++;       /* カウンタ更新 */
        printf("2");   /* 画面表示 */
     
        /* 処理が速すぎると画面が埋め尽くされるのでウェイトを入れる */
        for (volatile int i = 0; i < 50000; i++);
       /* V(3);*/
    }
}

void task3(void) {
    while (1) {
    waitP(1);
                count3++;       /* カウンタ更新 */
        printf("3");   /* 画面表示 */
        
        /* 処理が速すぎると画面が埋め尽くされるのでウェイトを入れる */
        for (volatile int i = 0; i < 50000; i++);
        /*V(1);:?*/
    }
}


int main(void) {
	setbuf(stdout,NULL);
    /* 1. カーネルの初期化 (TCB, Readyキュー, セマフォ等) */
    init_kernel();
     init_kernel();


    com0in  = fdopen(3, "r"); /* Port 0 入力 */
    com0out = fdopen(3, "w"); /* Port 0 出力 */

    com1in  = fdopen(4, "r"); /* Port 1 入力 */
    com1out = fdopen(4, "w"); /* Port 1 出力 */
    /* 2. ユーザタスクの登録 */
    /* set_task は内部でスタックを確保し、Readyキューに追加する */
    set_task(task1);
    set_task(task2);
    set_task(task3);
    /*semaphore[1].count = 1;
    semaphore[2].count = 0;
    semaphore[3].count = 0;*/
    semaphore[1].count = 0; // 初期値0
    semaphore[1].nst = 3;   // 3人集まったら出発

    /* 3. マルチタスクの開始 */
    /* ここでタイマが起動し、最初のタスクへ切り替わる (戻ってこない) */
    begin_sch();

    /* ここには到達しないはず */
    return 0;
}
