#include <stdio.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/shm.h>

#define MAXSIZE 500000
unsigned char *c, B[MAXSIZE];

int check(pid_t pidret, int s, int untraced){
	if(pidret == -1){
		if(errno == ECHILD)
			return 0;
		if(errno == EAGAIN)
			return 1;
	}
	if(WIFEXITED(s)) return 0;
	if(WIFSIGNALED(s))
		if(WTERMSIG(s) == SIGSEGV)
			return 0;
}

void MergeSort(unsigned char *c, int l, int r){
	
	if(l >= r)
		return;
//	printf("Se proceseaza bucata %d - %d\n", l, r);
	pid_t f1, f2;
	int m = (r + l) >> 1;
	errno = EAGAIN;
	while(errno == EAGAIN){
		errno = 0;
		if(!(f1 = fork())){
			MergeSort(c, l, m);
			exit(0);
		}
	}
	errno = EAGAIN;
	while(errno == EAGAIN){
		errno = 0;
		if(!(f2 = fork())){
			MergeSort(c, m+1, r);
			exit(0);
		}
	}
	pid_t r1, r2;
	int stare1, stare2;
	r1 = waitpid(f1, &stare1, 0);
	r2 = waitpid(f2, &stare2, 0);

//	printf("Se mergeuieste bucata %d - %d\n", l, r);
	int i,k,j;
	for(i = l, j = m+1, k = l; i <= m || j <= r;)
		if(j > r  || i <= m && c[i] < c[j])
			B[k++] = c[i++];
		else
			B[k++] = c[j++];
	for(i = l; i <= r; ++i)
		c[i] = B[i];
}
int readFile(unsigned char *c, int d){
	int i = 0;
	while(read(d, &c[i++], 1) > 0 && i < MAXSIZE);
	return i;
}
void print(unsigned char *c, int d){
	int size = strlen(c);
	int i;
	for(i = 0; i < size; ++i)
		write(d, &c[i], 1);
}
int main(int argc, char*args[]){
	if(argc != 3){
		perror("Utilizare: program nume_fisier_intrare nume_fisier_sortat\n");
		return -1;
	}	

	int d = open(args[1], O_RDONLY);
	if(d == -1){
		perror(args[1]);
		return -1;
	}

	key_t key = IPC_PRIVATE;
	int shmid = shmget(key, MAXSIZE * sizeof(int), IPC_CREAT|0666);
	c = (unsigned char*)shmat(shmid, NULL, 0);
	readFile(c, d);
	close(d);
	
	c[strlen(c)] = '\0';
	
	MergeSort(c, 0, strlen(c)-1);

	d = open(args[2], O_WRONLY|O_CREAT|O_TRUNC, 0666);
	print(c,d);
	close(d);
	shmdt(c);
	shmctl(shmid, IPC_RMID, NULL);
}