#include <stdio.h>
#include <stdlib.h>
#include <cmath>
// typedef int LONG;
// typedef unsigned short WORD;
// typedef unsigned int DWORD;


// defining a Byte
typedef unsigned char Byte;

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


unsigned char get_color(unsigned char *arr, int x, int y, int byWidth){
    return arr[x + y * byWidth];
}

unsigned char get_green(unsigned char *arr, int x, int y, int byWidth){
    return arr[x + y * byWidth];
}

unsigned char get_blue(unsigned char *arr, int x, int y, int byWidth){
    return arr[x+1 + y * byWidth];
}

unsigned char get_red(unsigned char *arr, int x, int y, int byWidth){
    return arr[x+2 + y * byWidth];
}

// take in floating pixel, return avg of 4 pixels
Byte* bilinear(unsigned char *idata,float mapX,float mapY,int
width2, int bigWidth, int height2, int bigHeight){
    
            Byte *result = new Byte[3];
  
            if(mapX > width2 || mapY > height2){
                printf("this is bad\n");
            }
          
           int xLow = (int) mapX;
           int xHigh = xLow + 1;
           int yLow = (int) mapY;
           int yHigh = yLow + 1;

            float dx = mapX - floor(mapX);
            float dy = mapY - floor(mapY);

            // printf("here is test of floor: %d and ceil: %d\n", floor(mapX), ceil(mapX));

            // getting color from upper left pixel
            Byte green_left_upper = get_green(idata, xLow*3, yHigh, width2);
            Byte green_left_lower = get_green(idata, xLow*3, yLow, width2);
            Byte green_right_upper = get_green(idata, (xHigh)*3, yHigh, width2);
            Byte green_right_lower = get_green(idata, 3*xHigh, yLow, width2);

            Byte green_left = green_left_upper * ( 1 - dy) + green_left_lower * (dy);
            Byte green_right = green_right_upper * (1 - dy) + green_right_lower * (dy);
            Byte green_avg = (Byte)(green_left * (float)(1-dx) + (float)green_right * dx);
            result[0] = green_avg;

            Byte blue_left_upper = get_blue(idata, xLow*3, yHigh, width2);
            Byte blue_left_lower = get_blue(idata, xLow*3, yLow, width2);
            Byte blue_right_upper = get_blue(idata, (xHigh)*3, yHigh, width2);
            Byte blue_right_lower = get_blue(idata, 3*xHigh, yLow, width2);

            Byte blue_left = blue_left_upper * ( 1 - dy) + blue_left_lower * (dy);
            Byte blue_right = blue_right_upper * (1 - dy) + blue_right_lower * (dy);
            Byte blue_avg = (Byte)(blue_left * (float)(1-dx) + (float)blue_right * dx);
            result[1] = blue_avg;


            Byte red_left_upper = get_red(idata, xLow*3, yHigh, width2);
            Byte red_left_lower = get_red(idata, xLow*3, yLow, width2);
            Byte red_right_upper = get_red(idata, (xHigh)*3, yHigh, width2);
            Byte red_right_lower = get_red(idata, 3*xHigh, yLow, width2);

            Byte red_left = red_left_upper * ( 1 - dy) + red_left_lower * (dy);
            Byte red_right = red_right_upper * (1 - dy) + red_right_lower * (dy);
            Byte red_avg = (Byte)(red_left * (float)(1-dx) + (float)red_right * dx);
            result[2] = red_avg;
            

    return result;
}

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

unsigned char* sameSize(unsigned char *idata, unsigned char *idata2, int width, int height, float ratio1){

    Byte *result = new Byte[width * height];

    for(int y = 0; y < height; y++){

        for(int x = 0; x < width; x++){
            // Byte bi1 = idata[x + y * width];
            // Byte bi2 = idata2[x + y *width];
            Byte bi1 = get_color(idata, x, y, width);
            Byte bi2 = get_color(idata2, x, y, width);
            Byte avg = bi1 *  (ratio1) + bi2 * (1-ratio1);
            // printf("avg: %d\n", avg);
            result[x+ y *width] = avg;

            // printf("bi1: %d bi2: %d and avg: %d\n", bi1, bi2, avg);
        }
    }


    return result;


}


unsigned char* same3(unsigned char *idata, unsigned char *idata2, int pixWidth, int height, float ratio1){

    int biWidth = ByWidthPadding(pixWidth);
    Byte *result = new Byte[ByWidthPadding(biWidth) * height];

    for(int y = 0; y < height; y++){

        for(int x = 0; x < pixWidth; x++){
     
            Byte biB = get_color(idata, (x*3), y, biWidth);
            Byte biG = get_green(idata, (x*3)+1, y, biWidth);
           Byte biR = get_color(idata, (x*3)+2, y, biWidth);
           Byte bi2B = get_color(idata2, (3*x), y, biWidth);
            Byte bi2G = get_green(idata2, (x*3)+1, y, biWidth);
           Byte bi2R = get_color(idata2, (x*3)+2, y, biWidth);
            
           Byte avgB = biB * (ratio1) + bi2B * ( 1 -ratio1);
            Byte avgG = biG * (ratio1) + bi2G * ( 1 -ratio1);
    
           result[(x*3)+ y *biWidth] = avgB;
            result[(x*3)+1+ y *biWidth] = avgG;
        //    result[(x*3)+2+ y *biWidth] = avgR;

            // printf("bi1: %d bi2: %d and avg: %d\n", bi1, bi2, avg);
        }
    }
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


unsigned char* diffSize(unsigned char *idata, unsigned char *idata2, int width1, int width2, int height1, int height2, float ratio1){

    int bigWidth;
    int bigHeight;

    Byte *result = new Byte[setResSize(&width1,&height1, &width2, &height2, &bigWidth, &bigHeight)];

    for(int y = 0; y < bigHeight; y++){
        for(int x = 0; x < bigWidth; x++){
            // trying to convert 2nd pic to big width and height
            float mapX = x * ((float) width2/ bigWidth);
            float mapY = y * ((float) height2 / bigHeight);
            // result[x+y*bigWidth] = bilinear(idata2, mapX,mapY,width2, bigWidth, height2, bigHeight);

            //trying with just x scaling
            
            if( x < width2 && y == 0){
                //  printf("width2= %d at x=%d and y=%d we are mapping to mapX=%f and mapY=%f\n", x, y, mapX, mapY);
                // printf("xlow=%d xhigh=%d ylow=%d yhigh=%d x: %d y: %d \n", xLow, xHigh, yLow,yHigh,x,y);
                // printf("left_upper: %d left_lower: %d left: %d right_upper: %d right_lower: %d right: %d avg is %d\n",left_upper, left_lower, left, right_upper, right_lower, right, avg);
            }

        }
    }
    return result;
}


unsigned char* diff3(unsigned char *idata, unsigned char *idata2, int piWidth, int width1, int width2, int height1, int height2, float ratio1){

    int bigWidth;
    int bigHeight;

    Byte *result = new Byte[setResSize(&width1,&height1, &width2, &height2, &bigWidth, &bigHeight)];

    for(int y = 0; y < bigHeight; y++){
        for(int x = 0; x < 1280; x++){
            // trying to convert 2nd pic to big width and height
            float mapX = (x) * ((float) width2/ bigWidth);
            float mapY = y * ((float) height2 / bigHeight);
            Byte *avgPix = new Byte[3];
            
            avgPix = (Byte *) bilinear(idata2, mapX,mapY,width2, bigWidth, height2, bigHeight);

            result[(x*3)+y*bigWidth] = avgPix[0];
            result[(x*3)+1 + y * bigWidth] = avgPix[1];
            result[(x*3)+2 + y * bigWidth] = avgPix[2];
            // result[(x*3)+1 +y*bigWidth] = bilinear(idata2, mapX+1,mapY,width2, bigWidth, height2, bigHeight);
            // result[(x*3)+2+y*bigWidth] = bilinear(idata2, mapX+2,mapY,width2, bigWidth, height2, bigHeight);
            //trying with just x scaling
            
            if( x == piWidth){
           
           
           }

        }
    }
    return result;
}


// arguments: programName, img1, img2, ratio, outputFileName
int main(int argc, char *argv[]){

    FILE *res = fopen("res.bmp", "wb");

    
    float ratio1 = 0.5;
    float ratio2 = 0.5;


    FILE *file = fopen("blendimages/jar.bmp", "rb");
    FILE *file2 = fopen("blendimages/flowers.bmp", "rb");

    BITMAPFILEHEADER bfh1;
    BITMAPINFOHEADER bih1;

    readHeader(bfh1, bih1, file);

    BITMAPFILEHEADER bfh2;
    BITMAPINFOHEADER bih2;

    readHeader(bfh2, bih2, file2);
    
    printf("\npixel width of tunnel should be 1005 but is %d\n", bih2.biWidth);

    int width1 = ByWidthPadding(bih1.biWidth);
    int height1 = bih1.biHeight;
    int width2 = ByWidthPadding(bih2.biWidth);
    int height2 = bih2.biHeight;

    int resWidth = 0;
    int *resPW = &resWidth;
    int resHeight = 0;
    int *resPH = & resHeight;
    int resSize = setResSize(&width1, &height1, &width2, &height2, resPW, resPH);


    // padding the bytes, not the pixels. The bytes need to be divisible by 4
    // int bigByWidth;
    // int littleByWidth;
    // int height;
    // int big;
    // if(bih1.biWidth > bih2.biWidth){
    //     bigByWidth = ByWidthPadding(bih1.biWidth);
    //     littleByWidth = ByWidthPadding(bih2.biWidth);
    //     height = bih1.biHeight;
    //     big = 1;
    // } else {
    //     bigByWidth = ByWidthPadding(bih2.biWidth);
    //     littleByWidth = ByWidthPadding(bih2.biWidth);
    //     height = bih2.biHeight;
    //     big = 2;
    // }

    // // // bigSize gives us the # of color bytes in image
    // // // height * real_width * bytes_per_pixel

    
    // int bigSize;
    // int littleSize;
    // if( big == 1){
    //     bigSize = bigByWidth*bih1.biHeight;
      
    // } else {
    //     bigSize = bigByWidth*bih2.biHeight;
        
    // }
   
//    // getting the color array of image

    printf("res width w/ padding:%d height:%d size:%d\n", resWidth, resHeight, resSize);

    
    int size1 = width1 * height1;
    Byte *idata = new Byte[ size1];
    fread(idata, size1, 1, file);

    int size2 = width2 * height2;
    Byte *idata2 = new Byte[ size2];
    fread(idata2, size2, 1, file2);

    
//    // the resulting output color array
    // Byte *result = new Byte[resSize];
    Byte *result = new Byte[resSize];

    printf("size1: %d size2: %d and resSize: %d", size1, size2, resSize);

    // printf("ratio1: %f ratio2: %f\n", ratio1, ratio2);

    

//    //  // printf("fixed width: %d\n", bigByWidth);
//    //  // printf("image bigSize is %d \n",  bih1.biSizeImage);

    // printf("little byte width: %d big byte width: %d\n", littleByWidth, bigByWidth);
 

//    result = sameSize(idata, idata2, width1,height1,ratio1);
// result = diffSize(idata, idata2, width1, width2, height1, height2, ratio1);
result = diff3(idata, idata2, bih1.biWidth,width1,  width2, height1, height2, ratio1);

// result = same3(idata, idata2, bih1.biWidth, height1, ratio1);


    // for(int y = 0; y < resHeight; y++){
    //     for(int x = 0; x < resWidth; x++){
    //         Byte curr = idata[x + y*width1];
    //         // Byte curr2 = idata2[x + y*width2];


    //         float wFrac = (float ) width2 / (float) resWidth;
    //         float yFrac = (float) height2 / (float) resHeight;
        
    //         float xf = ((float) x) * wFrac;
    //         float yf = ((float) y ) * yFrac;

    //         // int xR = ceil(xf);
    //         // int xL = floor(xf);

    //         // float dx = xf - (float)xL;



    //         // Byte left = idata2[ (int) ( xL + y * width2)   ];
    //         // Byte right = idata2[  (int)  (xR + y * width2   )   ];

            
    //         // Byte avg = left * 0.5 + right * 0.5;

    //         // if(avg == 0 && y == 0 && xf < 1005){
                
    //         // printf("\nxf: %f little xR: %d big xL: %d float dx: %f\n", xf,  xR, xL, dx);
    //         //     printf("gen_color: %d left: %d right: %d avg: %d\n", idata2[xL + y*littleByWidth], left, right, avg);
    //         // }

    //         // result[x + y *resWidth] = avg;

    //         // printf("x is %d. xf is %f\n", x, xf);

    //         if (bilinear(idata2, xf,yf, width2,height2) == 0){
    //             numBlack += 1;
    //         }

    //         result[x+y*resWidth] = bilinear(idata2, xf,yf, width2,height2);


    //     }
    // }

    // printf("numBlack is %d\n", numBlack);


//    for(int y = 0; y < height; y ++){
//       for(int x = 0; x < bigByWidth; x++){
            
//         Byte curr = idata[x + y*bigByWidth];
//         Byte curr2 = idata2[x + y*bigByWidth];
//         if( big == 1){
//             curr = idata[x + y * bigByWidth];



//             float x2 = x * (bih2.biWidth / (float) bih1.biWidth);
//             // float y2 = y * (bih2.biHeight / (float)bih1.biHeight);
    
//             // printf("calc x: %f y:%f\n", x2, y2);
//             // xS = x_small xL = x_large
//             int xL = ceil(x2);
//             int xS = floor(x2);
//             // int yL = ceil(y2);
//             // int yS = floor(y2);

//             float dx = x2 - ((float)  xS );
//             // float dy = y2 - ((float)  yS );

            
//             // printf("x: %f y:%f dx:%f dy:%f\n", x2, y2, dx, dy);

//             Byte color_upper_left = idata2[xS + y * bigByWidth];
//             Byte color_upper_right = idata2[xL + y * bigByWidth];
//             Byte color_lower_left = idata2[xL + y * bigByWidth];
//             Byte color_lower_right = idata2[xS + y * bigByWidth];

//             Byte color_left = color_upper_left * (0.5) + color_lower_left * 0.5;
            
//             Byte color_right = color_upper_right * (0.5) + color_lower_right * 0.5;



//             Byte color_result = color_left * (0.5) + color_right * 0.5;


//             // result[x+ y *bigByWidth] = (curr * ratio1) + (color_result * ratio2);
//             result[x+ y*bigByWidth] = color_result;


//         }

//         // else {

//         //     curr2 = idata2[x+ y *bigByWidth];
//         //     float x1;
//         //     float y1;

//         //     x1 = x * (bih1.biWidth/ (float)  bih2.biWidth);
//         //     y1 = y * (bih1.biWidth/  (float) bih2.biWidth);
//         //     // xS = x_small xL = x_large
//         //     int xL = ceil(x1);
//         //     int xS = floor(x1);
//         //     int yL = ceil(y1);
//         //     int yS = floor(y1);

//         //     float dx = x1 - ( (float) xS);
//         //     float dy = y1 - ( (float ) yS);

//         //     Byte color_upper_left = idata[xS + yL * bigByWidth];
//         //     Byte color_upper_right = idata[xL + yL * bigByWidth];
//         //     Byte color_lower_left = idata[xL + yS * bigByWidth];
//         //     Byte color_lower_right = idata[xS + yS * bigByWidth];

//         //     Byte color_left = color_upper_left * (1-dy) + color_lower_left * dy;
//         //     Byte color_right = color_upper_right * (1 -dy) + color_lower_right * dy;
//         //     Byte color_result = color_left * (1-dx) + color_right * dx;

//         //     result[x+ y *bigByWidth] = (color_result * ratio1) + (curr2 * ratio2);


//         // }
         
//         //  result[x + y * bigByWidth ] = (curr*ratio1) + (curr2*ratio2);
//    //  //         result[x*3 + y * bigByWidth * 3 + 1] = g;
//    //  //         result[x*3 + y * bigByWidth* 3 + 2] = r;

//       }
//    }

    
    // fwrite(&bfh1.bfType, 2, 1, res);
    // fwrite(&bfh1.bfSize, 4, 1, res);
    // fwrite(&bfh1.bfReserved1, sizeof(short), 1, res);
    // fwrite(&bfh1.bfReserved2, sizeof(short), 1, res);
    // fwrite(&bfh1.bfOffBits, sizeof(int), 1, res);
    // // reading in bitmap info header
    // fwrite(&bih1, sizeof(bih1), 1, res);

    // fwrite(result, sizeof(result), 1, res);

    
    
    fwrite(&bfh1.bfType, 2, 1, res);
    fwrite(&bfh1.bfSize, 4, 1, res);
    fwrite(&bfh1.bfReserved1, sizeof(short), 1 , res);
    fwrite(&bfh1.bfReserved2, sizeof(short), 1, res);
    fwrite(&bfh1.bfOffBits, sizeof(int), 1, res);
    
    fwrite(&bih1, sizeof(bih1), 1, res);

    fwrite(result, resSize, 1, res);


    delete[] idata;
    delete[] result;
    fclose(file);
    fclose(res);
    return 0;
}