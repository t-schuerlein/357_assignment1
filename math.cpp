#include <stdio.h>

typedef unsigned char Byte;

int main(){


    Byte left_upper = 147;
    Byte left_lower = 152;
    Byte right_upper = 99;
    Byte right_lower = 47;

    Byte left = left_upper * ( 1 - dy) + left_lower * (dy);
    Byte right = right_upper * (1 - dy) + right_lower * (dy);
    Byte avg = (Byte)(left * (float)(1-dx) + (float)right * dx);


    printf("%d\n", avg);
}