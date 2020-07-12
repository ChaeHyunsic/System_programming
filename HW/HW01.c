#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>
#include <fcntl.h>

void search(char* path);
void transform(char* buff);

int main(int argc, char* argv[]) {
	search(argv[1]);

	return 0;
}

void search(char* path) {
	char *pathname;
	DIR *dp;
	struct dirent *d;
	struct stat buf;
	int length, fp, n;
	char *buff;

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
				}
				else if (S_ISREG(buf.st_mode)) {
					fp = open(pathname, O_RDWR);

					n = lseek(fp, 0, SEEK_END);
					lseek(fp, 0, SEEK_SET);

					buff = (char*)malloc(sizeof(char)*(n + 1));

					read(fp, buff, n);
					buff[n] = '\0';

					transform(buff);

					lseek(fp, 0, SEEK_SET);

					write(fp, buff, n);

					free(buff);
					close(fp);
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

void transform(char *buff) {
	int i = 0;

	while (buff[i] != '\0') {
		buff[i++] ^= 0xFF;
	}
}
