#include <stdio.h>

typedef unsigned char Byte;

int main(){


    Byte val = 56;
    float ratio = 0.71;
    Byte ans = ratio * val;

    printf("%d\n", ans);
}