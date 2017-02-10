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

void p_jstime(struct jstime j)
{
	printf("%.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", j.y, j.mon, j.day, j.h, j.min, j.sec);
}

void load_jstime(cJSON *j, struct jstime *jt)
{
	cJSON *tmpjs;
	tmpjs = cJSON_GetObjectItem(j, "year");
	jt->y = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "month");
	jt->mon = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "day");
	jt->day = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "hour");
	jt->h = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "minute");
	jt->min = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "second");
	jt->sec = tmpjs->valueint;
	tmpjs = cJSON_GetObjectItem(j, "comment");
	jt->comment = tmpjs->valuestring;
}

int cmp_time(struct tm *tm, struct jstime jt)
{
//	if(jt.sec == tm->tm_sec)
//	{
		if(jt.min == tm->tm_min)
		{
			if(jt.h == tm->tm_hour)
			{
				if(jt.day == tm->tm_mday)
				{
					if(jt.mon == tm->tm_mon+1)
					{
						if(jt.y == tm->tm_year+1900)
						return 0;
					}
				}
			}
		}
//	}
	return -1;
}

void FS_cmptime(FILE_t *f)
{
	struct list_head *pos;
	struct jstime jt;
	CRB *temp;
	time_t t;
	struct tm *tm;
	char *tmp;// = malloc(sizeof(char) * strlen(temp->task));
	
	sleep(1);
	time(&t);
	tm = localtime(&t);	
	printf("%.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", 
		1900+tm->tm_year, 
		tm->tm_mon+1, 
		tm->tm_mday, 
		//tm->tm_hour > 15?(tm->tm_hour - 16):(tm->tm_hour+8), 
		tm->tm_hour,
		tm->tm_min, 
		tm->tm_sec);
		
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		tmp = malloc(sizeof(char) * strlen(temp->task));
		sprintf(tmp, "%s", temp->task);
		cJSON *tmproot = cJSON_Parse(tmp);
		load_jstime(tmproot, &jt);	
		p_jstime(jt);
		if(!cmp_time(tm, jt))
		{
			// EXEC
			printf("Alarm Time![%s]\n", jt.comment);
			FS_del_task(f, jt.comment);
			FS_rewrite(f);
			break;
		}
		free(tmp);
	}
}

int main(argc, argv)
	int argc;
	char *argv[];
{
/*
	time_t t;
	struct tm *tm;
	time(&t);
	tm = localtime(&t);
	printf("%.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", 
			1900+tm->tm_year, 
			tm->tm_mon+1, 
			tm->tm_mday, 
			//tm->tm_hour > 15?(tm->tm_hour - 16):(tm->tm_hour+8), 
			tm->tm_hour,
			tm->tm_min, 
			tm->tm_sec);
*/
	//FILE_t file = { .fp = 0, .patch = "files/cmd", .mode = "rb"};
	//struct jstime jt;
	//FS_open(&file);
	// ---------------------------
	//FS_getlist(&file);
	// char *jstmp = FS_get_task(&file, "Test3");
	// cJSON *tmproot = cJSON_Parse(jstmp);
	// load_jstime(tmproot, &jt);
	// p_jstime(jt);
	
	FILE_t file = { .fp = 0, .patch = "files/cmd", .mode = "rb"};
	FS_open(&file);
	while(1)
	{
		FS_reopen(&file, "rb");
		FS_getlist(&file);
		FS_cmptime(&file);
		if(0)
		{
			FS_close(&file);
		}
	}
	// ---------------------------
	// ---------------------------
	/*
	// printf test
	FS_getlist(&file);
	FS_plist();
	putchar('\n');
	// reopen test
	FS_reopen(&file, "rb");
	FS_getlist(&file);
	FS_plist();
	putchar('\n');
	// rewrite and add test
	FS_rewrite(&file);
	FS_getlist(&file);
	FS_add_task(&file, "{\"year\":2017, \"month\":2, \"day\":10, \"hour\":11, \"minute\":30, \"second\":0, \"comment\":\"Test5\"}\n");
	FS_plist();
	putchar('\n');
	// reopen and delete test
	FS_reopen(&file, "rb");
	FS_getlist(&file);
	FS_del_task(&file, "Test5");
	FS_plist();
	putchar('\n');
	// get task test
	FS_reopen(&file, "rb");
	FS_getlist(&file);
	printf("%s",FS_get_task(&file, "Test3"));
	putchar('\n');
	FS_plist();
	*/
	// ---------------------------
	//FS_close(&file);
	return 0;
}
