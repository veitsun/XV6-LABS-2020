/**
 * @file find.c
 * @author your name (you@domain.com)
 * @brief 实现一个简化版的 find： 在目录树中递归查找名字匹配的文件
 * @version 0.1
 * @date 2026-10-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"
#include "kernel/fs.h"




// find <path> <filename>

// 写一个提取文件名的辅助函数

char *basename(char *path) {
	char *p = path + strlen(path);
	while(p > path && *(p - 1) != '/') {
		p --;
	}
	return p;
}

void find(char *path, char *target) {
	int fd = open(path, 0);

	struct stat st;
	fstat(fd, &st);

	if(st.type == T_FILE) {
		// 这个是文件，那么就比名字
		if(strcmp(basename(path), target) == 0) {
			fprintf(1, "%s\n", path);
		}
	}
	else if(st.type == T_DIR) {
		// 递归继续： 是目录，遍历 + 对每个条目递归
		struct dirent de;
		while (read(fd, &de, sizeof(de)) == sizeof(de)) {
			if(de.inum == 0) {
				continue;
			}
			if(strcmp(de.name, ".") == 0 || strcmp(de.name, "..") == 0) {
				continue;
			}

			char newpath[512];
			char *p;
			strcpy(newpath, path);
			// strcat(newpath, "/");
			// strcat(newpath, de.name);
			p = newpath + strlen(newpath);
			*p++ = '/';
			strcpy(p, de.name);

			find(newpath, target);


		}
	}

	close(fd);
}

int main(int argc, char *argv[]) {
	if(argc != 3) {
		fprintf(2, "usage: find <dir> <file>\n");
		exit(1);
	}


	// 必须要先 open 目录拿到文件描述符，才能 read 出里面的目录项。 这是 xv6 的一个特点：
	// 目录在 xv6 里就是个普通文件，内容是一条条 struct dirent 记录
	find(argv[1], argv[2]);
	exit(0);


	
}