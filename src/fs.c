#include <string.h>
#include <stdio.h>
#include <stdlib.h>
//#include "cJSON.h"
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
			FS_add_task(file, strtmp);
		}
	}
}

void FS_del_task(FILE_t *file, char *cmt)
{
	struct list_head *pos;
	CRB *temp;
	list_for_each(pos, &localhead)
	{
		temp = list_entry(pos, CRB, list);
		//printf("%s", temp->task);
		if(strstr(temp->task, cmt) != NULL)
		//if(!strncmp(comment, cmt, strlen(cmt)))
		{
			free(temp->task);
			list_del_init(&temp->list);
			break;
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
	f->fp = freopen(f->patch, mode, f->fp);
	if(f->fp == NULL)
	{
		fputs("FS_open error!\n", stderr);
		exit(1);
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
		//printf("%s", temp->task);
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
