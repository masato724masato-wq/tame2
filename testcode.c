#include <stdio.h>
#include "mtk_c.h"
FILE *com0in;   /* UART1 (Port 0) Input  */
FILE *com0out;  /* UART1 (Port 0) Output */
FILE *com1in;   /* UART2 (Port 1) Input  */
FILE *com1out;  /* UART2 (Port 1) Output */

// fd にストリームを割り当てる関数
int main(){
com0in = fdopen(3, "r");
com0out = fdopen(3, "w");
com1in = fdopen(4, "r");
com1out = fdopen(4, "w");
char c;
while(1) {
fscanf(com0in, "%c", &c);
fprintf(com1out, "%c", c);
fscanf(com1in, "%c", &c);
fprintf(com0out, "%c", c);
}
return 0;
}
