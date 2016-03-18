/*
 * trans.c - Matrix transpose B = A^T
 *
 * Each transpose function must have a prototype of the form:
 * void trans(int M, int N, int A[N][M], int B[M][N]);
 *
 * A transpose function is evaluated by counting the number of misses
 * on a 1KB direct mapped cache with a block size of 32 bytes.
 */
#include <stdio.h>
#include "cachelab.h"
#include <stdlib.h>

int is_transpose(int M, int N, int A[N][M], int B[M][N]);

/*
 * transpose_submit - This is the solution transpose function that you
 *     will be graded on for Part B of the assignment. Do not change
 *     the description string "Transpose submission", as the driver
 *     searches for that string to identify the transpose function to
 *     be graded.
 */
 int max(int a,int b){
    return a<b?b:a;
 }
char transpose_submit_desc[] = "Transpose submission";
void transpose_submit(int M, int N, int A[N][M], int B[M][N]){
    if((M==32)&&(N==32)){
        int i,j,ii,jj,a0,a1,a2,a3,a4,a5,a6,a7;
        for(ii=0;ii<N;ii+=8){           //simple blocking
            for(jj=0;jj<M;jj+=8){
                j=jj;
                for(i=ii;i<ii+8;i++){
                    a0=A[i][j];
                    a1=A[i][j+1];
                    a2=A[i][j+2];
                    a3=A[i][j+3];
                    a4=A[i][j+4];
                    a5=A[i][j+5];
                    a6=A[i][j+6];
                    a7=A[i][j+7];

                    B[j][i]=a0;
                    B[j+1][i]=a1;
                    B[j+2][i]=a2;
                    B[j+3][i]=a3;
                    B[j+4][i]=a4;
                    B[j+5][i]=a5;
                    B[j+6][i]=a6;
                    B[j+7][i]=a7;
                }
            }
        }
    }
    if((M==64)&&(N==64)){   //let M,N be a10,a11
        int a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
        for(M=0;M!=64;M+=8){
            for(N=0;N!=64;N+=8){
                //round 1
                a0=A[N][M];
                a1=A[N][M+1];
                a2=A[N][M+2];
                a3=A[N][M+3];
                a4=A[N][M+4];
                a5=A[N][M+5];
                a6=A[N][M+6];
                a7=A[N][M+7];

                B[M][N]=a0;
                B[M+1][N]=a1;
                B[M+2][N]=a2;
                B[M+3][N]=a3;
                //round 2
                a0=A[N+1][M];
                a1=A[N+1][M+1];
                a2=A[N+1][M+2];
                a3=A[N+1][M+3];
                a8=A[N+1][M+4];
                a9=A[N+1][M+5];
                a10=A[N+1][M+6];
                a11=A[N+1][M+7];

                B[M][N+1]=a0;
                B[M+1][N+1]=a1;
                B[M+2][N+1]=a2;
                B[M+3][N+1]=a3;
                //round 3
                a0=A[N+2][M];
                a1=A[N+2][M+1];
                a2=A[N+2][M+2];
                a3=A[N+2][M+3];

                B[M][N+2]=a0;
                B[M+1][N+2]=a1;
                B[M+2][N+2]=a2;
                B[M+3][N+2]=a3;
                //round 4
                a0=A[N+3][M];
                a1=A[N+3][M+1];
                a2=A[N+3][M+2];
                a3=A[N+3][M+3];

                B[M][N+3]=a0;
                B[M+1][N+3]=a1;
                B[M+2][N+3]=a2;
                B[M+3][N+3]=a3;
                //round 5
                a0=A[N+4][M];
                a1=A[N+4][M+1];
                a2=A[N+4][M+2];
                a3=A[N+4][M+3];

                B[M][N+4]=a0;
                B[M+1][N+4]=a1;
                B[M+2][N+4]=a2;
                B[M+3][N+4]=a3;
                 //round 6
                a0=A[N+5][M];
                a1=A[N+5][M+1];
                a2=A[N+5][M+2];
                a3=A[N+5][M+3];

                B[M][N+5]=a0;
                B[M+1][N+5]=a1;
                B[M+2][N+5]=a2;
                B[M+3][N+5]=a3;
                 //round 7
                a0=A[N+6][M];
                a1=A[N+6][M+1];
                a2=A[N+6][M+2];
                a3=A[N+6][M+3];

                B[M][N+6]=a0;
                B[M+1][N+6]=a1;
                B[M+2][N+6]=a2;
                B[M+3][N+6]=a3;
                 //round 8
                a0=A[N+7][M];
                a1=A[N+7][M+1];
                a2=A[N+7][M+2];
                a3=A[N+7][M+3];

                B[M][N+7]=a0;
                B[M+1][N+7]=a1;
                B[M+2][N+7]=a2;
                B[M+3][N+7]=a3;

                //second column!
                //round 1
                a0=A[N+7][M+4];
                a1=A[N+7][M+5];
                a2=A[N+7][M+6];
                a3=A[N+7][M+7];

                B[M+4][N+7]=a0;
                B[M+5][N+7]=a1;
                B[M+6][N+7]=a2;
                B[M+7][N+7]=a3;

                B[M+4][N]=a4;
                B[M+5][N]=a5;
                B[M+6][N]=a6;
                B[M+7][N]=a7;

                B[M+4][N+1]=a8;
                B[M+5][N+1]=a9;
                B[M+6][N+1]=a10;
                B[M+7][N+1]=a11;
                //round 2
                a0=A[N+4][M+4];
                a1=A[N+4][M+5];
                a2=A[N+4][M+6];
                a3=A[N+4][M+7];
                a4=A[N+5][M+4];
                a5=A[N+5][M+5];
                a6=A[N+5][M+6];
                a7=A[N+5][M+7];
                a8=A[N+6][M+4];
                a9=A[N+6][M+5];
                a10=A[N+6][M+6];
                a11=A[N+6][M+7];

                B[M+4][N+4]=a0;
                B[M+5][N+4]=a1;
                B[M+6][N+4]=a2;
                B[M+7][N+4]=a3;

                B[M+4][N+5]=a4;
                B[M+5][N+5]=a5;
                B[M+6][N+5]=a6;
                B[M+7][N+5]=a7;

                B[M+4][N+6]=a8;
                B[M+5][N+6]=a9;
                B[M+6][N+6]=a10;
                B[M+7][N+6]=a11;
                //round 3
                a0=A[N+2][M+4];
                a1=A[N+2][M+5];
                a2=A[N+2][M+6];
                a3=A[N+2][M+7];
                a4=A[N+3][M+4];
                a5=A[N+3][M+5];
                a6=A[N+3][M+6];
                a7=A[N+3][M+7];

                B[M+4][N+2]=a0;
                B[M+5][N+2]=a1;
                B[M+6][N+2]=a2;
                B[M+7][N+2]=a3;

                B[M+4][N+3]=a4;
                B[M+5][N+3]=a5;
                B[M+6][N+3]=a6;
                B[M+7][N+3]=a7;
            }
        }
    }
    if((N==61)&&(M==67)){
        int a0,a1,a2,a3,a4,a5,a6,a7,a8,a9,a10,a11;
        for(M=0;M<64;M+=8){
            for(N=0;N<56;N+=8){
                if(M==0){
                    a0=A[N][64];
                    a1=A[N][65];
                    a2=A[N][66];
                    a3=A[N+1][64];
                    a4=A[N+1][65];
                    a5=A[N+1][66];
                    a6=A[N+2][64];
                    a7=A[N+2][65];
                    a8=A[N+2][66];
                    a9=A[N+3][64];
                    a10=A[N+3][65];
                    a11=A[N+3][66];

                    B[64][N]=a0;
                    B[65][N]=a1;
                    B[66][N]=a2;
                    B[64][N+1]=a3;
                    B[65][N+1]=a4;
                    B[66][N+1]=a5;
                    B[64][N+2]=a6;
                    B[65][N+2]=a7;
                    B[66][N+2]=a8;
                    B[64][N+3]=a9;
                    B[65][N+3]=a10;
                    B[66][N+3]=a11;

                    a0=A[N+4][64];
                    a1=A[N+4][65];
                    a2=A[N+4][66];
                    a3=A[N+5][64];
                    a4=A[N+5][65];
                    a5=A[N+5][66];
                    a6=A[N+6][64];
                    a7=A[N+6][65];
                    a8=A[N+6][66];
                    a9=A[N+7][64];
                    a10=A[N+7][65];
                    a11=A[N+7][66];

                    B[64][N+4]=a0;
                    B[65][N+4]=a1;
                    B[66][N+4]=a2;
                    B[64][N+5]=a3;
                    B[65][N+5]=a4;
                    B[66][N+5]=a5;
                    B[64][N+6]=a6;
                    B[65][N+6]=a7;
                    B[66][N+6]=a8;
                    B[64][N+7]=a9;
                    B[65][N+7]=a10;
                    B[66][N+7]=a11;
                }
                //row 1
                a0=A[N][M];
                a1=A[N][M+1];
                a2=A[N][M+2];
                a3=A[N][M+3];
                a4=A[N][M+4];
                a5=A[N][M+5];
                a6=A[N][M+6];
                a7=A[N][M+7];

                B[M][N]=a0;
                B[M+1][N]=a1;
                B[M+2][N]=a2;
                B[M+3][N]=a3;
                B[M+4][N]=a4;
                B[M+5][N]=a5;
                B[M+6][N]=a6;
                B[M+7][N]=a7;
                //row 2
                a0=A[N+1][M];
                a1=A[N+1][M+1];
                a2=A[N+1][M+2];
                a3=A[N+1][M+3];
                a4=A[N+1][M+4];
                a5=A[N+1][M+5];
                a6=A[N+1][M+6];
                a7=A[N+1][M+7];

                B[M][N+1]=a0;
                B[M+1][N+1]=a1;
                B[M+2][N+1]=a2;
                B[M+3][N+1]=a3;
                B[M+4][N+1]=a4;
                B[M+5][N+1]=a5;
                B[M+6][N+1]=a6;
                B[M+7][N+1]=a7;
                //row 4
                a0=A[N+2][M];
                a1=A[N+2][M+1];
                a2=A[N+2][M+2];
                a3=A[N+2][M+3];
                a4=A[N+2][M+4];
                a5=A[N+2][M+5];
                a6=A[N+2][M+6];
                a7=A[N+2][M+7];

                B[M][N+2]=a0;
                B[M+1][N+2]=a1;
                B[M+2][N+2]=a2;
                B[M+3][N+2]=a3;
                B[M+4][N+2]=a4;
                B[M+5][N+2]=a5;
                B[M+6][N+2]=a6;
                B[M+7][N+2]=a7;
                //row 4
                a0=A[N+3][M];
                a1=A[N+3][M+1];
                a2=A[N+3][M+2];
                a3=A[N+3][M+3];
                a4=A[N+3][M+4];
                a5=A[N+3][M+5];
                a6=A[N+3][M+6];
                a7=A[N+3][M+7];

                B[M][N+3]=a0;
                B[M+1][N+3]=a1;
                B[M+2][N+3]=a2;
                B[M+3][N+3]=a3;
                B[M+4][N+3]=a4;
                B[M+5][N+3]=a5;
                B[M+6][N+3]=a6;
                B[M+7][N+3]=a7;
                //row 5
                a0=A[N+4][M];
                a1=A[N+4][M+1];
                a2=A[N+4][M+2];
                a3=A[N+4][M+3];
                a4=A[N+4][M+4];
                a5=A[N+4][M+5];
                a6=A[N+4][M+6];
                a7=A[N+4][M+7];

                B[M][N+4]=a0;
                B[M+1][N+4]=a1;
                B[M+2][N+4]=a2;
                B[M+3][N+4]=a3;
                B[M+4][N+4]=a4;
                B[M+5][N+4]=a5;
                B[M+6][N+4]=a6;
                B[M+7][N+4]=a7;
                //row 6
                a0=A[N+5][M];
                a1=A[N+5][M+1];
                a2=A[N+5][M+2];
                a3=A[N+5][M+3];
                a4=A[N+5][M+4];
                a5=A[N+5][M+5];
                a6=A[N+5][M+6];
                a7=A[N+5][M+7];

                B[M][N+5]=a0;
                B[M+1][N+5]=a1;
                B[M+2][N+5]=a2;
                B[M+3][N+5]=a3;
                B[M+4][N+5]=a4;
                B[M+5][N+5]=a5;
                B[M+6][N+5]=a6;
                B[M+7][N+5]=a7;
                //row 7
                a0=A[N+6][M];
                a1=A[N+6][M+1];
                a2=A[N+6][M+2];
                a3=A[N+6][M+3];
                a4=A[N+6][M+4];
                a5=A[N+6][M+5];
                a6=A[N+6][M+6];
                a7=A[N+6][M+7];

                B[M][N+6]=a0;
                B[M+1][N+6]=a1;
                B[M+2][N+6]=a2;
                B[M+3][N+6]=a3;
                B[M+4][N+6]=a4;
                B[M+5][N+6]=a5;
                B[M+6][N+6]=a6;
                B[M+7][N+6]=a7;
                //row 8
                a0=A[N+7][M];
                a1=A[N+7][M+1];
                a2=A[N+7][M+2];
                a3=A[N+7][M+3];
                a4=A[N+7][M+4];
                a5=A[N+7][M+5];
                a6=A[N+7][M+6];
                a7=A[N+7][M+7];

                B[M][N+7]=a0;
                B[M+1][N+7]=a1;
                B[M+2][N+7]=a2;
                B[M+3][N+7]=a3;
                B[M+4][N+7]=a4;
                B[M+5][N+7]=a5;
                B[M+6][N+7]=a6;
                B[M+7][N+7]=a7;
            }
        }
        N=56;
        a0=A[N][64];
        a1=A[N][65];
        a2=A[N][66];
        a3=A[N+1][64];
        a4=A[N+1][65];
        a5=A[N+1][66];
        a6=A[N+2][64];
        a7=A[N+2][65];
        a8=A[N+2][66];
        a9=A[N+3][64];
        a10=A[N+3][65];
        a11=A[N+3][66];

        B[64][N]=a0;
        B[65][N]=a1;
        B[66][N]=a2;
        B[64][N+1]=a3;
        B[65][N+1]=a4;
        B[66][N+1]=a5;
        B[64][N+2]=a6;
        B[65][N+2]=a7;
        B[66][N+2]=a8;
        B[64][N+3]=a9;
        B[65][N+3]=a10;
        B[66][N+3]=a11;

        a0=A[N+4][64];
        a1=A[N+4][65];
        a2=A[N+4][66];

        B[64][N+4]=a0;
        B[65][N+4]=a1;
        B[66][N+4]=a2;
        for(M=0;M<64;M+=8){
            //row 1
            a0=A[N][M];
            a1=A[N][M+1];
            a2=A[N][M+2];
            a3=A[N][M+3];
            a4=A[N][M+4];
            a5=A[N][M+5];
            a6=A[N][M+6];
            a7=A[N][M+7];

            B[M][N]=a0;
            B[M+1][N]=a1;
            B[M+2][N]=a2;
            B[M+3][N]=a3;
            B[M+4][N]=a4;
            B[M+5][N]=a5;
            B[M+6][N]=a6;
            B[M+7][N]=a7;
            //row 2
            a0=A[N+1][M];
            a1=A[N+1][M+1];
            a2=A[N+1][M+2];
            a3=A[N+1][M+3];
            a4=A[N+1][M+4];
            a5=A[N+1][M+5];
            a6=A[N+1][M+6];
            a7=A[N+1][M+7];

            B[M][N+1]=a0;
            B[M+1][N+1]=a1;
            B[M+2][N+1]=a2;
            B[M+3][N+1]=a3;
            B[M+4][N+1]=a4;
            B[M+5][N+1]=a5;
            B[M+6][N+1]=a6;
            B[M+7][N+1]=a7;
            //row 4
            a0=A[N+2][M];
            a1=A[N+2][M+1];
            a2=A[N+2][M+2];
            a3=A[N+2][M+3];
            a4=A[N+2][M+4];
            a5=A[N+2][M+5];
            a6=A[N+2][M+6];
            a7=A[N+2][M+7];

            B[M][N+2]=a0;
            B[M+1][N+2]=a1;
            B[M+2][N+2]=a2;
            B[M+3][N+2]=a3;
            B[M+4][N+2]=a4;
            B[M+5][N+2]=a5;
            B[M+6][N+2]=a6;
            B[M+7][N+2]=a7;
            //row 4
            a0=A[N+3][M];
            a1=A[N+3][M+1];
            a2=A[N+3][M+2];
            a3=A[N+3][M+3];
            a4=A[N+3][M+4];
            a5=A[N+3][M+5];
            a6=A[N+3][M+6];
            a7=A[N+3][M+7];

            B[M][N+3]=a0;
            B[M+1][N+3]=a1;
            B[M+2][N+3]=a2;
            B[M+3][N+3]=a3;
            B[M+4][N+3]=a4;
            B[M+5][N+3]=a5;
            B[M+6][N+3]=a6;
            B[M+7][N+3]=a7;
            //row 5
            a0=A[N+4][M];
            a1=A[N+4][M+1];
            a2=A[N+4][M+2];
            a3=A[N+4][M+3];
            a4=A[N+4][M+4];
            a5=A[N+4][M+5];
            a6=A[N+4][M+6];
            a7=A[N+4][M+7];

            B[M][N+4]=a0;
            B[M+1][N+4]=a1;
            B[M+2][N+4]=a2;
            B[M+3][N+4]=a3;
            B[M+4][N+4]=a4;
            B[M+5][N+4]=a5;
            B[M+6][N+4]=a6;
            B[M+7][N+4]=a7;
        }
    }
}

void transpose_loadinbatch(int M, int N, int A[N][M], int B[M][N]){  //first edition, load 8 data (8*4=32) together
    int i=0,j=0,a0,a1,a2,a3,a4,a5,a6,a7,temp;
    temp=32-(long)(&A[0][0])%32;
    if((temp%4)!=0){
        printf("Address not aligned to 4!\n");
        exit(-1);
    }
    /* manully align the block offset to 0*/
    if(temp!=32){
        if(temp>=28){               //not using switch cause jump table may cause additional non-stack memory reference
            a7=A[i][j+6];
        }
        if(temp>=24){
            a6=A[i][j+5];
        }
        if(temp>=20){
            a5=A[i][j+4];
        }
        if(temp>=16){
            a4=A[i][j+3];
        }
        if(temp>=12){
            a3=A[i][j+2];
        }
        if(temp>=8){
            a2=A[i][j+1];
        }
        if(temp>=4){
            a1=A[i][j];
        }

        if(temp>=28){
            B[j+6][i]=a7;
        }
        if(temp>=24){
            B[j+5][i]=a6;
        }
        if(temp>=20){
            B[j+4][i]=a5;
        }
        if(temp>=16){
            B[j+3][i]=a4;
        }
        if(temp>=12){
            B[j+2][i]=a3;
        }
        if(temp>=8){
            B[j+1][i]=a2;
        }
        if(temp>=4){
            B[j][i]=a1;
        }
        j+=temp/4;
    }


    while(1){
        temp=M-j;
        if(((i+1)>=N)&&(temp<=8)){ //closing off
            if(temp==8){
                a0=A[i][j+7];
            }
            if(temp>=7){
                a7=A[i][j+6];
            }
            if(temp>=6){
                a6=A[i][j+5];
            }
            if(temp>=5){
                a5=A[i][j+4];
            }
            if(temp>=4){
                a4=A[i][j+3];
            }
            if(temp>=3){
                a3=A[i][j+2];
            }
            if(temp>=2){
                a2=A[i][j+1];
            }
            if(temp>=1){
                a1=A[i][j];
            }

            if(temp==8){
                B[j+7][i]=a0;
            }
            if(temp>=7){
                B[j+6][i]=a7;
            }
            if(temp>=6){
                B[j+5][i]=a6;
            }
            if(temp>=5){
                B[j+4][i]=a5;
            }
            if(temp>=4){
                B[j+3][i]=a4;
            }
            if(temp>=3){
                B[j+2][i]=a3;
            }
            if(temp>=2){
                B[j+1][i]=a2;
            }
            if(temp>=1){
                B[j][i]=a1;
            }
            return;

        }
        else{
            a0=A[i][j];
            a1=A[i][j+1];
            a2=A[i][j+2];
            a3=A[i][j+3];
            a4=A[i][j+4];
            a5=A[i][j+5];
            a6=A[i][j+6];
            a7=A[i][j+7];

            B[j][i]=a0;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a1;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a2;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a3;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a4;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a5;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a6;
            j++;
            i+=j/M;
            j%=M;
            B[j][i]=a7;
            j++;
            i+=j/M;
            j%=M;
        }
    }
    printf("error! control should never reach here! \n");
    exit(-1);
}
/*
 * You can define additional transpose functions below. We've defined
 * a simple one below to help you get started.
 */

/*
 * trans - A simple baseline transpose function, not optimized for the cache.
 */
char trans_desc[] = "Simple row-wise scan transpose";
void trans(int M, int N, int A[N][M], int B[M][N])
{
    int i, j, tmp;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; j++) {
            tmp = A[i][j];
            B[j][i] = tmp;
        }
    }

}

/*
 * registerFunctions - This function registers your transpose
 *     functions with the driver.  At runtime, the driver will
 *     evaluate each of the registered functions and summarize their
 *     performance. This is a handy way to experiment with different
 *     transpose strategies.
 */
void registerFunctions()
{
    /* Register your solution function */
    registerTransFunction(transpose_submit, transpose_submit_desc);

    /* Register any additional transpose functions */
    registerTransFunction(trans, trans_desc);

}

/*
 * is_transpose - This helper function checks if B is the transpose of
 *     A. You can check the correctness of your transpose by calling
 *     it before returning from the transpose function.
 */
int is_transpose(int M, int N, int A[N][M], int B[M][N])
{
    int i, j;

    for (i = 0; i < N; i++) {
        for (j = 0; j < M; ++j) {
            if (A[i][j] != B[j][i]) {
                return 0;
            }
        }
    }
    return 1;
}

