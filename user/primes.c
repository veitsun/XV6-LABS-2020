/**
 * @file primes.c
 * @author your name (you@domain.com)
 * @brief 实现一个素数筛
 * @version 0.1
 * @date 2026-10-18
 * 
 * @copyright Copyright (c) 2026
 * 
 */


#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

// 每个进程：
// 1 从左邻居读数字
// 2 第一个读到的数一定是素数，打印它
// 3 筛掉这个素数的倍数， 把剩下的传给右邻居
void sieve(int left_fd) {
	int prime;

	if(read(left_fd, &prime, sizeof(prime)) == 0) {
		close(left_fd);
		exit(0);
	}

	fprintf(2, "prime %d\n", prime);

	int p2c[2];
	pipe(p2c);

	int pid = fork();

	if(pid == 0) {
		close(p2c[1]);
		close(left_fd);

		sieve(p2c[0]);
		exit(0);
	}
	else {
		// 父进程
		close(p2c[0]);

		int n;
		while(read(left_fd, &n, sizeof(n)) > 0) {
			//
			if(n % prime != 0) {
				write(p2c[1], &n, sizeof(n));
			}
		}

		close(p2c[1]);
		close(left_fd);
		wait(0);
		exit(0);
	}
	
}

int main(int argc, char *argv[]) {

	int p2c[2];  // p2c[0] 和 p2c[1] 是 int 类型的文件描述符， 他们只是索引/句柄， 指向内核中的 pipe 对象

	pipe(p2c);


	int pid = fork();

	if(pid == 0) {
		close(p2c[1]);
		// 子进程，成为右邻居
		// int n;
		sieve(p2c[0]);
		exit(0);
	}
	else {
		close(p2c[0]);
		// 父进程，继续当前工作
		// 写 2 ~ 35 到 p[1], 关闭写端， 
		for(int i = 2; i <= 35; i++) {
			write(p2c[1], &i, sizeof(i));
		}
		close(p2c[1]);


		wait(0); // 等待子进程结束
		exit(0);
	}

}