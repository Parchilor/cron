#include <time.h>
//#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"
#include "cJSON.h"

int main(argc, argv)
	int argc;
	char *argv[];
{
	/* cron details
	CRON_TIME_t t = {
		.y		= atoi(argv[1]),
		.mon	= atoi(argv[2]),
		.day		= atoi(argv[3]),
		.h		= atoi(argv[4]),
		.min		= atoi(argv[5]),
		.sec		= atoi(argv[6]),
		.tag		= atoi(argv[7]),
		.comment	= argv[8]
	};
	*/
									
	FILE_t file = { .fp = 0, .patch = "files/cmd", .mode = "rb"};
	FS_open(&file);
	FS_getlist(&file);
	CRON_TIME_t *t = JS_init_jstime(atoi(argv[1]), 
									atoi(argv[2]), 
									atoi(argv[3]), 
									atoi(argv[4]), 
									atoi(argv[5]), 
									atoi(argv[6]), 
									//atoi(argv[7]), 
									argv[7]);
	FS_add_task(&file, (char *)JS_modify(JS_encode(t)));
	FS_rewrite(&file);
	FS_close(&file);
	return 0;
}