/**
 * @file sleep.c
 * @author your name (you@domain.com)
 * @brief 实现一个 sleep 程序，暂停指定的 tick 数 （Xv6 内核定义的时间单位）， 然后退出
 * @version 0.1
 * @date 2026-10-15
 * 
 * @copyright Copyright (c) 2026
 * 
 */

#include "kernel/types.h"
#include "kernel/stat.h"
#include "user.h"

int main(int argc, char *argv[]) {

	if(argc != 2) {
		fprintf(2, "usage: sleep ticks\n");
		exit(1);
	}

	uint ticks = atoi(argv[1]);
	sleep(ticks);

	exit(0);
}