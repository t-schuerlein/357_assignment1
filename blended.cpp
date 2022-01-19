#include <stdio.h>
#include <stdlib.h>
#include <cmath>
#include <iostream>


using namespace std;

//TODO argument handling 
//TODO print out explanation of function
//FIXME try to prevent upper right red and upper right blue from causing read error


// defining a Byte
typedef unsigned char Byte;

// bigSize according to teacher: 16
struct BITMAPFILEHEADER {
  short  bfType; //specifies the file type
   int bfSize; //specifies the bigSize in bytes of the bitmap file
   short  bfReserved1; //reserved; must be 0
   short  bfReserved2; //reserved; must be 0
   int bfOffBits; //species the offset in bytes from the bitmapfileheader to the bitmap bits
};

struct BITMAPINFOHEADER {
   int biSize; //specifies the number of bytes required by the struct
   int  biWidth; //specifies width in pixels
   int  biHeight; //species height in pixels
   short  biPlanes; //specifies the number of color planes, must be 1
   short  biBitCount; //specifies the number of bit per pixel
   int biCompression; //spcifies the type of compression
   int biSizeImage; //bigSize of image in bytes
   int  biXPelsPerMeter; //number of pixels per meter in x axis
   int  biYPelsPerMeter; //number of pixels per meter in y axis
   int biClrUsed; //number of colors used by th ebitmap
   int biClrImportant; //number of colors that are important
};



int ByWidthPadding(int pixWidth){
    int bigByWidth = pixWidth * 3;
    if(bigByWidth %4 == 0){

    } else{
        int extra = bigByWidth % 4;
        int padding = 4 - extra;
        bigByWidth += padding;
    }

    return bigByWidth;
}


unsigned char get_green(unsigned char *arr, int x, int y, int byWidth){
    return arr[x*3 + y * byWidth];
}

unsigned char get_blue(unsigned char *arr, int x, int y, int byWidth){
    return arr[(x*3)+1 + y * byWidth];
}

unsigned char get_red(unsigned char *arr, int x, int y, int byWidth){
  
    if( (3*1005 * 754) > (x*3) + 2 + y * byWidth ){
        // printf("x: %d y:%d\n", x, y);
    }
    return arr[(x*3)+2 + y * byWidth];
}

// take in floating pixel, return avg of 4 pixels
Byte* bilinear(unsigned char *idata,float mapX,float mapY,int width2, int height2){

            Byte *result = new Byte[3];

         

           int xLow = (int) mapX;
           int xHigh = xLow + 1;
           int yLow = (int) mapY;
           int yHigh = yLow + 1;

            float dx = mapX - floor(mapX);
            float dy = mapY - floor(mapY);

            // printf("here is test of floor: %d and ceil: %d\n", floor(mapX), ceil(mapX));

            // getting color from upper left pixel
            Byte green_left_upper = get_green(idata, xLow, yHigh, width2);
            Byte green_left_lower = get_green(idata, xLow, yLow, width2);
            Byte green_right_upper = get_green(idata, (xHigh), yHigh, width2);
            Byte green_right_lower = get_green(idata, xHigh, yLow, width2);

            Byte green_left = green_left_upper * ( 1 - dy) + green_left_lower * (dy);
            Byte green_right = green_right_upper * (1 - dy) + green_right_lower * (dy);
            Byte green_avg = (Byte)(green_left * (float)(1-dx) + (float)green_right * dx);
            result[0] = green_avg;

            Byte blue_left_upper = get_blue(idata, xLow, yHigh, width2);
            Byte blue_left_lower = get_blue(idata, xLow, yLow, width2);
            Byte blue_right_upper =  get_blue(idata, xHigh, yHigh, width2);
            Byte blue_right_lower = get_blue(idata, xHigh, yLow, width2);

            Byte blue_left = blue_left_upper * ( 1 - dy) + blue_left_lower * (dy);
            Byte blue_right = blue_right_upper * (1 - dy) + blue_right_lower * (dy);
            Byte blue_avg = (Byte)(blue_left * (float)(1-dx) + (float)blue_right * dx);
            result[1] = blue_avg;



            Byte red_left_upper = get_red(idata, xLow, yHigh, width2);
            Byte red_left_lower = get_red(idata, xLow, yLow, width2);
            

    

            

            Byte red_right_upper = get_red(idata, (xHigh), yHigh, width2);
Byte red_right_lower = get_red(idata, xHigh, yLow, width2);
            
            
            

            Byte red_left = red_left_upper * ( 1 - dy) + red_left_lower * (dy);
            Byte red_right = red_right_upper * (1 - dy) + red_right_lower * (dy);
            Byte red_avg = (Byte)(red_left * (float)(1-dx) + (float)red_right * dx);
            result[2] = red_avg;


    return result;
}



void readHeader(BITMAPFILEHEADER &bfh, BITMAPINFOHEADER &bih, FILE *file){
    // reading in bitmap file header
    fread(&bfh.bfType, 2, 1, file);
    fread(&bfh.bfSize, 4, 1, file);
    fread(&bfh.bfReserved1, sizeof(short), 1, file);
    fread(&bfh.bfReserved2, sizeof(short), 1, file);
    fread(&bfh.bfOffBits, sizeof(int), 1, file);
    // reading in bitmap info header
    fread(&bih, sizeof(bih), 1, file);

}

int setResSize(int *width1, int *height1, int *width2, int *height2, int *width, int *height){
    if(*width1 > *width2){
        // img1 is bigger and res should have its dimensions
        *width = *width1;
        *height = *height1;
    } else {
        *width = *width2;
        *height = *height2;
    }

    return (*width) * (*height);
}


unsigned char* diff3(unsigned char *idata, unsigned char *idata2, int piWidth, int width1, int width2, int height1, int height2, float ratio1){

    int bigWidth;
    int bigHeight;

    Byte *result = new Byte[setResSize(&width1,&height1, &width2, &height2, &bigWidth, &bigHeight)];

    Byte* noChange;
    Byte* scale;
    int smallWidth;
    int smallHeight;

     if(width1 == bigWidth){

                noChange = idata;
                scale = idata2;

                smallWidth = width2;
                smallHeight = height2;

            } else{

                 noChange = idata2;
                 scale = idata;

                 smallWidth = width1;
                 smallHeight = height1;
            }

        // printf("small width: %d small height: %d big width: %d big height: %d\n", smallWidth, smallHeight, bigWidth, bigHeight);

    for(int y = 0; y < bigHeight; y++){
        for(int x = 0; x < piWidth; x++){

            //simple copy of first image
            Byte regG = get_green(noChange, x, y, bigWidth);
            Byte regB = get_blue(noChange, x, y, bigWidth);
            Byte regR = get_red(noChange, x ,y , bigWidth);


            // trying to convert 2nd pic to big width and height
            float mapX = x * ((float) smallWidth / bigWidth);
            float mapY = y * ((float) smallHeight / bigHeight);
            

            Byte* avgPix = (Byte *) bilinear(scale, mapX,mapY,smallWidth, smallHeight);

            result[(x*3)+0 + y * bigWidth] = regG  * ratio1 + avgPix[0] * (1-ratio1);
            result[(x*3)+1 + y * bigWidth] = regB  * ratio1 + avgPix[1] * (1-ratio1);
            result[(x*3)+2 + y * bigWidth] = regR  * ratio1 + avgPix[2] * (1-ratio1);

            delete[] avgPix;
        }
    }
    return result;
}


// arguments: programName, img1, img2, ratio, outputFileName
int main(int argc, char *argv[]){

    // argument handling
    // if(argc < 5){
    //     printf("not enough arguments\n");
    //     return 1;
    // }
    // FILE *file = fopen(argv[1], "rb");
    // FILE *file2 = fopen(argv[2], "rb");
    // FILE *res = fopen(argv[4], "wb");

    // if(!file){
    //     printf("could NOT open image 1\n");
    //     return 1;
    // }

//valgrind

//print how to use function

    // float ratio1 = atof(argv[3]);

    // if(!file2){
    //     printf("could NOT open image 2\n");
    //     return 1;
    // }
    // if(!res){
    //     printf("could NOT open output file");
    //     return 1;
    // }



    FILE *res = fopen("res.bmp", "wb");


    float ratio1 = 0.5;

    FILE *file = fopen("blendimages/jar.bmp", "rb");
    FILE *file2 = fopen("blendimages/tunnel.bmp", "rb");

    BITMAPFILEHEADER bfh1;
    BITMAPINFOHEADER bih1;

    readHeader(bfh1, bih1, file);

    BITMAPFILEHEADER bfh2;
    BITMAPINFOHEADER bih2;

    readHeader(bfh2, bih2, file2);

    int resPiWidth;
    BITMAPINFOHEADER resInfo;
    if (bih1.biWidth > bih2.biWidth){
        resPiWidth = bih1.biWidth;
        resInfo = bih1;
    } else{
        resPiWidth = bih2.biWidth;
        resInfo = bih2;        
    }


    int width1 = ByWidthPadding(bih1.biWidth);
    int height1 = bih1.biHeight;
    int width2 = ByWidthPadding(bih2.biWidth);
    int height2 = bih2.biHeight;

    int resWidth = 0;
    int *resPW = &resWidth;
    int resHeight = 0;
    int *resPH = & resHeight;
    int resSize = setResSize(&width1, &height1, &width2, &height2, resPW, resPH);

    // printf("res width w/ padding:%d height:%d size:%d\n", resWidth, resHeight, resSize);

    int size1 = width1 * height1;
    Byte *idata = new Byte[ size1];
    fread(idata, size1, 1, file);


    printf("size of idata is %d", size1);

    int size2 = width2 * height2;
    Byte *idata2 = new Byte[ size2];
    fread(idata2, size2, 1, file2);



    // printf("size1: %d size2: %d and resSize: %d\n", size1, size2, resSize);


    Byte* result = diff3(idata, idata2, resPiWidth,width1,  width2, height1, height2, ratio1);

    fwrite(&bfh2.bfType, 2, 1, res);
    fwrite(&bfh2.bfSize, 4, 1, res);
    fwrite(&bfh2.bfReserved1, sizeof(short), 1 , res);
    fwrite(&bfh2.bfReserved2, sizeof(short), 1, res);
    fwrite(&bfh2.bfOffBits, sizeof(int), 1, res);

    fwrite(&resInfo, sizeof(resInfo), 1, res);

    fwrite(result, resSize, 1, res);


    delete[] idata;
    delete[] idata2;
    delete[] result;
    fclose(file);
    fclose(file2);
    fclose(res);
    return 0;
}