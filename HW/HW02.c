#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void search(char* path);
void permission(char* path);

int main(int argc, char* argv[]) {
	struct stat buf;

	if(!(lstat(argv[1], &buf) < 0)) {
		if(S_ISDIR(buf.st_mode)) {
			search(argv[1]);
			permission(argv[1]);
		}
		else if(S_ISREG(buf.st_mode)) {
			permission(argv[1]);
		}
		else {
			printf("Failed - argument error\n");
		}
	}

	return 0;
}

void search(char* path) {
	char *pathname;
	DIR *dp;
	struct dirent *d;
	struct stat buf;
	int length;

	if ((dp = opendir(path)) != NULL) {
		while (d = readdir(dp)) {
			if ((strcmp(d->d_name, ".") == 0) || (strcmp(d->d_name, "..") == 0))
				continue;
			
			length = strlen(path) + strlen(d->d_name) + 1;
			pathname = (char*)malloc(sizeof(char)*(length + 1));

			strcpy(pathname, path);
			strcat(pathname, "/");
			strcat(pathname, d->d_name);
			pathname[length] = '\0';
	
			if (!(lstat(pathname, &buf) < 0)) {
				if (S_ISDIR(buf.st_mode)) {
					search(pathname);
					permission(pathname);
				}
				else if (S_ISREG(buf.st_mode)) {
					permission(pathname);
				}
				else
					printf("Failed - Unkown mode\n");
			}
			else
				printf("Failed - lstat error\n");
		}
		closedir(dp);
	}
	else
		printf("Failed - open error\n");
}

void permission(char* path) {
	int result = 0;

	if(!(access(path, R_OK) < 0)) {
		result += 4;
	}
	if(!(access(path, W_OK) < 0)) {
		result += 2;
	}
	if(!(access(path, X_OK) < 0)) {
		result += 1;
	}

	printf("%d %s\n", result, path);
}
