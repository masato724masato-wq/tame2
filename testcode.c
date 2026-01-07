#include <stdio.h>
#include <stdarg.h>
#include "mtk_c.h"
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
