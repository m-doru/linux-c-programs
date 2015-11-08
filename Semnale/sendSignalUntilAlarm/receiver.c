#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <setjmp.h>
#define DONE_COUNTING 10
#define CLOCK_SECONDS 4
unsigned long long nr = 0;
sigset_t ms;
sigjmp_buf environment;
struct sigaction act;
pid_t pf;
void hUSR1(int n){
	++nr;
	kill(pf, SIGUSR2);

}
void hAlrm(int n){
	kill(pf, SIGINT);
	siglongjmp(environment, DONE_COUNTING);
}
int main(){
	act.sa_handler = hUSR1;
	sigemptyset(&act.sa_mask);
	sigaddset(&act.sa_mask, SIGALRM);
	act.sa_flags = 0;
	sigaction(SIGUSR1, &act, NULL);
	act.sa_handler = hAlrm;
	sigaction(SIGALRM, &act, NULL);
	sigemptyset(&ms);
	sigaddset(&ms, SIGUSR1);
	sigprocmask(SIG_SETMASK, &ms, NULL);
	
	printf("PID this: %d\n",getpid());
	printf("PID friend:");scanf("%d", &pf);
	sigfillset(&ms);
	sigdelset(&ms, SIGUSR1);sigdelset(&ms, SIGALRM);
	sigsuspend(&ms);
	alarm(CLOCK_SECONDS);
	if(sigsetjmp(environment, 0) != DONE_COUNTING){
		while(1){
			sigsuspend(&ms);
		}
	}		
	printf("Done counting\n");
	sleep(0.5);
	kill(pf, SIGKILL);
	printf("Am primit:%llu\n", nr);
	return 0;
}
