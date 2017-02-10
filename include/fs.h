#pragma once
#include "list.h"
typedef struct {
	FILE *fp;
	char *patch;
	char *mode;
} FILE_t;

typedef struct {
	struct list_head list;
	char *task;
} CRB;

struct list_head localhead;
struct list_head remotehead;
void FS_open(FILE_t *);
void FS_close(FILE_t *);
char *FS_readline(FILE_t *);

void FS_getlist(FILE_t *);
void FS_add_task(FILE_t *, char *);
void FS_del_task(FILE_t *, char *);
char *FS_get_task(FILE_t *, char *);
void FS_reopen(FILE_t *, char *);
void FS_rewrite(FILE_t *);
void FS_plist(void);
