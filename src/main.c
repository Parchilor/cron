#include <time.h>
//#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"
#include "cJSON.h"

void p_exec(void)
{
	printf("Exec CMD!\n");
}

int main(argc, argv)
	int argc;
	char *argv[];
{
	FILE_t file = { .fp = 0, .patch = "files/cmd", .mode = "rb", .exec_callback = NULL};
	FS_setcallback(&file, p_exec);
	FS_open(&file);
	while(1)
	{
		FS_reopen(&file, "rb");
		FS_getlist(&file);
		FS_cmptime(&file, 1);
		if(0)
		{
			FS_close(&file);
		}
	}
	return 0;
}
