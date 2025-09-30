#include <func.h>

void dfs_print(const char* path, int width);	/* width: 缩进的空格数目 */

int directories = 0, files = 0;

int main(int argc, char *argv[]){
	if (argc != 2){
		error(1,0,"Usage: %s dir",argv[0]);
	}

	puts(argv[1]);

	dfs_print(argv[1],4);

	printf("\n%d directories, %d files\n",directories,files);
	return 0;
}

void dfs_print(const char* path, int width){
	DIR* stream = opendir(path);
	if(!stream){
		error(1,errno,"opendir");
	}

	errno = 0;
	struct dirent* curr;
	while((curr=readdir(stream)) != NULL){

		const char* filename=curr->d_name;
		if(filename[0] == '.'){
			continue;
		}

		for(int i=0;i<width;i++){
			putchar(' ');
		}
		puts(filename);

		if(curr->d_type == DT_DIR){
			directories++;

			char subpath[128];
			sprintf(subpath,"%s/%s",path,filename);
			dfs_print(subpath,width+4);
		}
		if(curr->d_type == DT_REG){
			files++;
		}
	}
	if(errno){
		error(1,errno,"readdir");
	}

	closedir(stream);
}
