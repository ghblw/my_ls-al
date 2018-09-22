/*************************************************************************
	> File Name: my_ls-al.c
	> Author:yuxiaowei 
	> Mail:1147407739@qq.com 
	> Created Time: 2018年09月22日 星期六 16时36分40秒
 ************************************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>
#include<time.h>
#include<dirent.h>
#include<string.h>

char *uid(uid_t uid) {
    return getpwuid(uid)->pw_name;//获取user
}
char *gid(gid_t gid) {
    return getgrgid(gid)->gr_name;//获取group
}
char *ch_time(time_t *timep) {
    return ctime(timep);//转换时间
}
void inf(char *file) { 
    struct stat st;
    stat(file, &st);
    char mod[20];
    strcpy(mod,"----------");
    if ((st.st_mode & S_IFMT) == S_IFDIR) {
        mod[0]='d';//判断是否为目录类型
    }
    //文件所有者
    if (st.st_mode & S_IRUSR) mod[1] = 'r';
    if (st.st_mode & S_IWUSR) mod[2] = 'w';
    if (st.st_mode & S_IXUSR) mod[3] = 'x';
    //用户组
    if (st.st_mode & S_IRGRP) mod[4] = 'r';
    if (st.st_mode & S_IWGRP) mod[5] = 'w';
    if (st.st_mode & S_IXGRP) mod[6] = 'x';
    //其他组
    if (st.st_mode & S_IROTH) mod[7] = 'r';
    if (st.st_mode & S_IWOTH) mod[8] = 'w';
    if (st.st_mode & S_IXOTH) mod[9] = 'x';
    printf("%s ", mod);
    printf("%3d ",st.st_nlink);
    printf("%s ",uid(st.st_uid));
    printf("%s ",gid(st.st_gid));
    printf("%d ",st.st_size);
    char *p = ch_time(&st.st_ctime)+4;//ctime会自动换行，处理一下
    for (int i = 0; *p != '\n'; i++) {
        printf("%c",*p);
        p++;
    }
    printf(" ");
    if(mod[0] == 'd') {
        //蓝色加粗
        printf("\033[1;34m%s\033[0m\n",file);
    } else if (mod[9] == 'x'){
        //绿色加粗
        printf("\033[1;32m%s\033[0m\n",file);
    } else {
        printf("%s\n",file);
    }
}
void ls(char *dir) {
    struct dirent *drtp;
    DIR *dp = opendir(dir);//目录指针
    while (drtp = readdir(dp)) {
        inf(drtp -> d_name);//获取目录下每个文件的信息
    }
    closedir(dp);
}
int main(int argc, char *argv[]) {
    if (argc == 1) ls(".");//列出当前目录下文件
    else {
        ++argv;
        ls(*argv);
    }
    return 0;
}
