#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/dir.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>
#include <grp.h>
#include <time.h>


void printAllInfo(struct stat*,struct passwd*,struct group*,struct dirent*);
void printPerms(mode_t _mode);
void printFileType(mode_t _mode);

int main()
{
    DIR *curr_dir;
    struct dirent *dir;
    curr_dir = opendir(".");

    if (curr_dir) {
        struct stat stat_buf;
        struct passwd pswd;
        struct group usr_gr;
        while ((dir = readdir(curr_dir)) != NULL)
        {	
            printAllInfo(&stat_buf, &pswd, &usr_gr,dir);
        }

        closedir(curr_dir);
    }
    return 0;
}

void printAllInfo(struct stat *stat_buf, struct passwd *pswd, struct group *usr_gr, struct dirent *dir)
{
    lstat(dir->d_name, stat_buf);
    pswd = getpwuid(stat_buf->st_uid);
    usr_gr = getgrgid(stat_buf->st_gid);
    printFileType(stat_buf->st_mode);
    printPerms(stat_buf->st_mode);
    printf(" %u %s %s %5d %s %s\n", (unsigned short)stat_buf->st_nlink, pswd->pw_name, usr_gr->gr_name, 
                                    (int)stat_buf->st_size, ctime(&stat_buf->st_mtime), dir->d_name);
}

void printFileType(mode_t _mode)
{
    switch (_mode & S_IFMT)
    {
    case S_IFREG:
        printf("f");
        break;
    case S_IFBLK:
        printf("b");
        break;
    case S_IFCHR:
        printf("c");
        break;
    case S_IFDIR:
        printf("d");
        break;
    case S_IFIFO:
        printf("p");
        break;
    case S_IFLNK:
        printf("l");
        break;
    case S_IFSOCK:
        printf("s");
        break;
    default:
        printf("-");
        break;
    }
}

void printPerms(mode_t _mode)
{
    if(_mode & 0400)
        printf("r");
    else
        printf("-");
    if(_mode & 0200)
        printf("w");
    else
        printf("-");
    if(_mode & 0100)
        printf("x");
    else
        printf("-");
    if(_mode & 0040)
        printf("r");
    else
        printf("-");
    if(_mode & 0020)
        printf("w");
    else
        printf("-");
    if(_mode & 0010)
        printf("x");
    else
        printf("-");
    if(_mode & 0004)
        printf("r");
    else
        printf("-");
    if(_mode & 0002)
        printf("w");
    else
        printf("-");
    if(_mode & 0001)
        printf("x");
    else
        printf("-");
}
