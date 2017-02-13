#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include "cJSON.h"
#include "list.h"
#include "fs.h"

#define PATCH "files/cmd"

void FS_open(FILE_t *f)
{
	f->fp = fopen(f->patch, f->mode);
	if(f->fp == NULL)
	{
		fputs("FS_open error!\n", stderr);
		exit(1);
	}
}

void FS_close(FILE_t *f)
{
	fclose(f->fp);
	memset(f, 0, sizeof(FILE_t));
}

char *FS_readline(FILE_t *f)
{
	char *tmp, ch;
	
	long set = ftell(f->fp);
	while((ch = fgetc(f->fp)) != EOF)
	{
		if(ch == '\n')
			break;
	}
	long offset = ftell(f->fp) - set;
	
	if(offset == 0)
	{
		return NULL;
	}
	fseek(f->fp, set, SEEK_SET);
	tmp = malloc(sizeof(char) * offset);
	if(tmp == NULL)
	{
		fputs("malloc error!\n", stderr);
		return tmp;
	}
	int ret = fread(tmp, 1, offset, f->fp);
	if(ret != offset)
	{
		fputs("fread error!\n", stderr);
		return NULL;
	}
	return tmp;
}

void FS_add_task(FILE_t *file, char *cmt)
{
	CRB *tmp = malloc(sizeof(CRB));
	INIT_LIST_HEAD(&tmp->list);
	tmp->task = cmt;
	cJSON *jstmp = cJSON_Parse(tmp->task);
	JS_decode(jstmp, &tmp->cron);
	list_add_tail(&tmp->list, &localhead);
}

void FS_getlist(FILE_t *file)
{
	INIT_LIST_HEAD(&localhead);
	char *strtmp;
	while((strtmp = FS_readline(file)) != NULL)
	{
		/*
		CRB *tmp = malloc(sizeof(CRB));
		INIT_LIST_HEAD(&tmp->list);
		tmp->task = strtmp;
		list_add_tail(&tmp->list, &localhead);
		*/
		if(*strtmp == '{')
		{
			//pos = *(strtmp+strlen(strtmp)-2);
			//putchar(pos);
			FS_add_task(file, strtmp);
		}
	}
}

#if 0
CRON_TIME_t *JS_init_jstime(int y, int mon, int day, int h, int min, int sec, char *comment)
{
	CRON_TIME_t *t = malloc(sizeof(struct jstime));
	t->y = y;
	t->mon = mon;
	t->day = day;
	t->h = h;
	t->min = min;
	t->sec = sec;
	t->comment = comment;
	t->tag = 0;
	struct list_head *pos;
	CRB *temp;
	int flag = 0;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		if(temp->cron.comment == NULL)
		{
			continue;
		}
		if(!strncmp(temp->cron.comment, comment, strlen(comment)))
		{
			if(t->tag == temp->cron.tag)
			{
				t->tag++;
			}
		}
	}
	return t;
}
#else
CRON_TIME_t *JS_init_jstime(int y, int mon, int day, int h, int min, int sec, char *comment)
{
	CRON_TIME_t *t = malloc(sizeof(struct jstime));
	t->y = y;
	t->mon = mon;
	t->day = day;
	t->h = h;
	t->min = min;
	t->sec = sec;
	t->comment = comment;
	t->tag = 0;
	struct list_head *pos;
	CRB *temp;
	int cnt = 0;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		if(temp->cron.comment == NULL)
		{
			continue;
		}
		if(!strncmp(temp->cron.comment, comment, strlen(comment)))
		{
			cnt++;
		}
	}
	int tagarr[cnt], index = 0;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		if(temp->cron.comment == NULL)
		{
			continue;
		}
		if(!strncmp(temp->cron.comment, comment, strlen(comment)))
		{
			tagarr[index] = temp->cron.tag;
			index++;
		}
	}
	Sort_bubble(tagarr, cnt);
	for(index = 0; index < cnt; index++)
	{
		t->tag = t->tag < tagarr[index]? t->tag:tagarr[index] +1;
	}
	return t;
}
#endif

void FS_del_task(FILE_t *file, char *cmt, int tag)
{
	struct list_head *pos;
	CRB *temp;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		//printf("%s", temp->task);
		//if(strstr(temp->task, cmt) != NULL)
		cJSON *jstmp = cJSON_Parse(temp->task);
		JS_decode(jstmp, &temp->cron);
		if(!strncmp(temp->cron.comment, cmt, strlen(cmt)))
		{
			if(temp->cron.tag == tag)
			{
				free(temp->task);
				list_del_init(&temp->list);
				break;
			}
		}
	}
}

char *FS_get_task(FILE_t *file, char *cmt)
{
	struct list_head *pos;
	CRB *temp;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		if(strstr(temp->task, cmt) != NULL)
		{
			char *ret = malloc(sizeof(char) * strlen(temp->task));
			memcpy(ret, temp->task, strlen(temp->task));
			return ret;
		}
	}
	return NULL;
}

void FS_reopen(FILE_t *f, char *mode)
{
	if(f->fp == NULL)
	{
		f->fp = fopen(f->patch, mode);
		if(f->fp == NULL)
		{
			fputs("FS_open error!\n", stderr);
			exit(1);
		}
	}
	else
	{
		f->fp = freopen(f->patch, mode, f->fp);
		if(f->fp == NULL)
		{
			fputs("FS_open error!\n", stderr);
			exit(1);
		}
	}
}

void FS_rewrite(FILE_t *f)
{
	f->fp = freopen(f->patch, "wb", f->fp);
	if(f->fp == NULL)
	{
		fputs("FS_rewrite error!\n", stderr);
		exit(1);
	}
	struct list_head *pos;
	CRB *temp;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		printf("%s", temp->task);
		fwrite(temp->task, strlen(temp->task), 1, f->fp);
	}
	f->fp = freopen(f->patch, "rb", f->fp);
}

void FS_plist(void)
{
	struct list_head *pos;
	CRB *temp;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		printf("%s", temp->task);
	}
}

void p_jstime(CRON_TIME_t j)
{
	printf("%.4d/%.2d/%.2d %.2d:%.2d:%.2d\n", j.y, j.mon, j.day, j.h, j.min, j.sec);
}

void load_jstime(cJSON *j, CRON_TIME_t *jt)
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
	tmpjs = NULL;
}

int cmp_time(struct tm *tm, CRON_TIME_t jt)
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

void FS_cmptime(FILE_t *f, unsigned int stime)
{
	struct list_head *pos;
	CRON_TIME_t jt;
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
		JS_decode(tmproot, &jt);	
		p_jstime(jt);
		if(!cmp_time(tm, jt))
		{
			// EXEC
			printf("Alarm Time![%s]\n", jt.comment);
			if(f->exec_callback != NULL)
			{
				f->exec_callback();
			}
			FS_del_task(f, jt.comment, jt.tag);
			FS_rewrite(f);
			break;
		}
		free(tmp);
	}
}


void FS_setcallback(FILE_t *f, exec_callback in)
{
	f->exec_callback = in;
}

cJSON *JS_encode(CRON_TIME_t *jst)
{
	cJSON *tmpjs = cJSON_CreateObject();
	cJSON_AddNumberToObject(tmpjs, "year", jst->y);
	cJSON_AddNumberToObject(tmpjs, "month", jst->mon);
	cJSON_AddNumberToObject(tmpjs, "day", jst->day);
	cJSON_AddNumberToObject(tmpjs, "hour", jst->h);
	cJSON_AddNumberToObject(tmpjs, "minute", jst->min);
	cJSON_AddNumberToObject(tmpjs, "second", jst->sec);
	cJSON_AddStringToObject(tmpjs, "comment", jst->comment);
	cJSON_AddNumberToObject(tmpjs, "tag", jst->tag);//jst->tag
	return tmpjs;
}

void JS_decode(cJSON *j, CRON_TIME_t *jt)
{
	cJSON *tmpjs;
	tmpjs = cJSON_GetObjectItem(j, "year");
	jt->y = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "month");
	jt->mon = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "day");
	jt->day = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "hour");
	jt->h = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "minute");
	jt->min = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "second");
	jt->sec = tmpjs->valueint;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "comment");
	jt->comment = tmpjs->valuestring;
	//cJSON_Delete(tmpjs);
	tmpjs = cJSON_GetObjectItem(j, "tag");
	jt->tag = tmpjs->valueint;
	cJSON_Delete(tmpjs);
}

char *JS_modify(cJSON *root)
{
	char *tmp = cJSON_PrintUnformatted(root);
	tmp[strlen(tmp)] = '\n';
	return tmp;
}

/*
int main(argc, argv)
	int argc;
	char *argv[];
{
	FILE_t file = { .fp = 0, .patch = PATCH, .mode = "rb+"};
	FS_open(&file);
	// ---------------------------
	FS_getlist(&file);
	FS_plist();
	putchar('\n');
	FS_del_task(&file, "two min");
	FS_plist();
	putchar('\n');
	FS_reopen(&file);
	FS_getlist(&file);
	FS_plist();
	//FS_rewrite(&file);
	// ---------------------------
	FS_close(&file);
	return 0;
}
*/
