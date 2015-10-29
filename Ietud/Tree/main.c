#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
int tree(char *dirpath){
	DIR *director;
	static int indentation = 1;
	if((director = opendir(dirpath)) == NULL){perror(dirpath); return -1;}
	char path[256];
	strcpy(path, dirpath);
	if(strlen(path) > 0 && path[strlen(path)-1] != '/')
		strcat(path, "/");
	struct dirent *directorEntry;
	while((directorEntry = readdir(director)) != NULL){
		if(strcmp(directorEntry->d_name, ".") == 0 || strcmp(directorEntry->d_name, "..") == 0)continue;
		struct stat entryStatus;
		char cale[256];
		strcpy(cale, path);
		strcat(cale, directorEntry->d_name);
		if(stat(cale, &entryStatus) == -1){
			perror(path);
			continue;
		}
		if(S_ISDIR(entryStatus.st_mode)){
			int i;
			for(i = 0; i < indentation; ++i)
				printf("  ");
			printf("%s<dir>",directorEntry->d_name);
			printf("\n");
			indentation++;
			tree(cale);
			indentation--;
		}
		else{
			int i;
			for(i = 0; i < indentation; ++i)
				printf("  ");
			printf("%s\n", directorEntry->d_name);
		}

	}
	closedir(director);
	return 0;
}

int main(int argc, char **argv){
	if(argc != 2){fprintf(stderr,"Utilizare: %s director\n", argv[0]); return 41;}
	if(access(argv[1], F_OK) == 0){
		printf("%s\n", argv[1]);
	}
	else{
		perror(argv[1]);
		return 25;
	}
	tree(argv[1]);
	return 0;
}
