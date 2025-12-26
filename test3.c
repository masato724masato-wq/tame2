#include <stdio.h>
#include "mtk_c.h"
extern int inkey(int);
extern void skipmt(void);
/* ------------------------------------------------------------------
 * グローバル変数定義
 * ------------------------------------------------------------------ */
FILE *com0in;   /* UART1 (Port 0) Input  */
FILE *com0out;  /* UART1 (Port 0) Output */
FILE *com1in;   /* UART2 (Port 1) Input  */
FILE *com1out;  /* UART2 (Port 1) Output */

/* ------------------------------------------------------------------
 * ユーザタスクの定義
 * ------------------------------------------------------------------ */
/* 共有変数 */
volatile int answer_key = 0;   // 押されたキー
volatile int winner_id = 0;    // どちらが早押しに勝ったか
volatile int answer_player = 0;
volatile int q_type = 0;  
int p1_score = 0, p2_score = 0;
volatile int answer,answer_right1,answer_right2,q_count,correct_count,answer_now,timeout,time_count;
volatile char *answer_char;
volatile int time_left = -1; // 残り時間（-1はタイマー停止中という意味にする）
volatile int p1_hp = 2; // 初期ライフ3
volatile int p2_hp = 2;

int quest_time(){
  if(q_type == '1'){
   time_count = 5;
        /* 問題表示 */
       if(q_count==0){
        fprintf(com0out, "Q: 日本の首都は？ 1:大阪 2:東京 3:福岡 4:名古屋 \n");
        fprintf(com1out, "Q: 日本の首都は？ 1:大阪 2:東京 3:福岡 4:名古屋\n");
        answer = '2';
        answer_char = "東京";
       }
     if(q_count==1){
        fprintf(com0out, "Q: 面積が2番目に大きい国は？ 1:カナダ 2:ロシア 3:アメリカ 4:中国 \n");
        fprintf(com1out, "Q: 面積が2番目に大きい国は？ 1:カナダ 2:ロシア 3:アメリカ 4:中国 \n");
        answer = '1';
        answer_char = "カナダ";
     }
     if(q_count==2){
        fprintf(com0out, "Q: 日本で一番多い苗字は？ 1:鈴木 2:高橋 3:田中 4:佐藤 \n");
        fprintf(com1out, "Q: 日本で一番多い苗字は？ 1:鈴木 2:高橋 3:田中 4:佐藤 \n");
        answer = '4';
        answer_char = "佐藤";
       }
     if(q_count==3){
      fprintf(com0out, "Q: Linuxのマスコットは？  1: 犬  2: 猫　3: ペンギン  4:猿 \n");
        fprintf(com1out, "Q:Linuxのマスコットは？  1: 犬  2: 猫　3: ペンギン  4:猿  \n");
        answer = '3';
        answer_char = "ペンギン";
     }
     if(q_count==4){
      if(correct_count == 0){
        fprintf(com0out, "4問目から記述式になるので答えになる言葉を選んで\n");
        fprintf(com1out, "4問目から記述式になるので答えになる言葉を選んで\n");
      for(volatile int i=0; i<50000; i++){
      }
        fprintf(com0out, "Q: 世界初のプログラマーは？ 1:ビ 2: エイ 3:ステ 4:ダ\n");
        fprintf(com1out, "Q: 世界初のプログラマーは？ 1:ビ 2: エイ 3:ステ 4:ダ \n");
        answer = '2';
       answer_char = "エイダ・ラブレス";
       }
       if(correct_count == 1 ){
            fprintf(com0out, "1:カ 2: モ 3:ス 4:ダ \n");
            fprintf(com1out, "1:カ 2: モ 3:ス 4:ダ \n");
            answer = '4';
        }
      if(correct_count == 2 ){     
           fprintf(com0out, "1:ハ 2: ジョ 3:ヒカ 4:ラブ \n");
          fprintf(com1out, "1:ハ 2: ジョ 3:ヒカ 4:ラブ \n");
          answer = '4';   
         }
      if(correct_count == 3 ){
          
          fprintf(com0out, "1:ハ 2: ジ 3:リー 4:レス\n");
         
          fprintf(com1out, "1:ハ 2: ジ 3:リー 4:レス \n");
          answer = '4';
       }
     }
     if(q_count==5){
      if(correct_count == 0){
        fprintf(com0out, "Q: paypayドームの今の名前は？ 1:ミ 2:ヤ 3:ヒ 4:フ \n");
        fprintf(com1out, "Q: paypayドームの今の名前は？ 1:ミ 2:ヤ 3:ヒ 4:フ \n");
        answer = '1';
        answer_char = "ミズホpaypayドーム";
       }
        if(correct_count == 1 ){
         fprintf(com0out, "1:カミ 2:ズホ 3:クモ 4:モク\n");     
           fprintf(com1out, "1:カミ 2:ズホ 3:クモ 4:モク \n");
           answer = '2';    
       }
       if(correct_count == 2 ){
         fprintf(com0out, "1:pasmo 2:nimoca 3:paypay 4:楽天 \n"); 
          fprintf(com1out, "1:pasmo 2:nimoca 3:paypay 4:楽天 \n");
          answer = '3';
         
        }
      if(correct_count == 3 ){
        
         fprintf(com0out, "1:スタジアム 2:ドーム 3:コート 4:福岡 \n");
         fprintf(com1out, "1:スタジアム 2:ドーム 3:コート 4:福岡 \n");
         answer = '2';
      }
     }
     }

  if(q_type == '2'){
  
  time_count = 3;
     if(q_count==0){
        fprintf(com0out, "Q: アメリカの首都は？ 1:ニューヨーク 2:ワシントン 3:ロサンゼルス 4:カリフォルニア \n");
        fprintf(com1out, "Q:　アメリカの首都は？ 1:ニューヨーク 2:ワシントン 3:ロサンゼルス 4:カリフォルニア\n");
        answer = '2';
        answer_char = "ワシントン";
       }
     if(q_count==1){
        fprintf(com0out, "Q: この中で面積が一番大きい国は？ 1:日本 2:モンゴル 3:グリーンランド 4:中国 \n");
        fprintf(com1out, "Q: この中で面積が一番大きい国は？ 1:日本 2:モンゴル 3:グリーンランド 4:中国 \n");
        answer = '4';
        answer_char = "中国";
     }
     if(q_count==2){
        fprintf(com0out, "Q: 私の苗字は？ 1:鈴木 2:冨田 3:田中 4:佐藤 \n");
        fprintf(com1out, "Q: 私の苗字は？ 1:鈴木 2:冨田 3:田中 4:佐藤 \n");
        answer = '2';
        answer_char = "冨田";
       }
     if(q_count==3){
      fprintf(com0out, "Q: 万博のマスコットは？  1: ミャクミャク  2: 猫　3: ペンギン  4:猿 \n");
        fprintf(com1out, "Q:万博のマスコットは？  1: ミャクミャク  2: 猫　3: ペンギン  4:猿  \n");
        answer = '1';
        answer_char = "ミャクミャク";
     }
     if(q_count==4){
      if(correct_count == 0){
        fprintf(com0out, "4問目から記述式になるので答えになる言葉を選んで\n");
        fprintf(com1out, "4問目から記述式になるので答えになる言葉を選んで\n");
      for(volatile int i=0; i<50000; i++){
      }
        fprintf(com0out, "Q: この講義の名前は？ 1:電気 2: ソフト 3:ハード 4:基礎\n");
        fprintf(com1out, "Q: この講義の名前は？ 1:電気 2: ソフト 3:ハード 4:基礎 \n");
        answer = '2';
       answer_char = "ソフトウェア実験3";
       }
       if(correct_count == 1 ){
            fprintf(com0out, "1:クリーム 2: ウェア 3:ハード 4:実験 \n");
            fprintf(com1out, "1:クリーム 2: ウェア 3:ハード 4:実験 \n");
            answer = '2';
          
        }
      if(correct_count == 2 ){
           fprintf(com0out, "1:ハード 2: 実験 3:開発 4:基礎 \n");
          fprintf(com1out, "1:ハード 2:実験 3:開発 4:基礎 \n");
          answer = '2';
         }
      if(correct_count == 3 ){
          fprintf(com0out, "1:1 2: 2 3:3 4:4\n");
          fprintf(com1out, "1:1 2: 2 3:3 4:4 \n");
          answer = '3';
       }
     }
     if(q_count==5){
      if(correct_count == 0){
        fprintf(com0out, "Q: paypayドームの昔の名前は？ 1:ミ 2:ヤ 3:ヒ 4:フ \n");
        fprintf(com1out, "Q: paypayドームの今の名前は？ 1:ミ 2:ヤ 3:ヒ 4:フ \n");
        answer = '2';
        answer_char = "ヤフオクドーム";
       }
        if(correct_count == 1 ){
      
          fprintf(com0out, "1:ミ 2:フ 3:ク 4:モ\n");
        
           fprintf(com1out, "1:ミ 2:フ 3:ク 4:モ \n");
           answer = '2';
        
       }
       if(correct_count == 2 ){
        
         fprintf(com0out, "1:ウ 2:マク 3:オク 4:マル \n");
       
          fprintf(com1out, "1:ウ 2:マク 3:オク 4:マル \n");
          answer = '3';
         
        }
      if(correct_count == 3 ){
        
         fprintf(com0out, "1:スタジアム 2:ドーム 3:コート 4:福岡 \n");
         fprintf(com1out, "1:スタジアム 2:ドーム 3:コート 4:福岡 \n");
         answer = '2';
      }
     }
     }
     

 return 0;
}

int answer_check(){

 if (answer_right1==1 && answer_right2==1){
   
  fprintf(com0out, "正解は%s\n",answer_char);
  fprintf(com1out, "正解は%s\n",answer_char);
        
        } else {
            if (winner_id == 1) {
            
                p1_score++;
                fprintf(com0out, "正解！\n");
                fprintf(com1out, "相手が正解しました。\n");
                fprintf(com0out, "正解は%s\n",answer_char);
                fprintf(com1out, "正解は%s\n",answer_char);
            }  else{
            
                p2_score++;
                fprintf(com1out, "正解！\n");
                fprintf(com0out, "相手が正解しました。\n");
             fprintf(com1out, "正解は%s\n",answer_char);
              fprintf(com0out, "正解は%s\n",answer_char);
             
                /* Player2正解処理 */
            }
         
        }
        
        winner_id = 0;
        correct_count = 0;
        return 0;
 


}

/* HPを星の数で表示する関数 */
void print_hp(FILE *fp, int hp) {
    fprintf(fp, "[");
    for (int i = 0; i < 3; i++) {
        if (i < hp) {
            fprintf(fp, "*"); // 生存
        } else {
            fprintf(fp, " "); 
        }
    }
    fprintf(fp, "]");
}

/* ステータス表示用 両方の画面に今のHPを出す */
void show_status() {
    /* Player 1 画面 */
    fprintf(com0out, "\nあなた:");
    print_hp(com0out, p1_hp);
    fprintf(com0out, "  相手:");
    print_hp(com0out, p2_hp);
    fprintf(com0out, "\n");

    /* Player 2 画面 */
    fprintf(com1out, "\nあなた:");
    print_hp(com1out, p2_hp);
    fprintf(com1out, "  相手:");
    print_hp(com1out, p1_hp);
    fprintf(com1out, "\n");
}

/* Playerタスク（共通ロジック） */
void task1() {
    int key1;
    while(1){
  key1 = inkey(0);
  if (key1 != -1){
  q_type = key1;
  fprintf(com0out,"難易度は%dです\n",(q_type-48) );
  fprintf(com1out,"難易度は%dです\n",(q_type-48));
    break;
  }
}

    while (1) {
    
        key1 = inkey(0); 

        if (key1 != -1) { /* 何か押されたら */
            P(1);
            /* 2. 早押し判定：回答権セマフォを取得してみる */
            /* P操作。もし相手が先に取っていたらここで待たされる */
         if(q_count <= 3 && answer_right1 != 1){
            
            /* 3. ここに来れた＝回答権ゲット！ */
            if (winner_id == 0 && answer_right1 == 0) { /* まだ誰も回答していないなら */
                answer_key = key1;
                fprintf(com0out, "\nあなたが回答権を得ました！\n");
                fprintf(com1out, "\n相手が回答権を得ました！\n");
                
                if (answer_key == answer){
                 winner_id = 1;
                /* Masterに「回答があった」と知らせる */
                 
                } else{
                 fprintf(com0out, "\n不正解\n");
                 fprintf(com1out, "\n不正解\n");
                 p1_hp--;        // 自分のHPを減らす
                 show_status();  //  HP表示
                 if (answer_right2 == 0){
                 fprintf(com1out,"あなたの番\n");
                 }
                 answer_right1 = 1;
                }
                
 
           } 
        } else if(answer_right1==0 && answer_now != 2 ){/*4問目以降*/
          answer_key = key1;
          answer_now = 1; 
          if(answer_key == answer){
          if(correct_count == 0){
          fprintf(com0out, "\nあなたが回答権を得ました！\n");
                fprintf(com1out, "\n相手が回答権を得ました！\n");
          }
           winner_id = 1;
           correct_count++;
          } else{
          answer_right1 = 1;
          answer_right2 = 1;
           correct_count = 0;
           winner_id = 0;
           answer_now = 0;
           fprintf(com0out, "\n不正解\n");
           fprintf(com1out, "\n不正解\n");
            p1_hp--;        // 自分のHPを減らす
            show_status();  //  HP表示
          
         }
          
         }
         V(1);
        }
        
     
    }
}

void task2() {
    
    int key2;

    while (1) {
        /* 1. キー入力待ち */
       
        key2 = inkey(1); 
         if (key2 != -1) { /* 何か押されたら */
            
            /*早押し判定*/
            P(1); 
          if(q_count <= 3 && answer_right2 != 1){

            /*回答権ゲット*/
            if (winner_id == 0 && answer_right2 == 0) { /* まだ誰も回答していないなら */
                
                answer_key = key2;
                
                fprintf(com1out, "\nあなたが回答権を得ました！\n");
                fprintf(com0out, "\n相手が回答権を得ました！\n");
                
                if (answer_key == answer){
                 winner_id = 2;
                /* Masterに回答があったと知らせる */
                 
                 } else{
                 fprintf(com1out,"\n不正解\n");
                 fprintf(com0out,"\n不正解\n");
                 p2_hp--;        //自分のHPを減らす
                 show_status();  //HP表示
           if (answer_right1 == 0){
                 fprintf(com0out,"あなたの番\n");
                 }
                 answer_right2 = 1;
                
                 }
 
            }
          }else if(answer_right2==0 && answer_now != 1){/*4問目以降*/
            answer_now = 2;
           answer_key = key2;
            if(answer_key == answer){
            if(correct_count == 0){
            fprintf(com1out, "\nあなたが回答権を得ました！\n");
                fprintf(com0out, "\n相手が回答権を得ました！\n");
            }
            correct_count++;
             winner_id = 2;
            }
             if(answer_key != answer) {
             answer_right2 = 1;
             answer_right1 = 1;
             correct_count = 0;
             winner_id = 0;
             answer_now = 0;
             fprintf(com0out, "\n不正解\n");
             fprintf(com1out, "\n不正解\n");
             p2_hp--;        //自分のHPを減らす
             show_status();  //HP表示
             
             
           }
        
         }
      V(1);       
     } 
       
    }
  
}


void task4() {
    while (1) {
    
        for (volatile int i = 0; i < 70000; i++); 

        P(1); /* 変数を操作するのでロックする */
        
        /* タイマーが動いている かつ まだ誰も勝っていない(winner_id==0) 場合 */
        if (time_left > 0 && winner_id == 0 && (answer_right1 == 0 || answer_right2 == 0)) {
            time_left--;

            /* 残り時間を表示 */
            if (time_left <= 5) {
                fprintf(com0out, "あと%d秒\n", time_left);
                fprintf(com1out, "あと%d秒\n", time_left);
            }

            /* 時間切れの処理 */
            if (time_left == 0) {
                fprintf(com0out, "\n--- 時間切れ！ ---\n");
                fprintf(com1out, "\n--- 時間切れ！ ---\n");

                /* 両方間違えたことにする */
                answer_right1 = 1;
                answer_right2 = 1;
                winner_id = 0; 
                correct_count = 0;
                
                
            }
        }
        V(1); /* ロック解除 */
    }
}

/* Masterタスク */
void task3(){
   
fprintf(com0out,"ゲーム開始\n難易度を選んで\n難易度1or2\n");
   while (1) {
    if (q_type != 0){
        break; 
    }
    }
 fprintf(com0out,"ゲーム開始\n正解と思う選択肢を選んで\n");
       fprintf(com1out,"ゲーム開始\n正解と思う選択肢を選んで\n");
 for(volatile int i=0; i<50000; i++){
 }
 
 winner_id = 0;
 answer_key = 0;
 q_count = 0;
    while (1) {
    P(1);
        winner_id = 0;
        answer_key = 0;
        answer_right1 = 0;
        answer_right2 = 0;
        timeout = 0;
       
     quest_time();
     
     
     if (q_count == 6){
      fprintf(com0out,"あなたの点数は%d点\n",p1_score);
        fprintf(com1out,"あなたの点数は%d点\n",p2_score);
        fprintf(com0out,"相手の点数は%d点\n",p2_score);
        fprintf(com1out,"相手の点数は%d点\n",p1_score);
      if (p1_score > p2_score){
       fprintf(com0out,"あなたの勝ち\n");
        fprintf(com1out,"あなたの負け\n");
      } 
       if(p1_score < p2_score){
       fprintf(com1out,"あなたの勝ち\n");
        fprintf(com0out,"あなたの負け\n");
      }
      if(p1_score == p2_score){
       fprintf(com0out,"引き分け\n");
       fprintf(com1out,"引き分け\n");
      }
        while(1){/*終了*/ 
        } 
    }
    
        time_left = time_count;
        /* 回答権のロックを解除（スタート！） */
        V(1);
         while (1){/*時間制限*/
         
    if (winner_id != 0 || (answer_right1 == 1 && answer_right2 == 1)){
        break; 
    }
    }
    
     P(1);
     
     
     
     if (q_count < 4 ){
      answer_check();
        /* 回答があった！ */
     q_count++;
     time_left = -1;
     } else if (correct_count ==4 || correct_count == 0 ){/*回答し終えるか、二人とも間違えたか*/
       answer_check();
       q_count++;
       time_left = -1;
      } else {
        fprintf(com0out,"正解\n次は\n");
        fprintf(com1out,"正解\n次は\n");
        time_left = -1;
       }
       
       if (p1_hp <= 0 || p2_hp <= 0) {
        fprintf(com0out, "\n=== 決着！ ===\n");
        fprintf(com1out, "\n=== 決着！ ===\n");
        
        if (p1_hp > p2_hp) {
            fprintf(com0out, "あなたの勝ち！\n");
            fprintf(com1out, "あなたの負け...\n");
        } else if (p2_hp > p1_hp) {
            fprintf(com1out, "あなたの勝ち！\n");
            fprintf(com0out, "あなたの負け...\n");
        } else {
            fprintf(com0out, "引き分け（ダブルKO）\n");
            fprintf(com1out, "引き分け（ダブルKO）\n");
        }
        
        while(1){} // ゲーム停止
    }
       
      winner_id = 0;
      V(1);
     for(volatile int i=0; i<90000; i++){
     }
    }
   }
   
   
  

  

/*-----------------------------------------------------
 * メイン関数
 * ------------------------------------------------------------------ */
int main(void)
{
    /* 1. カーネルの初期化 */
    init_kernel();


    com0in  = fdopen(3, "r"); /* Port 0 入力 */
    com0out = fdopen(3, "w"); /* Port 0 出力 */

    com1in  = fdopen(4, "r"); /* Port 1 入力 */
    com1out = fdopen(4, "w"); /* Port 1 出力 */

    /* 3. 開始メッセージ */
    printf("\n--- Theme 3 Input Test Start ---\n");
    
    if (com1out == NULL || com1in == NULL) {
        printf("Error: fdopen(4) failed! \n");
        while(1);
    } 

 

    /* 4. タスクの登録 */
    set_task(task3); /* 表示用タスク */
    set_task(task2); /* 入力用タスク */
    set_task(task1);
    set_task(task4);

    /* 5. マルチタスク開始 */
    printf("Starting Multitasking...\n");
    begin_sch();
    return 0;
}

/* exit関数 (crt0.s用) */
void exit(int status) {
    while(1);
}
