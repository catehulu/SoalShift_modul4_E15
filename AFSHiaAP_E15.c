#define FUSE_USE_VERSION 28
#include <fuse.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <dirent.h>
#include <errno.h>
#include <sys/time.h>
#include <pwd.h>
#include <grp.h>

// static const char *dirpath = "/home/catehulu/Desktop/sisop/soalshit_modul4/realfolder";
static const char *dirpath = "/home/catehulu/shift4";
//--------------------FUNGSI BUATAN------------------------
void decrypt(char* answer){
    char *chiper="qE1~ YMUR2\"`hNIdPzi\%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
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

void encrypt(char* answer){
    char *chiper="qE1~ YMUR2\"`hNIdPzi\%^t@(Ao:=CQ,nx4S[7mHFye#aT6+v)DfKL$r?bkOGB>}!9_wV']jcp5JZ&Xl|\\8s;g<{3.u*W-0";
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

int check_files(const char *fpath,const char *filename){
	char owner1[] = "www-data";
	char owner2[] = "ic_controller";
	char group1[] = "www-data";
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
					printf("\n%s\n\n",fpath);
					char output_path[1000];
					char filemiris[] = "filemiris.txt";
					encrypt(filemiris);
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

//----------------FUNGSI FUSE-----------------------------------------//

static int xmp_getattr(const char *path, struct stat *stbuf)
{
    int res;
	char fpath[1000];
	char newname[1000];
	strcpy(newname,path);
    encrypt(newname);
	sprintf(fpath,"%s%s",dirpath,newname);
	
    // printf("\n get attr %s \n\n",fpath);
	
	res = lstat(fpath, stbuf);

	if (res == -1)
		return -errno;

	return 0;
}
static int xmp_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
		       off_t offset, struct fuse_file_info *fi)
{
  	char fpath[1000];
	encrypt(path);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else sprintf(fpath, "%s%s",dirpath,path);
    // printf("\n readdir %s \n\n",path);
	int res = 0;

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
		printf("\n check path %s | newname %s\n\n",check_path,newname);
		if (!check_files(check_path,newname)) {
			continue;
		}
        decrypt(newname);
		res = (filler(buf, newname, &st, 0));
			if(res!=0) break;
	}

	closedir(dp);
	return 0;
}

static int xmp_read(const char *path, char *buf, size_t size, off_t offset,
		    struct fuse_file_info *fi)
{
  	char fpath[1000];
	char realname[1000];
	strcpy(realname,path);
    encrypt(realname);
    // printf("\n realname %s\n\n",realname);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		sprintf(fpath, "%s%s",dirpath,realname);
	}
    // printf("\n fpath %s\n\n",fpath);
	int res = 0;
  	int fd = 0 ;

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

static int xmp_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	int fd;
	char fpath[1000];
	char realname[1000];
	if (strncmp(path,"/YOUTUBER",9) == 0) {
		mode = 0640;
		strcat(path,".iz1");
	}
	encrypt(path);
	sprintf(fpath, "%s%s",dirpath,path);
	fd = open(fpath, fi->flags, mode);
	if (fd == -1)
		return -errno;

	fi->fh = fd;
	return 0;
}

static int xmp_write(const char *path, const char *buf, size_t size,
		     off_t offset, struct fuse_file_info *fi)
{
	char fpath[1000];
	char realname[1000];
	strcpy(realname,path);
    encrypt(realname);
    // printf("\n realname %s\n\n",realname);
	if(strcmp(path,"/") == 0)
	{
		path=dirpath;
		sprintf(fpath,"%s",path);
	}
	else {
		sprintf(fpath, "%s%s",dirpath,realname);
	}
    // printf("\n fpath %s\n\n",fpath);
	int res = 0;
  	int fd = 0 ;

	(void) fi;
	fd = open(fpath, O_WRONLY);
	if (fd == -1)
		return -errno;

	res = pwrite(fd, buf, size, offset);
	if (res == -1)
		res = -errno;

	close(fd);
	return res;
}

static int xmp_mknod(const char *path, mode_t mode, dev_t rdev)
{
	int res;
	char fpath[1000];
	char realname[1000];
	sprintf(fpath, "%s%s",dirpath,path);

	if (S_ISFIFO(mode))
		res = mkfifo(fpath, mode);
	else
		res = mknod(fpath, mode, rdev);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_utimens(const char *path, const struct timespec ts[2])
{
	int res;
	struct timeval tv[2];
	char fpath[1000];
	char realname[1000];
	sprintf(fpath, "%s%s",dirpath,path);

	tv[0].tv_sec = ts[0].tv_sec;
	tv[0].tv_usec = ts[0].tv_nsec / 1000;
	tv[1].tv_sec = ts[1].tv_sec;
	tv[1].tv_usec = ts[1].tv_nsec / 1000;

	res = utimes(fpath, tv);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_mkdir(const char *path, mode_t mode)
{
	int res;
	char fpath[1000];

	if (strncmp(path,"/YOUTUBER",9) == 0 && strlen(path) != 9) {
		mode = 0750;
	}
	encrypt(path);
	sprintf(fpath, "%s%s",dirpath,path);

	// printf("\n makedir %s \n\n",fpath);

	res = mkdir(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static int xmp_chmod(const char *path, mode_t mode)
{
	int res;
	char fpath[1000];
	char realname[1000];
	int len = strlen(path);
	
	const char *ext = &path[len-4];

	// printf("\nchmod path %s ext %s\n\n",path,ext);

	if (len >= 4 && strcmp(ext,".iz1") == 0) {
		pid_t child_id;
		int show_error;
		child_id = fork();
		if (child_id == 0) {
			char *param_alert[] = {"zenity","--error","--title=\"Pesan error\"","--text=\"File ekstensi iz1 tidak boleh diubah permissionnya.\n\"",NULL};
			execv("/usr/bin/zenity",param_alert);
			return 0;
		}
		return 0;
	}
	
	
	encrypt(path);
	sprintf(fpath, "%s%s",dirpath,path);

	res = chmod(fpath, mode);
	if (res == -1)
		return -errno;

	return 0;
}

static struct fuse_operations xmp_oper = {
	.getattr	= xmp_getattr,
	.readdir	= xmp_readdir,
	.read		= xmp_read,
	.create		= xmp_create,
	.write		= xmp_write,
	.mknod		= xmp_mknod,
	.utimens	= xmp_utimens,
	.mkdir		= xmp_mkdir,
	.chmod		= xmp_chmod,
};



int main(int argc, char *argv[])
{
	umask(0);
	return fuse_main(argc, argv, &xmp_oper, NULL);
}