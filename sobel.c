#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define LOW_THRESHOLD 15
#define HIGH_THRESHOLD 35

//All global variables to be used
int pic[256][256];
int outpicx[256][256];
int outpicy[256][256];

int maskx[3][3] = {{-1,0,1}, {-2,0,2}, {-1,0,1}};
int masky[3][3] = {{1,2,1}, {0,0,0}, {-1,-2,-1}};

double ival[256][256], maxival;
double loThresh[256][256];
double hiThresh[256][256];

int main(int argc, char** argv)
{
    int i, j, p, q, mr, sum1, sum2;
    FILE *fo1, *fo2, *fo3, *fp1;
    char *foobar;

    //Increment to the next command line argument
    argc--;
    argv++;

    //Set the file, foobar, to the name of the image file to read in from
    foobar= *argv;
    fp1= fopen(foobar,"rb");

    //Increment to the next command line argument
    argc--;
    argv++;

    //Set the file, foobar, to the name of the image file to write out to
    foobar= *argv;
    fo1= fopen(foobar,"wb");

    //Increment to the next command line argument
    argc--;
    argv++;

    //Set the file, foobar, to the name of the image file to write out to
    foobar= *argv;
    fo2= fopen(foobar,"wb");

    //Increment to the next command line argument
    argc--;
    argv++;

    //Set the file, foobar, to the name of the image file to write out to
    foobar= *argv;
    fo3= fopen(foobar,"wb");

    //Reading the header in just to move the file pointer to the correct position
    fscanf(fp1, "%*s\n%*s %*s\n%*s\n");

    //Store the values of the input file in a 2D array
    for (i=0; i<256; i++)
    {
        for (j=0; j<256; j++)
        {
            pic[i][j]= getc(fp1);
        }
    }

    //Convolution
    mr = 1;
    for (i=mr; i<255-mr; i++)
    {
        for (j=mr; j<255-mr; j++)
        {
            sum1 = 0;
            sum2 = 0;
            for (p=-mr; p<=mr; p++)
            {
                for (q=-mr; q<=mr; q++)
                {
                   sum1 += pic[i+p][j+q] * maskx[p+mr][q+mr];
                   sum2 += pic[i+p][j+q] * masky[p+mr][q+mr];
                }
            }
            outpicx[i][j] = sum1;
            outpicy[i][j] = sum2;
        }
    }

    //Getting the gradient magnitude
    maxival = 0;
    for (i=mr;i<256-mr;i++)
    {
        for (j=mr;j<256-mr;j++)
        {
            ival[i][j]=sqrt((double)((outpicx[i][j]*outpicx[i][j]) + (outpicy[i][j]*outpicy[i][j])));
            if (ival[i][j] > maxival)
                maxival = ival[i][j];
        }
    }

    //Output the header to the file
    fprintf(fo1,"P5\n256 256\n255\n");

    //Doing the scaling of the image afterwards and writing it to the file
    for (i=0;i<256;i++)
    {
        for (j=0;j<256;j++)
        {
            ival[i][j] = (ival[i][j] / maxival) * 255;
            fprintf(fo1,"%c",(char)((int)(ival[i][j])));
        }
    }

    //Output the header to the file
    fprintf(fo2,"P5\n256 256\n255\n");

    //Do low threshold
    for(i=0; i<255; i++)
    {
        for(j=0; j<255; j++)
        {
            if(abs(ival[i+1][j+1]-ival[i][j])>LOW_THRESHOLD)
                loThresh[i][j]=255;
            else
                loThresh[i][j]=0;
        }
    }

    for (i=0;i<256;i++)
    {
        for (j=0;j<256;j++)
        {
            fprintf(fo2,"%c",(char)((int)(loThresh[i][j])));
        }
    }

    //Do high threshold
    for(i=0; i<255; i++)
    {
        for(j=0; j<255; j++)
        {
            if(abs(ival[i+1][j+1]-ival[i][j])>HIGH_THRESHOLD)
                hiThresh[i][j]=255;
            else
                hiThresh[i][j]=0;
        }
    }

    //Output the header to the file
    fprintf(fo3,"P5\n256 256\n255\n");

    for (i=0;i<256;i++)
    {
        for (j=0;j<256;j++)
        {
            fprintf(fo3,"%c",(char)((int)(hiThresh[i][j])));
        }
    }



    return 0;
}
