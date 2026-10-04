#include "dir_explorer.h"

int compare_by_name(const void *a, const void *b) {
	struct file_struct *fileA = (struct file_struct *)a;
	struct file_struct *fileB = (struct file_struct *)b;
	return strcmp(fileA->d_name, fileB->d_name);
}

int flag_l = false;

char* address_comp(){
	char *prompt = getcwd(NULL,0);
	return prompt;
}

void clear_input_buffer() {
	int c;
	while ((c = getchar()) != '\n' && c != EOF){}
}

struct file_struct set_information(const char* filename, struct stat* statbuf) {
	struct file_struct ret;

	snprintf(ret.d_name, sizeof(ret.d_name), "%s", filename);

	if(stat(filename,statbuf) == 0){
		snprintf(ret.permissions, sizeof(ret.permissions), (S_ISDIR(statbuf->st_mode)) ? "d" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IRUSR) ? "r" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IWUSR) ? "w" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IXUSR) ? "x" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IRGRP) ? "r" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IWGRP) ? "w" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IXGRP) ? "x" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IROTH) ? "r" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IWOTH) ? "w" : "-");
		strcat(ret.permissions, (statbuf->st_mode & S_IXOTH) ? "x" : "-");

		ret.n_link = statbuf->st_nlink;
		ret.file_size = statbuf->st_size; 
		ret.tm_info = localtime(&statbuf->st_mtime); // 수정 시간 정보
		strftime(ret.time_str, sizeof(ret.time_str), "%b %d %H:%M", ret.tm_info); // 포맷 설정

		// 사용자 및 그룹 정보 설정
		ret.pw = getpwuid(statbuf->st_uid); // 사용자 정보
		ret.gr = getgrgid(statbuf->st_gid); // 그룹 정보
	} else {
		perror("fail get information");

	}
	return ret;
}

void fill_list_information(struct file_struct* file_list, struct dirent *entry, int index) {
	struct stat statbuf;
	if(stat(entry->d_name, &statbuf) == 0){
		file_list[index] = set_information(entry->d_name, &statbuf);
	}

}

void change_working_dir(char* mv_dir, char* dest) {
	if(chdir(dest)==0){}
	else{
		printf("invalid directory number\n");
	}
}

void print_dir_info(struct file_struct* file_list, int file_cnt) {
	for(int i =0; i<file_cnt; i++){
		if(flag_l){
			/*fill code*/
     printf("[%d] ",i+1);
     printf("%s ",file_list[i].permissions);
     printf("%ld ",file_list[i].n_link);
     
     printf(" %s ", file_list[i].pw ? file_list[i].pw->pw_name : "?");
     printf(" %s ", file_list[i].gr ? file_list[i].gr->gr_name : "?");
     
     printf(" %10lld ",file_list[i].file_size);
     printf("%s ", file_list[i].time_str);
     printf("%s%s%s", COLOR_GREEN, file_list[i].d_name, COLOR_RESET);
     /*
      
     */
     printf("\n");
		}
		else{
			/*fill code*/
			printf("[%d] %s%s%s\n", i + 1,COLOR_GREEN, file_list[i].d_name, COLOR_RESET);
		}
	}
}

void print_reg_file_info(struct file_struct* file_list, int file_cnt) {
	for(int i =0; i<file_cnt; i++){
		if(flag_l){
			/*fill code*/
      printf("[X] ");
     printf("%s ",file_list[i].permissions);
     printf("%ld ",file_list[i].n_link);
     printf(" %s ", file_list[i].pw ? file_list[i].pw->pw_name : "?");
     printf(" %s ", file_list[i].gr ? file_list[i].gr->gr_name : "?");
     printf(" %10lld ",file_list[i].file_size);
     printf("%s ", file_list[i].time_str);
     printf("%s",  file_list[i].d_name);
   
   printf("\n");
   }
		else{
			/*fill code*/
			printf("[X] %s\n", file_list[i].d_name);
		}
	}
}

void run(){
	char *p = address_comp();
	if (p == NULL) { perror("getcwd"); exit(EXIT_FAILURE); }
	printf("%s%s$ %s\n", COLOR_BLUE, p, COLOR_RESET);
	free(p);

	struct file_struct directory_list[1023 + 1];
	struct file_struct reg_file_list[1023 + 1];

	DIR *dir;
	struct dirent *entry;
	char cwd[1024];
	int dir_count = 0;
	int reg_file_count = 0;

	/*
	   fill several codes
	   file_list_information() 함수 활용
	   */
	dir = opendir(".");
	if (dir == NULL) { perror("opendir"); return; }
	while((entry = readdir(dir))!=NULL){
		struct stat statbuf;
		if(stat(entry->d_name, &statbuf)==0){
			if(S_ISDIR(statbuf.st_mode)){
				if (dir_count < 1024)
				    fill_list_information(directory_list, entry, dir_count++);
			}
			else{
				if (reg_file_count < 1024)
				    fill_list_information(reg_file_list, entry, reg_file_count++);
			}
		}

	}

 //printf("dir_count: %d\n", dir_count);
 //printf("reg_file_count: %d\n", reg_file_count);

	qsort(directory_list, dir_count, sizeof(struct file_struct), compare_by_name);
	qsort(reg_file_list, reg_file_count, sizeof(struct file_struct), compare_by_name);

	print_dir_info(directory_list, dir_count);
	print_reg_file_info(reg_file_list, reg_file_count);

	int next_dir_number = 0; 
	printf(">>Enter directory number(cancel: -1, -l option : -2): ");
	int input_result = scanf("%d", &next_dir_number);
	closedir(dir);
	if (input_result == EOF) { exit(0); }
	if (input_result != 1) {
		printf("Invalid input. Please enter a number.\n");
		clear_input_buffer();
		return;
	}
	clear_input_buffer();

	/*
	   fill several code
	   */
	if(next_dir_number == -1){
		exit(0);
	}
	else if(next_dir_number == -2){
		if(flag_l){
			flag_l = false;
			printf("turn off the long list option\n");
		}
		else{
			flag_l = true;
			printf("turn on the long list option\n");
		}
	}
 else{

	if (next_dir_number < 1 || next_dir_number > dir_count) {
		printf("invalid directory number\n");
		return;
	}
	change_working_dir(cwd, directory_list[next_dir_number-1].d_name);
 }
 printf("\n");
}
