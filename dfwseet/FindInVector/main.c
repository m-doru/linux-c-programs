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
	return check(r1, stare1) + check(r2, stare2);
}

int main(int argc, char*args[]){
	if(argc < 3 ){
		perror("Utilizare: numar vectorul_de_numere\n");
		return 1;
	}	

	int nr;
	int *v = (int*)malloc((argc-2) * sizeof(int));

	sscanf(args[1], "%d", &nr);
	int i;
	for(i = 2; i < argc; ++i)
		sscanf(args[i], "%d", v+i-2);

	int ans = Find(v, 0, argc-3, nr);

	if(ans > 0)
		printf("Gasit de %d ori!\n", ans);
	else
		printf("Negasit!\n");	

	free(v);
}
