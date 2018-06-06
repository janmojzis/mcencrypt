#ifndef _DIE_H____
#define _DIE_H____

extern void die_usage(const char *, const char *, const char *);
extern void die_temp(const char *, const char *);
extern void die_perm(const char *, const char *);
extern void die(int);

#endif
