#pragma once
#include "list.h"
#include "cJSON.h"

typedef void (*exec_callback)(void);

typedef struct {
	FILE *fp;
	char *patch;
	char *mode;
	void (*exec_callback)(void);
} FILE_t;

typedef struct jstime{
	int y;
	int mon;
	int day;
	int h;
	int min;
	int sec;
	char *comment;
	int tag;
} CRON_TIME_t;

typedef struct {
	struct list_head list;
	char *task;
	CRON_TIME_t cron;
} CRB;


struct list_head localhead;
struct list_head remotehead;
void FS_open(FILE_t *);
void FS_close(FILE_t *);
char *FS_readline(FILE_t *);

void FS_getlist(FILE_t *);
void FS_add_task(FILE_t *, char *);
void FS_del_task(FILE_t *, char *, int);
char *FS_get_task(FILE_t *, char *);
void FS_reopen(FILE_t *, char *);
void FS_rewrite(FILE_t *);
void FS_plist(void);
void FS_cmptime(FILE_t *, unsigned int);
void p_jstime(CRON_TIME_t);
void load_jstime(cJSON *, CRON_TIME_t *);

cJSON *JS_encode(CRON_TIME_t *);
void JS_decode(cJSON *, CRON_TIME_t *);
CRON_TIME_t *JS_init_jstime(int, int, int, int, int, int, char *);
char *JS_modify(cJSON *);


void Sort_bubble(int *,int);

void FS_setcallback(FILE_t *, exec_callback);