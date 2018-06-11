#ifndef _DIE_H____
#define _DIE_H____

extern void die_usage(const char *, const char *, const char *);
extern void die_temp(const char *, const char *);
extern void die_perm(const char *, const char *);
extern void die_(int);

/* XXX workaround for 'control reaches end of non-void function' */
#define die(x) { die_(x); return x; }

#endif
