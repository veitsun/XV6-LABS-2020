/**
 * @file pingpong.c
 * @author your name (you@domain.com)
 * @brief 父进程和子进程通过 pipe 互相发送一个字节，像打乒乓球一样
 * @version 0.1
 * @date 2026-10-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */



#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"




int main(int argc, char argv[]) {
	int p2c[2];		// parent to child : 0 读， 1 写
	int c2p[2]; 	// child to parent : 0 读， 1 写

	pipe(p2c);  // 创建第一个 pipe
	pipe(c2p);	// 创建第二个 pipe

	int pid = fork();

	if(pid == 0) {
		// 说明这是子进程： 子进程给父进程传递信息 c2p[1] 写， p2c[0] 读
		close(c2p[0]);
		close(p2c[1]);

		char buf;
		read(p2c[0], &buf, 1);
		// printf("%d: received ping\n", getpid());
		fprintf(1, "%d: received ping\n", getpid());
		write(c2p[1], "x", 1);
		
		close(c2p[1]);
		close(p2c[0]);

		exit(0);
	}

	else if(pid > 0){
		// 说明这是父进程：父进程给子进程传递信息 p2c[1] 写， c2p[0] 读
		close(p2c[0]);
		close(c2p[1]);

		write(p2c[1], "x", 1);
		char buf;
		read(c2p[0], &buf, 1);
		// printf("%d: received pong\n", getpid());
		fprintf(1, "%d: received pong\n", getpid());

		exit(0);

	}
	else {
		// fprintf(2, "error: fork fatal");
		exit(1);
	}

	exit(0);
}

