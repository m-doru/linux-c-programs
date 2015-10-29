#include <stdio.h>
#include <stdlib.h>
#define OUT_OF_RANGE_ACCESS 74
#define FAILED_TO_OPEN_FILE 212
#define ERROR_FILE_READING 223
#define WRONG_ARGS_NR 123
int new(char *filename, int m, int n){
	FILE *f;
	if(NULL == (f= fopen(filename, "wb")))
		return -1;
	fwrite(&m, 1, sizeof(int), f);
	fwrite(&n, 1, sizeof(int), f);
	float a = 0;
	int i;
	for(i = 0; i < m * n; ++i)
		fwrite(&a, 1, sizeof(float), f);
	fclose(f);
	return 0;
}

int get(FILE *f, int i, int j, float *ans){
    long poz = ftell(f);
    int m, n;
    fseek(f, 0, SEEK_SET);
    if(fread(&m, 1, sizeof(int), f) == 0 || fread(&n, 1, sizeof(int), f) == 0){
	fseek(f, poz, SEEK_SET);    	
	return -1;    
    }
        if(i > m || j > n || i <= 0 || j <= 0){
		fseek(f, poz, SEEK_SET);			
		return -1;
        }
        fseek(f, (i-1)*n*sizeof(float) + (j-1)*sizeof(float), SEEK_CUR);
        fread(ans, 1, sizeof(float), f);
	fseek(f, poz, SEEK_SET);        
	return 0;
}
int set(FILE *f, int i, int j, float x){
    int m, n;
    long poz = ftell(f);
    fseek(f, 0, SEEK_SET);
    if(fread(&m, 1, sizeof(int), f) == 0 || fread(&n, 1, sizeof(int), f) == 0){
	fseek(f, poz, SEEK_SET);        
	return -1;
    }
    if(i > m || j > n || i <= 0 || j <= 0){
        fseek(f, poz, SEEK_SET);        
	return -1;
    }
    fseek(f, (i-1)*n*sizeof(float) + (j-1)*sizeof(float), SEEK_CUR);
    fwrite(&x, 1, sizeof(float), f);
    fseek(f, poz, SEEK_SET);   
    return 0;
}
int main(int argc, char *argv[]){
	if(argc != 4){
		printf("Utilizare: program file1 file2 file3\nAbort program!\n");
		exit(WRONG_ARGS_NR);
	}
	FILE *f1, *f2, *f;
	if(NULL == (f1 = fopen(argv[1], "rb"))){
		perror(argv[1]);
		exit(FAILED_TO_OPEN_FILE);
	}
	if (NULL == (f2 = fopen(argv[2], "rb"))){ 
		perror(argv[2]);
		fclose(f1);		
		exit(FAILED_TO_OPEN_FILE);
	}
	if (NULL == (f = fopen(argv[3], "wb+"))){
		perror(argv[3]);
		fclose(f1);
		fclose(f2);
		exit(FAILED_TO_OPEN_FILE);
	}
    int m1, m2, n1, n2;
    fread(&m1, 1, sizeof(int), f1);
    fread(&n1, 1, sizeof(int), f1);

    fread(&m2, 1, sizeof(int), f2);
    fread(&n2, 1, sizeof(int), f2);

    if(n1 != m2){
        printf("Matricile nu se pot inmulti!\n");
        return 0;
    }
    fwrite(&m1, 1, sizeof(int), f);
    fwrite(&n2, 1, sizeof(int), f);

    int i, j,k;
    float rez, a1, a2;
    for(i = 1; i <= m1; ++i){
        for(k = 1; k <= n2; ++k){
            rez = 0;
            for(j = 1; j <= n1; ++j){
                get(f1, i, j, &a1);
		get(f2, j, k, &a2);
            	rez += a1 * a2;
	    }
	    set(f, i, k, rez);
        }
    }

    fclose(f1);
    fclose(f2);
    fclose(f);
	return 0;
}
