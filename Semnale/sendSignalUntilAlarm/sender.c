#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
unsigned long long nr = 0;
sigset_t ms;
pid_t pf;
struct sigaction act;
void hUSR2(int n){
	
}
void hINT(int n){
	printf("Am trimis: %llu\n",nr);
}
int main(){
	act.sa_handler = hUSR2;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	sigaction(SIGUSR2, &act, NULL);
	act.sa_handler = hINT;
	sigaction(SIGINT, &act, NULL);
	sigemptyset(&ms);
	sigaddset(&ms, SIGUSR2);sigaddset(&ms, SIGINT);
	sigprocmask(SIG_SETMASK, &ms, NULL);
	
	printf("PID this: %d\n",getpid());
	printf("PID friend:");scanf("%d", &pf);
	sigfillset(&ms);
	sigdelset(&ms, SIGUSR2);sigdelset(&ms, SIGINT);
	while(1){
		kill(pf, SIGUSR1);
		nr++;
		sigsuspend(&ms);
	}

	return 0;
}
