#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

int isIgnored(char c){
	if(c == ' ' || c == '\t' || c == '\n')
		return 1;
	return 0;
}

int getint(int d){
	char c;
	int ans = 0;
	int readDigit = 0;
	int semn = 1;
	while(read(d, &c, sizeof(char)) > 0){
		if(isIgnored(c)&& !readDigit)
			continue;
		else if(isIgnored(c) && readDigit){
			lseek(d, -1L, SEEK_CUR);
			return ans * semn;
		}
		else{
			if(c >= '0' && c <= '9'){
				if(ans < 214748364)
					ans = ans * 10 + (c - '0');
				else if(ans == 214748364 && c <= '7')
					ans = ans * 10 + (c - '0');
				else{
					errno = ERANGE;
					lseek(d, -1L, SEEK_CUR);
					return 0;
				}
				readDigit++;
			}
			else if(c == '-' || c == '+'){
				if(readDigit != 0){
					errno = 0;
					lseek(d, -1L, SEEK_CUR);
					return ans * semn;
				}
				else{
					if(c == '-')
						semn = -1;
				}
			}
			else 
			{
				if(readDigit != 0){
					errno = 0;
					lseek(d, -1L, SEEK_CUR);
					return ans * semn;
				}
				else{
					errno = EINVAL;
					lseek(d, -1L, SEEK_CUR);
					return 0;
				}
			}
		}
	}
	return ans * semn;
}

int main(int argc, char *args[]){
	if(argc > 1){
	int d = open(args[1], O_RDONLY);
	if(d == -1){
		perror("default.in");
		return -1;
	}
	int x = 1;
	while(x = getint(d))
		printf("%d\n", x);
	close(d);
	}
	return 0;
}
