#include <func.h>

void deleteDir(const char* path);

int main(int argc ,char* argv[]){
	if(argc != 2){
		error(1,0,"Usage: %s src",argv[0]);
	}

	deleteDir(argv[1]);
	return 0;
}

void deleteDir(const char* path){
	DIR* stream = opendir(path);
	if(!stream){
		error(1,errno,"opendir");
	}

	errno=0;
	struct dirent* curr;
	while((curr = readdir(stream)) != NULL){
		const char* filename = curr->d_name;
		
		if(filename[0] == '.'){
			continue;
		}

		if(curr->d_type == DT_DIR){
			char subpath[1024];
			sprintf(subpath, "%s/%s", path, filename);
			deleteDir(subpath);
		}else if(curr->d_type == DT_REG){
			char subpath[1024];
			sprintf(subpath, "%s/%s", path, filename);
			unlink(subpath);
		}
	}

	if(rmdir(path) == -1){
		error(1,errno,"rmdir");
	}

	if(errno){
		error(1,errno,"readdir");
	}

	closedir(stream);
}
