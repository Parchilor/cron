#include <time.h>
//#include <errno.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "fs.h"
#include "cJSON.h"

struct jstime{
	int y;
	int mon;
	int day;
	int h;
	int min;
	int sec;
	char *comment;
};

cJSON *JS_encode(struct jstime jst)
{
	cJSON *tmpjs = cJSON_CreateObject();
	cJSON_AddNumberToObject(tmpjs, "year", jst.y);
	cJSON_AddNumberToObject(tmpjs, "month", jst.mon);
	cJSON_AddNumberToObject(tmpjs, "day", jst.day);
	cJSON_AddNumberToObject(tmpjs, "hour", jst.h);
	cJSON_AddNumberToObject(tmpjs, "minute", jst.min);
	cJSON_AddNumberToObject(tmpjs, "second", jst.sec);
	cJSON_AddStringToObject(tmpjs, "comment", jst.comment);
	return tmpjs;
}

char *JS_modify(cJSON *root)
{
	char *tmp = cJSON_PrintUnformatted(root);
	tmp[strlen(tmp)] = '\n';
	return tmp;
}

int main(argc, argv)
	int argc;
	char *argv[];
{
	// cron details
	struct jstime t = {
		.y		= atoi(argv[1]),
		.mon	= atoi(argv[2]),
		.day		= atoi(argv[3]),
		.h		= atoi(argv[4]),
		.min		= atoi(argv[5]),
		.sec		= atoi(argv[6]),
		.comment	= argv[7]
	};
	
	FILE_t file = { .fp = 0, .patch = "files/cmd", .mode = "rb"};
	FS_open(&file);
	FS_getlist(&file);
	FS_add_task(&file, JS_modify(JS_encode(t)));
	FS_rewrite(&file);
	FS_close(&file);
	return 0;
}