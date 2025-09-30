#include <func.h>

void copyFile(const char* src, const char* dst);
void copyDir(const char* src, const char* dst);

int main(int argc, char* argv[]) {
    // ./copyDir src dst
	if(argc != 3){
		error(1,0,"Usage: %s src dst",argv[0]);
	}

    copyDir(argv[1], argv[2]);
    return 0;
}

void copyFile(const char* src, const char* dst) {
	// 复制文件 
	FILE* src_fp=fopen(src,"rb");
	if(!src_fp){
		error(1,errno,"fopen");
	}

	FILE* dst_fp=fopen(dst,"wb");
	if(!dst_fp){
		error(1,errno,"fopen");
	}

	char buffer[4096];
	int n;
	while((n = fread(buffer,1,4096,src_fp)) > 0){
		fwrite(buffer,1,n,dst_fp);
	}

	fclose(src_fp);
	fclose(dst_fp);
}

void copyDir(const char* src, const char* dst) {
    // 创建dst目录
	mkdir(dst, 0777);
    // 打开src目录
	DIR* stream = opendir(src);
	if(!stream){
		error(1,errno,"opendir");
	}

    // 遍历目录流
	errno=0;
	struct dirent* curr;
    while((curr = readdir(stream)) != NULL) {
		const char* filename = curr->d_name;

        // 忽略.和..
		if(strcmp(filename, ".") == 0 || strcmp(filename, "..") == 0){
			continue;
		}

		char subpath1[256];
		char subpath2[256];
		sprintf(subpath1,"%s/%s",src,filename);
		sprintf(subpath2,"%s/%s",dst,filename);

        // 如果该目录项是目录，则调用copyDir递归复制
		if(curr->d_type == DT_DIR){
			copyDir(subpath1,subpath2);
		}

        // 如果该目录项是文件，则调用copyFile复制文件
		if(curr->d_type == DT_REG){
			copyFile(subpath1,subpath2);
		}
    }
	// 关闭目录流
	closedir(stream);

	if (errno){
		error(1,errno,"readdir");
	}
}
