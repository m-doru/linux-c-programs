#include <stdio.h>
#include <fcntl.h>
#include <signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#define MAXSIZE 500000
char c[MAXSIZE], B[MAXSIZE];

int check(pid_t pidret, int s){
	if(WIFEXITED(s)) return WEXITSTATUS(s);
	return 0;
}

int Find(int *c, int l, int r, int nr){
	
	if(l == r)
		return c[l] == nr;	
	pid_t f1, f2;
	int m = (r + l) >> 1;
	errno = EAGAIN;
	while(errno == EAGAIN){
		errno = 0;
		if(!(f1 = fork())){
			exit(Find(c, l, m, nr));
		}
	}
	errno = EAGAIN;
	while(errno == EAGAIN){
		errno = 0;
		if(!(f2 = fork())){
			exit(Find(c, m+1, r, nr));
		}
	}
	pid_t r1, r2;
	int stare1, stare2;
	r1 = waitpid(f1, &stare1, 0);
	r2 = waitpid(f2, &stare2, 0);
	return check(r1, stare1) || check(r2, stare2);
}
void readFile(char *c, int d){
	int i = 0;
	while(read(d, &c[i++], 1) > 0 && i < MAXSIZE);
}
int main(int argc, char*args[]){
	if(argc < 3 ){
		perror("Utilizare: numar vectorul_de_numere\n");
		return -1;
	}	

	int nr;
	int *v = (int*)malloc((argc) * sizeof(int));

	sscanf(args[1], "%d", &nr);
	for(int i = 2; i < argc; ++i)
		sscanf(args[i], "%d", v+i-2);

	if(Find(v, 0, argc-3, nr) == 1)
		printf("Gasit!\n");
	else
		printf("Negasit!\n");	

	free(v);
}
