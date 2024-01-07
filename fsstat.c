#define _POSIX_C_SOURCE 200809L
#include <grp.h>
#include <pwd.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>

#define PARSEOPT(opt, act)\
	if (!strcmp(argv[i], "-" opt)) {\
		act;\
		continue;\
	}
#define PARSEITOPT(opt, itval) PARSEOPT(opt, it = itval);
#define PARSEFLOPT(opt, flval) PARSEOPT(opt, fl = flval);

enum {ItNone, ItType, ItSize, ItHSize, ItPerms, ItOPerms, ItUsr, ItUID, ItGrp,
      ItGID, ItMDate};

void die(char *fmt, ...);
void fsstat(char *path);
void statgrp(char *path, struct stat *s);
void stathsize(struct stat *s);
void statmdate(struct stat *s);
void statperms(struct stat *s);
void stattype(struct stat *s);
void statusr(char *path, struct stat *s);

int it = ItNone;
int fl = 0;

void die(char *fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	fprintf(stderr, "fsstat: ");
	vfprintf(stderr, fmt, args);
	va_end(args);
	exit(1);
}

void fsstat(char *path)
{
	struct stat s;
	if (fl ? stat(path, &s) : lstat(path, &s))
		die("can't stat \"%s\".\n", path);
	if (!it)
		die("missing an info type to stat \"%s\".\n", path);
	if (it == ItType)
		stattype(&s);
	else if (it == ItSize)
		printf("%ld\n", s.st_size);
	else if (it == ItHSize)
		stathsize(&s);
	else if (it == ItPerms)
		statperms(&s);
	else if (it == ItOPerms)
		printf("%o\n", s.st_mode & (S_IRUSR | S_IWUSR | S_IXUSR |
					    S_IRGRP | S_IWGRP | S_IXGRP |
					    S_IROTH | S_IWOTH | S_IXOTH));
	else if (it == ItUsr)
		statusr(path, &s);
	else if (it == ItUID)
		printf("%u\n", s.st_uid);
	else if (it == ItGrp)
		statgrp(path, &s);
	else if (it == ItGID)
		printf("%u\n", s.st_gid);
	else if (it == ItMDate)
		statmdate(&s);
}

void statgrp(char *path, struct stat *s)
{
	struct group *g;
	if (!(g = getgrgid(s->st_gid)))
		die("can't find group that owns \"%s\".\n", path);
	printf("%s\n", g->gr_name);
}

void stathsize(struct stat *s)
{
	char pref[] = {'G', 'M', 'k'};
	float mult[] = {1e9, 1e6, 1e3};
	float size;
	int i;
	for (i = 0; i < 3; i++)
		if ((size = s->st_size / mult[i]) >= 1) {
			printf("%.1f%cB\n", size, pref[i]);
			return;
		}
	printf("%ldB\n", s->st_size);
}

void statmdate(struct stat *s)
{
	char buf[29];
	strftime(buf, sizeof(buf), "%a %b %d %T %Z %Y", localtime(&s->st_mtime));
	printf("%s\n", buf);
}

void statperms(struct stat *s)
{
	char chars[] = {'r', 'w', 'x'};
	int flags[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP,
	               S_IROTH, S_IWOTH, S_IXOTH};
	int i;
	for (i = 0; i < 9; i++)
		putchar(s->st_mode & flags[i] ? chars[i < 3 ? i : (i - 3) % 3] :
		        '-');
	putchar('\n');
}

void stattype(struct stat *s)
{
	printf("%c\n", S_ISREG(s->st_mode) ? 'r' : S_ISDIR(s->st_mode) ? 'd' :
	       S_ISLNK(s->st_mode) ? 'l' : S_ISCHR(s->st_mode) ? 'c' :
	       S_ISBLK(s->st_mode) ? 'b' : S_ISFIFO(s->st_mode) ? 'f' : 's');
}

void statusr(char *path, struct stat *s)
{
	struct passwd *p;
	if (!(p = getpwuid(s->st_uid)))
		die("can't find user that owns \"%s\".\n", path);
	printf("%s\n", p->pw_name);
}

int main(int argc, char **argv)
{
	int i;
	for (i = 1; i < argc; i++) {
		PARSEITOPT("t", ItType);
		PARSEITOPT("s", ItSize);
		PARSEITOPT("hs", ItHSize);
		PARSEITOPT("p", ItPerms);
		PARSEITOPT("op", ItOPerms);
		PARSEITOPT("u", ItUsr);
		PARSEITOPT("ui", ItUID);
		PARSEITOPT("g", ItGrp);
		PARSEITOPT("gi", ItGID);
		PARSEITOPT("md", ItMDate);
		PARSEFLOPT("fl", 1);
		PARSEFLOPT("ul", 0);
		fsstat(argv[i]);
	}
	return 0;
}
