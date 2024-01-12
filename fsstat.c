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
#define PARSEINFOOPT(opt, i) PARSEOPT(opt, info_g = i);
#define PARSESYMLINKOPT(opt, f) PARSEOPT(opt, isfollow_g = f);

enum {InfoNone, InfoType, InfoSize, InfoHSize, InfoPerms, InfoOPerms, InfoUsr,
      InfoUID, InfoGrp, InfoGID, InfoMDate};

void die(char *fmt, ...);
void fsstat(char *path);
void statgrp(char *path, struct stat *s);
void stathsize(struct stat *s);
void statmdate(struct stat *s);
void statperms(struct stat *s);
void stattype(struct stat *s);
void statusr(char *path, struct stat *s);

int info_g = InfoNone;
int isfollow_g = 0;

void die(char *fmt, ...) {
  va_list args;
  va_start(args, fmt);
  fprintf(stderr, "fsstat: ");
  vfprintf(stderr, fmt, args);
  va_end(args);
  exit(1);
}

void fsstat(char *path) {
  struct stat s;
  if (isfollow_g ? stat(path, &s) : lstat(path, &s)) {
    die("can't stat \"%s\".\n", path);
  } else if (!info_g) {
    die("missing an info type to stat \"%s\".\n", path);
  } else if (info_g == InfoType) {
    stattype(&s);
  } else if (info_g == InfoSize) {
    printf("%ld\n", s.st_size);
  } else if (info_g == InfoHSize) {
    stathsize(&s);
  } else if (info_g == InfoPerms) {
    statperms(&s);
  } else if (info_g == InfoOPerms) {
    printf("%o\n", s.st_mode & (S_IRUSR | S_IWUSR | S_IXUSR | S_IRGRP |
                                S_IWGRP | S_IXGRP | S_IROTH | S_IWOTH |
                                S_IXOTH));
  } else if (info_g == InfoUsr) {
    statusr(path, &s);
  } else if (info_g == InfoUID) {
    printf("%u\n", s.st_uid);
  } else if (info_g == InfoGrp) {
    statgrp(path, &s);
  } else if (info_g == InfoGID) {
    printf("%u\n", s.st_gid);
  } else if (info_g == InfoMDate) {
    statmdate(&s);
  }
}

void statgrp(char *path, struct stat *s) {
  struct group *g;
  if (!(g = getgrgid(s->st_gid))) {
    die("can't find group that owns \"%s\".\n", path);
  }
  printf("%s\n", g->gr_name);
}

void stathsize(struct stat *s) {
  char pref[] = {'G', 'M', 'k'};
  float mult[] = {1e9, 1e6, 1e3};
  float size;
  int i;
  for (i = 0; i < 3; i++) {
    if ((size = s->st_size / mult[i]) >= 1) {
      printf("%.1f%cB\n", size, pref[i]);
      return;
    }
  }
  printf("%ldB\n", s->st_size);
}

void statmdate(struct stat *s) {
  char buf[29];
  strftime(buf, sizeof(buf), "%a %b %d %T %Z %Y", localtime(&s->st_mtime));
  printf("%s\n", buf);
}

void statperms(struct stat *s) {
  char chars[] = {'r', 'w', 'x'};
  int flags[] = {S_IRUSR, S_IWUSR, S_IXUSR, S_IRGRP, S_IWGRP, S_IXGRP, S_IROTH,
                 S_IWOTH, S_IXOTH};
  int i;
  for (i = 0; i < 9; i++) {
    putchar(s->st_mode & flags[i] ? chars[i < 3 ? i : (i - 3) % 3] : '-');
  }
  putchar('\n');
}

void stattype(struct stat *s) {
  printf("%c\n", S_ISREG(s->st_mode) ? 'r' : S_ISDIR(s->st_mode) ? 'd' :
         S_ISLNK(s->st_mode) ? 'l' : S_ISCHR(s->st_mode) ? 'c' :
         S_ISBLK(s->st_mode) ? 'b' : S_ISFIFO(s->st_mode) ? 'f' : 's');
}

void statusr(char *path, struct stat *s) {
  struct passwd *p;
  if (!(p = getpwuid(s->st_uid))) {
    die("can't find user that owns \"%s\".\n", path);
  }
  printf("%s\n", p->pw_name);
}

int main(int argc, char **argv) {
  int i;
  for (i = 1; i < argc; i++) {
    PARSEINFOOPT("t", InfoType);
    PARSEINFOOPT("s", InfoSize);
    PARSEINFOOPT("hs", InfoHSize);
    PARSEINFOOPT("p", InfoPerms);
    PARSEINFOOPT("op", InfoOPerms);
    PARSEINFOOPT("u", InfoUsr);
    PARSEINFOOPT("ui", InfoUID);
    PARSEINFOOPT("g", InfoGrp);
    PARSEINFOOPT("gi", InfoGID);
    PARSEINFOOPT("md", InfoMDate);
    PARSESYMLINKOPT("fl", 1);
    PARSESYMLINKOPT("ul", 0);
    fsstat(argv[i]);
  }
  return 0;
}
