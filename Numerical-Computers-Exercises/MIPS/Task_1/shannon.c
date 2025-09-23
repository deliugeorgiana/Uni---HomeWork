#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <fstream>

//compile gcc -o shannon shannon.c -lm
//run ./shannon tom_sawyer_intro_mini.txt

int main(int argc, char **argv){
    int i,j, len, N;
    int *N_ap;
    float Info;
    float *p;
    char *buffer;

    File *inFile, *outFile;

    inFile= fopen(argv[1],"r");

    fseek(inFile,0, SEEK_END);
    len=ftell(inFile);
    rewind(inFile);

    if((buffer=malloc(len*sizeof(char)))==NULL){
        printf("Eroare alocare memorie\n");
        return(1);
    }

    fread(buffer,len,1,inFile);

    fclose(inFile);

    for(i=0;i<10;i++){
        printf("%i \n",*(buffer+i));
    }

    N=128;
    N_ap=malloc(N*sizeof(int));
    p=malloc(N*sizeof(float));

    for(i=0;i<N;i++){
        N_ap[i]=0;
    }
    for(i=0;i<len;i++){
        j=*(buffer+i);
        N_ap[j]=N_ap[j]+1;
    }

    for(i=0;i<N;i++){
        p[i]=float(N_ap[i])/len;
        if(p[i]!=0) printf("%i %f\n",i,p[i]);
    }

    Info=0;

    for(i=0;i<N;i++){
        if(p[i]!=0) Info=Info-p[i]*log2(p[i]);
    }
    printf("Info=%f\n",Info);
    
}