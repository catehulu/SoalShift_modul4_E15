#define FUSE_USE_VERSION 28
#include<fuse.h>
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<dirent.h>
#include<time.h>
#include<errno.h>
#include<sys/time.h>
#include<sys/stat.h>
#include<pwd.h>
#include<grp.h>

static const char *dirpath = "/home/catehulu/Desktop/sisop/soalshit_modul4/realfolder";
char *path_to_backup="/home/catehulu/Desktop/sisop/soalshit_modul4/realfolder/XB.Jhu";
char *path_to_recycle_bin="/home/catehulu/Desktop/sisop/soalshit_modul4/realfolder/oO.k.EOX[)";
// static const char *dirpath = "/home/catehulu/shift4";
// char *path_to_backup="/home/catehulu/shift4/XB.Jhu";
// char *path_to_recycle_bin="/home/catehulu/shift4/oO.k.EOX[)";

char *chiper="qE1~ YMUR2\"`hNIdPzi%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";

// Fungsi buat sendiri

void decrypt(char* answer)
{
    int i;
    for(i=0; i<strlen(answer); i++){
        char *pch=strchr(chiper, answer[i]);
        if(pch==NULL) continue;
        else
        {   
            //dekripsi
            if((pch-chiper-17)<0) pch=chiper+((pch-chiper-17+strlen(chiper))%strlen(chiper));
            else pch=chiper+((pch-chiper-17)%strlen(chiper));

            answer[i]=*pch;
        }
    }
    answer[strlen(answer)] = '\0';
}

void encrypt(char* answer)
{
    int i;
    for(i=0; i<strlen(answer); i++){
        char *pch=strchr(chiper, answer[i]);
        if(pch==NULL) continue;
        else
        {    
            //enkripsi
            pch=chiper+(pch-chiper+17)%strlen(chiper);

            answer[i]=*pch;
        }
    }
    answer[strlen(answer)] = '\0';
}

void check_path(char *fpath, char *path)
{
    if(strcmp(path,"/") == 0) sprintf(fpath,"%s",dirpath);
	else sprintf(fpath, "%s%s",dirpath,path);
    // printf("%s %s\n", fpath, path);
}

void copy(char *source, char *dest)
{
	FILE *fptr1, *fptr2; 
  
    // Open one file for reading 
    fptr1 = fopen(source, "r"); 
    if (fptr1 == NULL) 
    { 
        exit(0); 
    } 
  
    // Open another file for writing 
    fptr2 = fopen(dest, "w"); 
    if (fptr2 == NULL) 
    { 
        exit(0); 
    } 
  
    // Read contents from file 
    char c = fgetc(fptr1); 
    while (c != EOF) 
    { 
        fputc(c, fptr2); 
        c = fgetc(fptr1); 
    } 
  
    fclose(fptr1); 
    fclose(fptr2); 
}

int check_files(const char *fpath,const char *filename)
{
	char owner1[] = "chipset";
	char owner2[] = "ic_controller";
	char group1[] = "rusak";
	struct stat per;
	struct passwd *user;
	struct group *group;
	if(stat(fpath,&per) == 0){
        user = getpwuid(per.st_uid);
        group = getgrgid(per.st_gid);
        // printf("%s %s\n", user->pw_name, group->gr_name);
        if((strcmp(user->pw_name,owner1) == 0 || strcmp(user->pw_name,owner2) == 0 ) 
				&& strcmp(group->gr_name,group1) == 0
				&& access(fpath, R_OK) != 0){
					// printf("\n%s\n\n",fpath);
					char output_path[1000];
					char filemiris[] = "filemiris.txt";
					// encrypt(filemiris);
					sprintf(output_path,"%s/%s",dirpath,filemiris);
					FILE *output = fopen(output_path,"a");
					fprintf(output,"----------\n");
					fprintf(output,"nama file : %s\n",filename);
					fprintf(output,"group id : %d\n",per.st_gid);
					fprintf(output,"owner id : %d\n",per.st_uid);
					fprintf(output,"waktu akses : %s",ctime(&per.st_atime));
					fprintf(output,"----------\n");
					remove(fpath);
                    fclose(output);
					return 0;
        }
	}
	return 1;
}

void file_merge(char *path,char *fpath){
	DIR *dp;
	struct dirent *de;
    char filesource[1000];
    char fildest[1000];
    
    strcpy(fildest,fpath);
    strcat(fildest,"/");
    strcat(fildest,path);

    printf("\n path %s\n fpath %s\n\n",path,fpath);

    FILE  *fptr2; 
  
    fptr2 = fopen(fildest, "a"); 
    if (fptr2 == NULL) 
    { 
        exit(0); 
    } 
  
    dp = opendir(dirpath);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        char check_name[1000];
        strcpy(check_name,de->d_name);
        check_name[strlen(check_name) - 4] = '\0';
        printf("check_name %s\n\n",check_name);
        if (strcmp(check_name,path) == 0) {
            FILE *fptr1;

            strcpy(filesource,dirpath);
            strcat(filesource,"/");
            strcat(filesource,de->d_name);
            
            fptr1 = fopen(filesource, "r"); 


            if (fptr1 == NULL) 
            { 
                exit(0); 
            }
            char c = fgetc(fptr1); 
            while (c != EOF) 
            { 
                fputc(c, fptr2); 
                c = fgetc(fptr1); 
            }

            fclose(fptr1); 
            
            printf("\nDone for %s\n\n",filesource);
        }
	}
    fclose(fptr2); 
    closedir(dp);

}

int if_exist(char *path){
    char video[] = "/Videos";
    encrypt(video);
    char fpath[1000];
    char tmp[1000];
    strcpy(tmp,path);
    sprintf(fpath,"%s%s",dirpath,video);
    DIR *dp;
    struct dirent *de;
    tmp[strlen(tmp)-4] = '\0';
    dp = opendir(fpath);
    while((de = readdir(dp)) != NULL){
        // printf("path %s dname %s\n\n",tmp,filename);
        if (strcmp(tmp,de->d_name) == 0) {
            closedir(dp);
            return 1;
        }
        
    }
    return 0;
}

// Fungsi fuse

static int xmp_getattr(const char *path, struct stat *stbuf)
{
	char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);
	
    int res;
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

    int res;
    
    DIR *dp;
	struct dirent *de;
	(void) offset;
	(void) fi;

	dp = opendir(fpath);
	if (dp == NULL)
		return -errno;

	while ((de = readdir(dp)) != NULL) {
		struct stat st;
		char newname[1000];
		char check_path[1000];
		memset(&st, 0, sizeof(st));
		st.st_ino = de->d_ino;
		st.st_mode = de->d_type << 12;
		strcpy(newname,de->d_name);
		strcpy(check_path,fpath);
		strcat(check_path,"/");
		strcat(check_path,newname);
		// printf("\n check path %s | newname %s\n\n",check_path,newname);
		if (!check_files(check_path,newname)) {
			continue;
		}
        if (if_exist(newname)) {
            continue;
        }
        
        decrypt(newname);
		res = (filler(buf, newname, &st, 0));
			if(res!=0) break;
    }

	closedir(dp);
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
			 off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);
    
    int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
    close(fd);
	if (res == -1)
		res = -errno;
    else if(res > 0)
    {
        mkdir(path_to_backup,  0775);

        char tanggal[30];
        time_t waktuku;
        struct tm *t;
        time(&t1);
        t = localtime(&t1);
        sprintf(tanggal, "_%d-%d%d-%d%d_%d%d:%d%d:%d%d", t->tm_year+1900, (t->tm_mon+1)/10, (t->tm_mon+1)%10, t->tm_mday/10, t->tm_mday%10, t->tm_hour/10, t->tm_hour%10, t->tm_min/10, t->tm_min%10, t->tm_sec/10, t->tm_sec%10);
        char eks_file[10];
        strcpy(eks_file, strrchr(nama, '.'));

        sprintf(namafile, "/%s%s", nama, tanggal);
        if(eks_file[0]!=NULL) strcat(namafile, eks_file);

        encrypt(namafile);

        sprintf(path_copy, "%s%s", path_to_backup, namafile);

        copy(fpath, path_copy);
    }
    return res; 
}

static int xmp_truncate(const char *path, off_t size)
{
    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

    int res;
    
    res = truncate(fpath, size);
    if(res == -1)
        return -errno;

    return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

	int res;
	struct timeval tv[2];

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_create(const char* path, mode_t mode, struct fuse_file_info* fi) 
{
    char temp[1000];

    strcpy(temp, path);
    
    if (strstr(temp,"/YOUTUBER")) 
    {
		mode = 0640;
		strcat(temp, ".iz1");
        strcat(path, ".iz1");
    }

    char fpath[1000];
    encrypt(temp);
    check_path(fpath, temp);

    (void) fi;

    int res;
    res = creat(fpath, mode);
    if(res == -1)
	return -errno;

    close(res);

    return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

	int fd;
	int res;

	(void) fi;
	fd = open(fpath, O_RDONLY);
	if (fd == -1)
		return -errno;

	res = pread(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_unlink(const char *path)
{
    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

    char a[100];
    strcpy(a, strrchr(path, '.'));

    if(strcmp(a, ".swp")!=0)
    {
        char tanggal[30];
        time_t waktuku;
        struct tm *t;
        time(&t1);
        t = localtime(&t1);
        sprintf(tanggal, "_deleted_%d-%d%d-%d%d_%d%d:%d%d:%d%d", t->tm_year+1900, (t->tm_mon+1)/10, (t->tm_mon+1)%10, t->tm_mday/10, t->tm_mday%10, t->tm_hour/10, t->tm_hour%10, t->tm_min/10, t->tm_min%10, t->tm_sec/10, t->tm_sec%10);
        encrypt(tanggal);

        char nama[1000];
        strcat(nama, strrchr(path, '/')+2);
        encrypt(nama);
        strcat(nama, "*");

        char namazip[100],path_zip[1000];
        strncpy(namazip, path, strlen(path)-strlen(a));
        encrypt(namazip);

        sprintf(path_zip, "%s%s%s", path_to_recycle_bin, namazip, tanggal);

        pid_t zip;
        int status;
        zip=fork();

        if(zip==0)
        {
            char *agr[]={"zip", path_zip, nama};
            execv("/usr/bin/zip", arg);
        }
        else
        {
            while((wait(&status))>0);
        }
        
    }

	int res;

	res = unlink(fpath);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
    if (strncmp(path,"/YOUTUBER",9) == 0 && strlen(path) != 9) {
		mode = 0750;
    }

    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

	int res;

	res = mkdir(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
    int len = strlen(path);
    const char *ext = &path[len-4];

    if (len >= 4 && strcmp(ext,".iz1") == 0) {
		pid_t child_id;
		child_id = fork();
		if (child_id == 0) {
			char *param_alert[] = {"zenity","--error","--title=\"Pesan error\"","--text=\"File ekstensi iz1 tidak boleh diubah permissionnya.\n\"",NULL};
			execv("/usr/bin/zenity",param_alert);
			return 0;
		}
		return 0;
    }

    char fpath[1000];
    char temp[1000];
    strcpy(temp, path);
    encrypt(temp);
    check_path(fpath, temp);

	int res;

	res = chmod(path, mode);
	if (res == -1)
		return -errno;

	return 0;
}

void* xmp_init(struct fuse_conn_info *conn){
	char path[100] = "Videos";
	char fpath[1000];
	DIR *dp;
	struct dirent *de;
	encrypt(path);
	sprintf(fpath,"%s/%s",dirpath,path);
	mkdir(fpath,0775);

    dp = opendir(dirpath);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        char filename[1000];
        strcpy(filename,de->d_name);
        decrypt(filename);
        int len = strlen(filename);
        const char *ext = &filename[len-4];
        if (strcmp(".000",ext) == 0) {
            pid_t child_id;
            child_id = fork();
            encrypt(filename);
            if (child_id == 0) {
        		char merged_file[1000];
                strncpy(merged_file,filename,len-4);
                file_merge(merged_file,fpath);
                exit(0);
            }
            
        }

	}

}

void xmp_destroy(void* private_data){
	DIR *dp;
	struct dirent *de;

	char path[100] = "Videos";
	char fpath[1000];
	encrypt(path);
	sprintf(fpath,"%s/%s",dirpath,path);
	
	dp = opendir(fpath);
	if (dp == NULL)
		return;

	while ((de = readdir(dp)) != NULL) {
        int len = strlen(de->d_name);
        const char *number = &path[len-4];
		char remove_path[1000];
		strcpy(remove_path,fpath);
		strcat(remove_path,"/");
		strcat(remove_path,de->d_name);
		remove(remove_path);
	}

	rmdir(fpath);

	closedir(dp);
}

static struct fuse_operations xmp_oper = {
    .getattr	= xmp_getattr,
    .readdir	= xmp_readdir,
    .write      = xmp_write,
    .truncate   = xmp_truncate,
    .utimens    = xmp_utimens,
    .create     = xmp_create,
    .read       = xmp_read,
    .unlink     = xmp_unlink,
    .mkdir      = xmp_mkdir,
    .chmod      = xmp_chmod,
    .init       = xmp_init,
    .destroy    = xmp_destroy,
};

int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}
