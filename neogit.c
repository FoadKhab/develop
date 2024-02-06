#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include<sys/stat.h>
#include<unistd.h>
#include <ctype.h>
#include<dirent.h>
#include<time.h>
struct commit{
    char date[100];
    char message[100];
    int id;
    char branch[100];
    char author[100];
    int num_of_file; 
    long long seconds_1970;
    long long last_commit_id;
    char email[100];
};
struct tag{

    struct commit tag_commit;
    char message[100];
    char name[100];
    char tagger[100];
};


#define max_ino_code 50
#define max_path_size 100
#define max_command_size 100
#define max_message_size 72
#define max_date_size 50
#define max 100
int last_commit;
char branch_name[max_ino_code];
char neogit_folder_location[max_path_size];
char head_or_not[max];
char local_or_general[max];
char user_name[max];
char user_email[max];
int neogit_found_or_not;
int run_init();
int check_exist_neogit_folder(char *);
void filling_timeline(char *);
void add_to_timeline(char * , char *);
int run_add(char *argv[] , int argc);
void add_name_to_list(char *);
int check_add(char*);
void copy_file_to_staged(char*);
int normal_add_operation(char * , int);
void add_name_to_tracked(char *cwd);
int multi_add_operation(char* arc[] , int);
void show_staged_or_not(int);
void search_stagedfiles(int n);
int wildcard_add_operation(char *);
int run_reset(char *argv[] , int argc);
int normal_reset_operation(char *file_path , int mode);
void remove_name_from_list(char * path);
void del_file_from_staged(char * path);
void add_to_unstage_list(char *path);
int wildcard_reset_operation(char *file_name);
int multi_reset_operation(char *argv[] , int argc);
int undo_reset_operation();
int check_status_M(char *cwd);
void check_status_A(char * cwd);
void commit_operation_without_space(char *argv[]);
void commit_operation_with_space(char *argv[] , int args);
int run_status();
void delete_files(char *folder_b);
void copy_dir(char *srcpath, char *destpath);
void copy_files(char *srcpath, char *destpath);
int run_remove(char *argv[] , int argc);
int run_replace(char *argv[] , int argc);
int run_set(char *argv[] , int argc);
void commit_operation_with_shortcut(char *argv[] , int argc);
void commit_operation_with_space(char *argv[] , int args);
void commit_operation_with_space(char *argv[] , int args);
void commit_operation_without_space(char *argv[]);
int run_commit(char *agrv[] , int argc);
void print_commit_log(struct commit commit_info);
void normal_log();
int run_log(char *argv[] , int argc);
void since_before_log(char *argv[] , int mode);
void branch_author_log(char *branch_author , int mode);
void n_log(char * num);
int run_branch(char *argv[] , int argc);
void finding_neogit_path();
int is_staging_area_empty();

void checkout_commit(char *commit_name);
int run_checkout(char *argv[] , int argc);
void checkout_branch(char *bran);
int run_config(char *argv[], int argc);
int wildcardMatchWord(const char *pattern, const char *word);
int wildcardMatchSentence(const char *pattern, const char *sentence); 
void wildcard_log(char * str);
int multi_search(char*argv[] , int argc);
void normal_tag(struct tag * all_tags , int count);
// find_tag(all_tags , count , argv[3]);
void find_tag(struct tag * all_tags , int count , char *name);
int redo_addition(char *argv[] , int argc);
int run_tag(char *argv[],int  argc);
int main(int argc, char const *argv[]){
    // int argc;
    // scanf("%d" , &argc);
    // char argv[50][50];
    // for(int i=0 ;i<argc ;i++){
        // scanf("%s" , argv[i]);
    // }
    
    if(strcmp(argv[1] , "init") == 0) return run_init();
    else if(strcmp(argv[1] , "config") == 0) return run_config(argv , argc);
    else if(strcmp(argv[1] , "add")==0) return run_add(argv , argc);
    else if(strcmp(argv[1] , "reset") == 0) return run_reset(argv , argc);
    else if(strcmp(argv[1] , "status") == 0) return run_status(argv , argc);
    else if(strcmp(argv[1] , "commit") == 0) return run_commit(argv , argc);
    else if(strcmp(argv[1] , "set") == 0) return run_set(argv , argc);
    else if(strcmp(argv[1] , "replace") == 0) return run_replace(argv , argc);
    else if(strcmp(argv[1] , "remove") == 0) return run_remove(argv ,argc);
    else if(strcmp(argv[1] , "log") == 0 ) return  run_log(argv , argc);
    else if(strcmp(argv[1] , "branch") == 0) return run_branch(argv , argc);
    else if(strcmp(argv[1] , "checkout") == 0) return run_checkout(argv , argc);
    else if(strcmp(argv[1] , "tag") == 0) return run_tag(argv, argc);
    else if(strcmp(argv[1] , "grep") == 0) return run_grep(argv, argc);
}


void finding_neogit_path(){
    char cwd[max];
    getcwd(cwd, sizeof(cwd));
    // prisntf("%s\n" ,cwd);
    if(strlen(cwd) == 3){
        printf("No neogit folder found!\n");
        return;
    }
    system("echo. > temp11.txt");
    system("dir /B > temp11.txt");
    FILE* fp = fopen("temp11.txt","r");
    char line[max_path_size];
    int n = 0;
    while (fgets(line , max_path_size , fp))
    {
        int len = strlen(line);
        if(line[len-1] == '\n') line[len-1] = '\0';
        if(strcmp(line , ".neogit") == 0){
            // neogit exists
            // printf("saak\n");
            n = 1;
            strcpy(neogit_folder_location , cwd);
            // printf("%s\n" , neogit_folder_location);
            fclose(fp);
            system("del temp11.txt");
            return ;
        }
    }
    if(n==0){
        chdir("..");
        finding_neogit_path();
        chdir(cwd);
    }
    fclose(fp);
    system("del temp11.txt");
    return ;
}
// config functions 
int run_config(char *argv[] , int argc){
    finding_neogit_path();
    char path[max];
    snprintf(path , max , "%s\\.neogit\\branch_name.txt" , neogit_folder_location);
    FILE *fil = fopen(path , "r");
    fgets(branch_name , max_path_size , fil);
    fclose(fil);
    char path2[max];
    snprintf(path2 , max , "%s\\.neogit\\last_commit.txt" , neogit_folder_location);
    FILE *fp = fopen(path , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);
    
    
    
    if(strcmp("-global" , argv[2]) == 0){
        //username
        
        if(strcmp("user.name" , argv[3]) == 0){
            char command[max];
            snprintf(command , max , "C:\\.neogit_info\\project_locations.txt");
            FILE *file = fopen(command , "r");
            char line[max];
            // fgets(line , max ,file);
    printf("oomad");
            while(fgets(line , max ,file)){
                int n = strlen(line);
                if(line[n-1] == '\n') line[n-1] = '\0';
                strcat(line , "\\.neogit\\local_username.txt");
                FILE *name = fopen(line , "w");
                fprintf(name , "%s" , argv[4]);
                fclose(name);
            }
            fclose(file);
        }
        if(strcmp("user.email" , argv[3]) == 0){
            char command[max];
            snprintf(command , max , "C:\\.neogit_info\\project_locations.txt");
            FILE *file = fopen(command , "r");
            char line[max];
            fgets(line , max ,file);
            while(fgets(line , max ,file)){
                int n = strlen(line);
                if(line[n-1] == '\n') line[n-1] = '\0';
                strcat(line , "\\.neogit\\local_useremail.txt");
                FILE *name = fopen(line , "w");
                fprintf(name , "%s" , argv[4]);
                fclose(name);
            }
            fclose(file);
        }

    }
    else{
        if(strcmp("user.name" , argv[2]) == 0){
            char command[max];
            finding_neogit_path();
            snprintf(command ,max , "%s\\.neogit\\local_username.txt");
            FILE *file = fopen(command , "w");
            fprintf(file , "%s" , argv[3]);
            fclose(file);
        }
        if(strcmp("user.email" , argv[2]) == 0){
            char command[max];
            finding_neogit_path();
            snprintf(command ,max , "%s\\.neogit\\local_useremail.txt");
            FILE *file = fopen(command , "w");
            fprintf(file , "%s" , argv[3]);
            fclose(file);
        }


    }
}
// init functions
int check_exist_neogit_folder(char * cwd){
    // printf("%d\n", strlen(cwd));
    // printf("%s\n" , cwd);
    if(strlen(cwd) == 3){
        return 0;
    }
    system("echo. > temp11.txt");
    system("dir /B > temp11.txt");
    FILE* fp = fopen("temp11.txt","r");
    char line[max_path_size];
    int n = 0;
    while (fgets(line , max_path_size , fp))
    {
        int len = strlen(line);
        if(line[len-1] == '\n') line[len-1] = '\0';
        if(strcmp(line , ".neogit") == 0){
            // neogit exists
            n = 1;
            printf(".neogit folder exists\n");
            fclose(fp);
            system("del temp11.txt");
            return 1;
        }
    }
    fclose(fp);
    system("del temp11.txt");
    if(n == 0){
        printf("yes\n");
        char str[max];
        chdir("..");
        getcwd(str , max);
        return check_exist_neogit_folder(str);
        // chdir(cwd);
    }
}
void add_to_timeline(char * cwd , char * timeline){
    struct stat file_stat;
    char copy[max_path_size];
    strcpy(copy, cwd);
    if(stat(cwd , &file_stat) == 0){
        if(S_ISDIR(file_stat.st_mode)){
            chdir(cwd);
            system("echo. >salam.txt");
            system("dir /B > salam.txt");
            FILE * fp = fopen("salam.txt", "r");
            char line[max_path_size];
            while (fgets(line, max_path_size, fp)){
                int len = strlen(line);
                if(line[len-1] == '\n') line[len-1] = '\0';
                if(strcmp(line , "salam.txt") != 0 && strcmp(line , ".neogit")){
                strcat(cwd ,  "\\");
                strcat(cwd , line);
                add_to_timeline(cwd , timeline);
                strcpy(cwd , copy);
                }
            }
            fclose(fp);
            system("del salam.txt");
            chdir("..");
        }
        if(S_ISREG(file_stat.st_mode)){
            FILE *file = fopen(timeline , "a");
            fprintf(file,"%s\n" , cwd );
            time_t modification_time = file_stat.st_mtime;
            fprintf(file , "%ld\n" , modification_time);
            fclose(file);
        }
    }
}
void filling_timeline(char * lazem){
    char cwd[max_path_size];
    getcwd(cwd, sizeof(cwd));
    char copy[max_path_size];
    strcpy(copy , cwd);
    system("echo. >temp22.txt");
    system("dir /B > temp22.txt");
    FILE *fp =  fopen("temp22.txt", "r");
    char line[max_path_size];
    while(fgets(line , max_path_size , fp)){
        int len = strlen(line);
        if(line[len-1] == '\n') line[len-1] = '\0';
        if(strcmp(line , ".neogit") != 0 && strcmp(line , "temp22.txt")){
        strcat(copy , "\\");
        strcat(copy , line);
        // chdir("..");
        add_to_timeline(copy , lazem);
        // chdir(".neogit")
        strcpy(copy,  cwd) ;
        }
    }
    fclose(fp);
    system("del temp22.txt");
}
int run_init(){


    
    // first we check .neogit folder is availvabale before in this path or its parent
    char cwd[max_path_size];
    getcwd(cwd , sizeof(cwd));
    if(check_exist_neogit_folder(cwd) == 0){
        //neogit folder doesn't exist
        // fprintf(lf , "%s" , "HEAD");
        // fclose(lf);
        // FILE *file = fopen("C:\\.neogit_info\\project_location.txt" , "w");
        // fprintf(file , "%s" ,  cwd );
        // fclose(file);
        // FILE *file2 = fopen("C:\\.neogit_info\\branch_name.txt" , "w");
        // fprintf(file2, "%s" , "master");
        // fclose(file2);
        // FILE *file3 = fopen("C:\\.neogit_info\\last_commit.txt" , "w");
        // fprintf(file3, "%s" , "0");
        // fclose(file3);
        // FILE *file4 =fopen("C:\\.neogit_info\\gen_username.txt" , "r");
        // char name[max];
        // fgets(name ,max , file4);
        // int availableOrnot = 1 ;
        // if(name == NULL) availableOrnot = 0;
        // fclose(file4);
        // if(availableOrnot == 0) {
            // FILE * file5 = fopen("C:\\.neogit_info\\gen_username.txt" , "w");
            // fclose(file5);
        // }
        // FILE * file6 = fopen("C:\\.neogit_info\\gen_useremail.txt" , "r");
        // fgets(name , max , file6);
        // availableOrno/t = 1;
        // if(name == NULL) availableOrnot = 0;
        // fclose(file6);
        // if(availableOrnot == 0){
            // FILE * file7 = fopen("C:\\.neogit_info\\gen_useremail.txt" , "w");
            // fclose(file7);
        // }
        // FILE * sf = fopen("C:\\.neogit_info\\loc_or_gen.txt" , "w");
        // if(availableOrnot == 1)  fprintf(sf,"%s","general");
        // fclose(sf);
        chdir(cwd);
        printf("%s" , cwd);
        system("mkdir .neogit");
        chdir(".neogit");
        system("mkdir commits");
        system("echo HEAD >head_or_not.txt");
        FILE * fope = fopen("branch_name.txt" , "w");
        fprintf(fope , "master");
        fclose(fope);
        FILE *lf = fopen("C:\\.neogit_info\\head_or_not.txt" , "w");
        system("echo 0 >last_commit.txt");
        chdir("commits");
        mkdir("master");
        system("echo 1 > commit_hash.txt");
        system("echo. > message_shortcut.txt");
        system("echo. > commit_massages.txt");
        chdir("..");
        mkdir("hooks");
        chdir("hooks");
        system("echo eof_blank_space.txt > all_hooks.txt");
        system("echo todo_ckeck >> all_hooks.txt");
        system("echo file_size_check.txt >> all_hooks.txt");
        system("echo character_limit.txt >> all_hooks.txt");

        system("echo 0 > eof_blank_space.txt");
        system("echo 0 > todo_ckeck.txt");
        system("echo 0 > file_size_check.txt");
        system("echo 0 > character_limit.txt");
        chdir("..");
        system("mkdir staging_area");
        chdir("staging_area");
        system("echo. >staged_files.txt");
        system("echo. >unstaged_files.txt");
        chdir("..");
        system("echo. >tag.txt");
        system("echo. >tracked.txt");
        system("echo. >timeline.txt");
        system("echo. >local_username.txt");
        system("echo. >local_useremail.txt");
        // we made the essential folders now we go back to project path
        chdir("..");
        strcat(cwd , "\\.neogit\\timeline.txt");
        filling_timeline(cwd);
        finding_neogit_path();
        system("mkdir C:\\.neogit_info");
        // char command[max];
        // snprintf(command , max , "echo %s >> C:\\.neogit_info\\project_locations.txt" , neogit_folder_location);
        // system(command);
        system("echo. > C:\\.neogit_info\\project_locations.txt");
        FILE *fii = fopen("C:\\.neogit_info\\project_locations.txt" , "a");
        fprintf(fii,"%s\n",neogit_folder_location);
        fclose(fii);
        return 1;
    }
    else{
        printf("NeoGit has already been initialized in this directory or above.\n");
        return 0;
    }
}



//add functions
int run_add(char *argv[] , int argc){
    char cwd[max];
    getcwd(cwd  , max);
    // printf("ss\n");
    finding_neogit_path();
    chdir(cwd);
    // printf("%s\n" , cwd);
    //scannnig the first empty line
    // printf(".%s.\n" , neogit_folder_location);
    if(argc == 3) {
        if(strchr(argv[2] , '*') != NULL){
            //there is star in argv[2]
            return wildcard_add_operation(argv[2]);
        }
        if(strcmp(argv[2] ,"redo") == 0) {return  redo_addition(argv , argc);}
        else // it is a normal add operation
        return normal_add_operation(argv[2] , 0);
    }
    else if( argc > 3 && (strcmp(argv[2] , "-f") == 0)) return multi_add_operation(argv , argc);
    else if(argc> 3 && (strcmp(argv[2] , "-n") ==0)) 
    {
        int n = atoi(argv[3]);
        search_stagedfiles(n);
        return n;
    }
    return 0;
}
void add_name_to_list(char * file_path){
    char loc[max_path_size];
    strcpy(loc , neogit_folder_location);
    strcat(loc , "\\.neogit\\staging_area\\staged_files.txt");
    FILE * file = fopen(loc , "a");
    fprintf(file , "%s\n" , file_path);
    time_t present = time(NULL);
    fprintf(file , "%ld\n" , (long)present);
    fclose(file);
}//adding name of the file into the list
int check_add(char * file_path){
    char loc[max_path_size];
    strcpy(loc , neogit_folder_location);
    strcat(loc , "\\.neogit\\staging_area\\staged_files.txt");
    FILE * file = fopen(loc , "r");
    char line[max_path_size];
    while(fgets(line, max_path_size , file)){
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(strcmp(line , file_path) == 0){
            return 1;// means that the file has been existed in staged area
        }
    }
    return 0; // means that the file need to be added to staged area
}
void copy_file_to_staged(char * file_path){
    // printf("SSSS\n");
    struct stat file_stat;
    char copy[max_path_size];
    strcpy(copy , file_path);
    if(stat(file_path , & file_stat) != 0 ) { perror("Error: "); exit(-1);}
    // char inoCode[max_ino_code];
    // snprintf(inoCode , max_ino_code , "%lu" , file_stat.st_ino);
    char staging_area_loc[max_path_size];
    strcpy(staging_area_loc , neogit_folder_location);
    strcat(staging_area_loc , "\\.neogit\\staging_area\\");
    char file_path_copy[max_path_size];
    strcpy(file_path_copy , file_path);
    // char *token = strtok(file_path , "\\");
    // char *token_copy;
    // while(token != NULL){
    //     strcpy(token_copy , token);
    //     token = strtok(NULL , "\\");
    // }
    // strcat(staging_area_loc , token_copy);
    char command[max_command_size];
    strcpy(command , "copy ");
    strcat(command , file_path_copy);
    strcat(command , " ");
    strcat(command , staging_area_loc);
    // printf("%s \n %s\n%s\n\n" , command , file_path , staging_area_loc);
    system(command);
}
int normal_add_operation(char * file_path  , int mode){
    // first check if it has been added before or not
    // finding_neogit_path();
    char cwd[max_path_size];
    if(mode == 0){
    getcwd(cwd, sizeof(cwd));
    strcat(cwd , "\\");
    strcat(cwd , file_path);
    }
    char copy[max_path_size];
    strcpy(copy , cwd);
    struct stat file_stat;
    // printf("%s\n" , cwd);
    if(stat(cwd , &file_stat) == 0){
    // printf("salam\n");
        if(S_ISDIR(file_stat.st_mode)){
            chdir(cwd);
            system("echo. > khab.txt");
            system("dir /B > khab.txt");
            FILE * fp = fopen("khab.txt" , "r");
            char line[max_path_size];
            char all_path[20];
            int count = 0;
            while (fgets(line , max_path_size , fp))
            {
                int len = strlen(line);
                if(line[len - 1] == '\n') line[len-1] = '\0';
                if(strcmp(line , "khab.txt") != 0 && strcmp(line , ".neogit")!= 0){
                    // printf("line:%s\n" , line);
                    normal_add_operation(line , 0);
                }
            }
            fclose(fp);
            system("del khab.txt");
            chdir("..");
        }
        else if(S_ISREG(file_stat.st_mode)){
            // it is a file
            if(check_add(cwd) == 0){
                // the file hasn't been added before
                // start the proccess of adding
                // printf("%s\n" , cwd );
                add_name_to_tracked(cwd); // adding name  to tracked files list with time of adding
                add_name_to_list(cwd); // adding file to list with time of addding
                copy_file_to_staged(cwd); // copying file into staging area
                printf("%s added successfully\n" , file_path);
            }
            else{
            // it has been added before
            printf("%s has been added before\n" , file_path);

        }
    }
    else{
        printf("%s : wrong path\n" , file_path);
    }
    }
    return 0;
}
int multi_add_operation(char *argv[] , int argc){
    for(int i = 3 ; i<argc ; i++){
        normal_add_operation(argv[i] , 0);
    }
    return 1;
}
void add_name_to_tracked(char *cwd){
    char current_working_d[max_path_size];
    getcwd(current_working_d , max_path_size);
    char temp_loc[max_path_size];
    char loc[max_path_size];
    printf("%s\n",neogit_folder_location);
    strcpy(loc , neogit_folder_location);
    strcpy(temp_loc , loc);
    strcat(loc , "\\.neogit\\tracked.txt");
    strcat(temp_loc , "\\.neogit\\temp.txt");
    char command[max_command_size];
    printf("%s %s\n", loc , temp_loc);
    // strcmp(command , "echo. >");
    FILE* fp = fopen(loc , "r");
    FILE* temp = fopen(temp_loc , "w");
    fprintf(temp , " \n");
    char line[max_path_size];
    int situation = 0 ; // 0 means it wasn't found in the tracked files
    // fgets(line , max_path_size , fp); // the first empty line
    // char command[max_command_size];
    while(fgets(line , max_path_size , fp)){
        // printf("this line is :%s\n" , line);
        int n = strlen(line);
        if(line [n-1] == '\n') line[n-1] = '\0';
        if(strcmp(cwd , line)==0){
            situation = 1; // 1 means it was added to stage in the past
            fprintf(temp , "%s\n" , line);
            time_t present = time(NULL);
            fgets(line,max_command_size , fp);
            fprintf(temp , "%ld\n" , (long)present);
        }
        else{
            fprintf(temp , "%s\n" , line);
            fgets(line,max_command_size , fp);
            fprintf(temp , "%s" , line);
        }
    }
    if(situation == 0){
        fprintf(temp , "\n%s\n" , cwd);
        time_t present = time(NULL);
        fprintf(temp , "%ld\n" , (long)present);
    }
    fclose(fp);
    fclose(temp);
    char first_command[max_command_size];
    strcpy(first_command , "del " );
    strcat(first_command , loc);
    system(first_command);
    char second_command[max_command_size];
    strcpy(second_command , "move " );
    strcat(second_command , temp_loc);
    strcat(second_command , " " );
    strcat(second_command , loc);
    system(second_command);
}
void search_stagedfiles(int depth){
    char files[55][55] ;
    DIR * dir = opendir(".") ;
    struct dirent * entry ;
    int i ;
    for ( i = 0 ; ( entry = readdir(dir) ) != NULL ; i++ ) {
        if ( strcmp(entry->d_name , ".pro") && strcmp(entry->d_name , "." ) && strcmp(entry->d_name , ".." ) ) {
            strcpy( files[i] , entry->d_name ) ;
        } else {
            i-- ;
        }
    } 
    closedir(dir) ;
    char cwd_copy[max] ;
    getcwd(cwd_copy , sizeof(cwd_copy) ) ;
    for ( int j = 0 ; j < i ; j++ ) {
        chdir(cwd_copy) ;
        char cwd[max] ;
        getcwd(cwd , sizeof(cwd)) ;
        strcat(cwd , "\\") ;
        strcat(cwd , files[j]) ;

        int is_folder = 0 ;
        struct stat file_info ;
        stat(cwd , &file_info ) ;

        if( S_ISDIR( file_info.st_mode ) ) {
            is_folder = 1 ;
        }
        if ( is_folder && depth > 1  ) {
            fprintf( stdout , "%s <it is a folder> \n" , cwd ) ;
            chdir(files[j]) ;
            search_stagedfiles( depth - 1 ) ;
            chdir("..") ;
        } else if ( is_folder && depth == 1 ) {
            fprintf( stdout , "%s folder \n" , cwd ) ;
        } else if ( !is_folder ) {
            if ( check_add(cwd) == 0 ) {
                fprintf(stdout , "%s: is staged\n" , cwd ) ;
            } else {
                fprintf(stdout , "%s : isn't staged\n" , cwd ) ;
            }
        }
    }
    return ; 





    } // not working just work for n = 1
int wildcard_add_operation(char *file_name){
    char all_files[50][50];
    char command[max_command_size];
    strcpy(command , "dir /B ");
    strcat(command , file_name);
    strcat(command , " > fili.txt");
    system(command);
    FILE* fp = fopen("fili.txt","r");
    int i = 0;
    char line[max_path_size];
    while(fgets(line , max_path_size , fp)){
        if(strstr(line , "File Not Found") != NULL){
            printf("NO FILE EXIST WITH THAT NAME\n");
            fclose(fp);
            remove("fili.txt");
            return 0;
        }
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        printf("%s" , line);
        char cwd[max_path_size];
        getcwd(cwd , max_path_size);
        if(line != NULL)
        normal_add_operation(line , 0);
    }
    fclose(fp);
    system("del fili.txt");
    return 1;
} // seems to work properly
int redo_addition(char *argv[] , int argc){
    char path[max];
    snprintf(path , max , "%s\\.neogit\\staging_area\\unstaged_files.txt" , neogit_folder_location);
    FILE *redo_file = fopen(path , "r");
    char line[max];
    fgets(line , max , redo_file);
    while(fgets(line, max ,redo_file)){
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        normal_add_operation(line,1);
    }
    fclose(redo_file);
    char command[max];
    snprintf(command , max ,"echo. > %s" , max);
}


//reset functions
int run_reset(char *argv[] , int argc){
    finding_neogit_path();
    // printf("%s\n\n" , neogit_folder_location);
    if(argc == 3){
        if(strcmp(argv[2] , "-undo") == 0){
            return undo_reset_operation();
        }
        else if(strchr(argv[2] , '*') != NULL){
            //there is star in argv[2]
            return wildcard_reset_operation(argv[2]);
        }
        // else if(strcmp("undo" , argv[2]) == 0) 
            // return undo_reset();  // reset undo
        else // it is a normal add operation
        return normal_reset_operation(argv[2] , 0);
    }
    else if( argc > 3 && (strcmp(argv[2] , "-f") == 0)) return multi_reset_operation(argv , argc);
    return 0;
}
int normal_reset_operation(char *file_path , int mode){
    char cwd[max_path_size];
    if(mode == 0){
    getcwd(cwd , sizeof(cwd));
    strcat(cwd , "\\");
    strcat(cwd , file_path);
    }
    else if(mode == 1){
        strcpy(cwd , file_path);
    }
    char copy[max_path_size];
    strcpy (copy , cwd);
    struct stat file_stat;
    if(stat(cwd , &file_stat) == 0){
        if(S_ISDIR(file_stat.st_mode)){
            chdir(cwd);
            system("echo. > khab.txt");
            system("dir /B > khab.txt");
            FILE *fp = fopen("khab.txt" , "r");
            char line[max_path_size];
            while(fgets(line , max_path_size , fp))
            {
                int len = strlen(line);
                if(line[len -1] == '\n') line[len-1] = '\0';
                if(strcmp(line , "khab.txt") != 0 && strcmp(line ,".neogit") != 0){
                    normal_reset_operation(line , 0);
                }
            }
            fclose(fp);
            system("del khab.txt");
            chdir("..");
        }
        else if(S_ISREG(file_stat.st_mode)){
            // it is a file
            if(check_add(cwd) == 0){
                //the file ins't in staging area
                printf("%s is'nt in staging file\n" , file_path);
            }
            else if( check_add(cwd) == 1){
                //the file is in staging area so we shoule
                //1. remove it from staging area
                remove_name_from_list(cwd);
                //2. delete the file from staging area
                del_file_from_staged(cwd);
                printf("ss\n");
                //3.add to unstaged list with time 
                add_to_unstage_list(cwd);
            }
        }
        else{
            printf("%s : wrong path\n" , file_path);
        }
    }
    return 0;
}

void remove_name_from_list(char * path){
    // char cwd[max_path_size];
    // getcwd(cwd, sizeof(cwd));
    // strcat(cwd , "\\");
    // strcat(cwd , path); // path of the file
    char staged[max_command_size];
    char temp[max_command_size];
    strcpy(staged , neogit_folder_location);
    strcpy(temp , neogit_folder_location);
    strcat(temp , "\\.neogit\\staging_area\\temp.txt");
    // printf("cwd = %s\n te   mp =%s \n" , staged , temp);
    char command[max_command_size];
    strcpy(command , "echo. >");
    strcat(command , temp);
    system(command);
    strcat(staged , "\\.neogit\\staging_area\\staged_files.txt");
    FILE* fp = fopen(staged , "r");
    FILE* ft = fopen(temp , "w");
    char line[max_path_size];
    // printf("cwd = %s\n temp =%s \n" , staged , temp);
    while(fgets(line, max_path_size , fp)) {
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(strcmp(path , line) == 0){
            fgets(line , max_path_size , fp);
            continue;
        }
        else{
            fprintf(ft,"%s\n",line);
        }
    }
    fclose(fp);
    fclose(ft);
    remove(staged);
    rename(temp , staged);
}

void del_file_from_staged(char * path){
    char *toke_copy;
    printf("path = %s\n" , path);
    strcpy(toke_copy , path);
    char *tok = strtok(toke_copy , "\\");
    // printf("%s\n" ,  tok);
    char tok_copy[max_path_size];
    while(tok != NULL){
        strcpy(tok_copy , tok);
        tok = strtok(NULL , "\\");
    }
    char staged[max_path_size];
    snprintf(staged , max_path_size , "%s\\.neogit\\staging_area\\%s" , neogit_folder_location , tok_copy);
    // printf("%s\n" , staged);
    char command[max_command_size];
    remove(staged);
}
void add_to_unstage_list(char *path){
    char cwd[max_path_size];
    getcwd(cwd , max_path_size);
    strcat(cwd , "\\");
    strcat(cwd , path);
    char unsaged[max_path_size];
    strcpy(unsaged , neogit_folder_location);;
    strcat(unsaged , "\\.neogit\\staging_area\\unstaged_files.txt");
    FILE * fp = fopen(unsaged , "a");
    fprintf(fp ,"%s\n" , cwd);
    time_t current = time(NULL);
    fprintf(fp , "%ld\n" , (long)current);
    fclose(fp);
}
int wildcard_reset_operation(char *file_name){
    char command[max_command_size];
    strcpy(command , "dir /B ");
    strcat(command , file_name);
    strcat(command , " > filif.txt");
    system(command);
    FILE*fp =fopen("filif.txt" , "r");
    char line[max_path_size];
    while(fgets(line , max_path_size , fp)){
        if(strstr(line , "File Not Found") != NULL){
            printf("NO FILE EXIST WITH THAT NAME\n");
            fclose(fp);
            return 0;
        }        
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(line != NULL)
        normal_reset_operation(line , 0);
    }
    system("del filif.txt");
    fclose(fp);
    return  1;
}
int multi_reset_operation(char *argv[] , int argc){
    for(int i = 3 ; i < argc ; i++){
        normal_reset_operation(argv[i] , 0);
    }
    return 1;
}
int undo_reset_operation(){
    char loc[max_path_size];
    snprintf(loc , max_command_size , "%s\\.neogit\\staging_area\\staged_files.txt" , neogit_folder_location);
    FILE * file = fopen(loc , "r");
    char line[max_path_size];
    char another_line[max_path_size];
    char third_line[max_path_size];
    fgets(line , max_path_size , file);
    while(fgets(line , max_path_size , file)){
        strcpy(third_line , another_line);
        strcpy(another_line , line);
    }
    normal_reset_operation(line , 1);
    return 0;
}
//end of reset function



// status functions
int run_status(){
    DIR * dir = opendir(".") ;
    struct dirent * entry ;
    while ( ( entry = readdir(dir) ) != NULL ) {
        char cwd[max_path_size];
        getcwd(cwd,sizeof(cwd));
        printf("%s\n" , entry->d_name);
        strcat(cwd , entry->d_name);
        struct stat file_stat;
        stat(cwd , &file_stat);
        if (S_ISREG(file_stat.st_mode)) {
            printf("%s : " ,entry->d_name);
            if(check_add(cwd) == 1){
                printf("+");
            }
            else{
                printf("-");
            }
            if(check_status_M(cwd) == 0){
                check_status_A(cwd);
            }
            // check_status_D(cwd);
            // check_status_T(cwd);
        }
    }
    return 0;
}
void check_status_A(char * cwd){

}
int check_status_M(char *cwd){
    // finding_neogit_path();
    // char line[max_path_size];
    // long long time;
    // while(fgets(line , max_command_size , fp)){
    //     int n = strlen(line);
    //     if(line[n-1] == '\n') line[n-1] = '\0';
    //     if(strcmp(line , cwd)==0){
    //         fscanf(fp , "%lld" ,  &time);
    //         struct stat file_stat;
    //         long long int mod = file_stat.st_mode;
    //         if(mod < time){
    //             printf("M");
    //             return 1;
    //         }
    //         else
    //         return 0;
    //     }
    // }
    return -1;
}


//commit functions
int run_commit(char *agrv[] , int argc){

    finding_neogit_path();
    char pp[max];
    snprintf(pp , max , "%s\\.neogit\\local_useremail.txt" , neogit_folder_location);
    FILE* useridfile = fopen(pp , "r");
    fscanf(useridfile , "%s"  , user_email);
    fclose(useridfile);
    char ss[max];
    snprintf(ss,max,"%s\\.neogit\\local_username.txt" , neogit_folder_location);
    FILE * pas = fopen(ss , "r");
    fscanf(pas, "%s" , user_name);
    fclose(pas);
    char path[max];
    snprintf(path , max , "%s\\.neogit\\branch_name.txt" , neogit_folder_location);
    FILE *fil = fopen(path , "r");
    fscanf(fil , "%s" , branch_name);
    printf(".%s.\n",branch_name );
    fclose(fil);
    char path2[max];
    snprintf(path2 , max , "%s\\.neogit\\last_commit.txt" , neogit_folder_location);
    FILE *fp = fopen(path2 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);
    char pathh[max];
    snprintf(pathh , max , "%s\\.neogit\\head_or_not.txt" , neogit_folder_location);
    FILE *fopa = fopen(pathh , "r");
    fscanf(fopa , "%s" , head_or_not);
    fclose(fopa);
    if(strcmp(head_or_not , "HEAD") != 0 ){
        printf("you should be on the head branch to comitt");
        return 0;
    }
    if(argc == 3 && strcmp(agrv[2] , "-m") == 0){
        printf("invalid message\n");
    }
    if(argc == 4 && strcmp(agrv[2] , "-m") == 0)
        commit_operation_without_space(agrv);
    if(argc > 4 && strcmp(agrv[2] , " -m") == 0)
        commit_operation_with_space(agrv , argc);
    if(argc==4 && strcmp(agrv[2] , "-s")==0)
        commit_operation_with_shortcut(agrv , argc);
    return 0;
}
void commit_operation_without_space(char *argv[]){
    // printf("%d %d " ,argv[3][0] == '\"', argv[3][strlen(argv[3])-1] == '\"');
    if(argv[3][0] == '\"' || argv[3][strlen(argv[3])-1] == '\"'){
        printf("invalid message\n");
        return;
    }
    commit_operation_with_space(argv , 4);
}
void commit_operation_with_space(char *argv[] , int args){
    if(args > 4){
        printf("invalid message\n");
    }
    int sum = 0;
    if(strlen(argv[3]) > 72){
        printf("reached maximum limit\n");
    }
    char commit_hash[max_path_size];
    snprintf(commit_hash , max_path_size , "%s\\.neogit\\commits\\commit_hash.txt" , neogit_folder_location);
    FILE *fp = fopen(commit_hash , "r");
    int num;
    int s ;
    while (fscanf(fp , "%d" , &num)!=-1)
    {
        s = 1;
        // printf("%d\n" , fscanf(fp , "%d" , &num));
        num++;
    }
    fclose(fp);
    // if(s!=1){
    //     num = 1;
    // }
    printf("%d\n" , num);
    chdir(neogit_folder_location);
    chdir(".neogit\\commits");
    printf("%s\n" , branch_name);
    chdir(branch_name);
    char command[max_command_size];
    char temp[max_path_size];
    snprintf(temp , max , "%d" , num);
    mkdir(temp);
    char first_path[max_path_size];
    char second_path[max_path_size];
    strcpy(first_path , "");
    strcpy(second_path , "");
    printf(".%s." , branch_name);
    snprintf(first_path ,max_command_size, "%s\\.neogit\\commits\\%s\\%d\0" , neogit_folder_location , branch_name ,num);
    snprintf(second_path ,max_command_size ,"%s\\.neogit\\staging_area\0" , neogit_folder_location);
    printf("second : %s , first : %s\n" , second_path , first_path);
    printf("reached\n");
    copy_dir(second_path, first_path);
    char third_path[max_path_size];
    snprintf(third_path , max_path_size , "%s\\.neogit\\commits\\commit_massages.txt");
    FILE *file = fopen(third_path , "a");
    fprintf(file , "%d\n" , num); //commit hash
    fprintf(file , "%s\n" , argv[3]);
    // fprintf(file, "\n"); //message
    fprintf(file, "%s\n" , branch_name); //branch name
    fprintf(file , "%d\n" , last_commit); //last commit number
    
    char cwd[max_path_size];
    getcwd(cwd , sizeof(cwd));
    chdir(first_path);
    int num_of_files = 0;
    DIR*dir = opendir(".") ;
    struct dirent * entry ;
    while ( (entry = readdir(dir)) != NULL ){
        num_of_files++;
    }
    closedir(dir);
    chdir(cwd);
    fprintf(file , "%d\n" , num_of_files - 2); //num of files

    time_t present = time(NULL);
    fprintf(file ,"%d\n" , ctime(&present)); //time in secnods

    struct tm *time_info;
    char date[max_date_size];

    time(&present);
    time_info=localtime(&present);

    strftime(date ,  sizeof(date) , "%y/%m/%d %H:%M:%S" , time_info);
    fprintf(file,"%s\n" , date); // time in year month day hour minute second format
    fprintf(file , "%s\n" ,user_name );
    fprintf(file , "%s\n" , user_email);

    fclose(file);
    char fourth_path[max_path_size];
    snprintf(fourth_path , max_path_size , "%s\\.neogit\\staging_area" , neogit_folder_location);



    delete_files(fourth_path);
    FILE * ft =fopen(commit_hash , "w");
    fprintf(ft , "%d" , num);
    fclose(ft);

    char tempf[max_command_size];
    strcpy(tempf , fourth_path);
    strcat(fourth_path , "\\staged_files.txt");
    strcat(tempf , "\\unstaged_files.txt");
    char commandf[max_command_size];
    snprintf(commandf ,max_command_size ,  "echo. > %s", fourth_path);
    system(commandf);
    char second_command[max_command_size];
    snprintf(second_command , max_path_size , "echo. > %s" , temp);
    system(second_command);
}
void commit_operation_with_shortcut(char *argv[] , int argc){
    char path1[max];
    snprintf(path1 ,max , "%s\\.neogit\\last_commiy.txt" , neogit_folder_location);
    FILE *fp = fopen(path1 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);    
    
    char path[max_path_size];
    strcpy(path , neogit_folder_location);
    strcat(path , "\\.neogit\\commits\\message_shortcut.txt");
    FILE *ft = fopen(path , "r");
    char line[max_path_size];
    char message[max_message_size];
    int found_or_not = 0;
    while (fgets(line ,max_path_size , ft))
    {
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(strcmp(line , argv[argc - 1]) == 0){
            found_or_not = 1;
            fgets(message , max_message_size , ft);
            break;
        }
    }
    if(found_or_not == 0){
        printf("Shortcut not found.\n");
        return;
    }


    char commit_hash[max_path_size];
    snprintf(commit_hash , max_path_size , "%s\\.neogit\\commits\\commit_hash.txt" , neogit_folder_location);
    FILE *fz = fopen(commit_hash , "r");
    int num;
    int s ;
    while (fscanf(fz , "%d" , &num)!=-1)
    {
        s = 1;
        // printf("%d\n" , fscanf(fp , "%d" , &num));
        num++;
    }
    fclose(fz);
    // if(s!=1){
    //     num = 1;
    // }
    printf("%d\n" , num);
    chdir(neogit_folder_location);
    chdir(".neogit\\commits\\master");
    char command[max_command_size];
    char temp[max_path_size];
    snprintf(temp , max , "%d" , num);
    mkdir(temp);
    char first_path[max_path_size];
    char second_path[max_path_size];
    strcpy(first_path , "");
    strcpy(second_path , "");
    snprintf(first_path ,max_command_size, "%s\\.neogit\\commits\\%s\\%d" , neogit_folder_location , branch_name ,num);
    snprintf(second_path ,max_command_size ,"%s\\.neogit\\staging_area" , neogit_folder_location);
    // printf("reached\n");
    // printf("second : %s , first : %s\n" , second_path , first_path);
    copy_dir(second_path, first_path);
    char third_path[max_path_size];
    snprintf(third_path , max_path_size , "%s\\.neogit\\commits\\commit_massages.txt", neogit_folder_location);
    FILE *file = fopen(third_path , "a");
    fprintf(file , "%d\n" , num); //commit hash
    fprintf(file ,  "%s\n" , message); //message
    // fprintf(file, "\n");
    fprintf(file, "%s\n" , branch_name); // branch name
    fprintf(file , "%d\n" , last_commit); // last commit number

    char cwd[max_path_size];
    getcwd(cwd , sizeof(cwd));
    chdir(first_path);
    int num_of_files = 0;
    DIR*dir = opendir(".") ;
    struct dirent * entry ;
    while ( (entry = readdir(dir)) != NULL ){
        num_of_files++;
    }
    closedir(dir);
    chdir(cwd);
    fprintf(file , "%d\n" , num_of_files - 2); // num of files
    time_t present = time(NULL);
    fprintf(file ,"%s\n" , ctime(&present)); // time in seconds
    struct tm *time_info;
    char date[max_date_size];

    time(&present);
    time_info=localtime(&present);

    strftime(date ,  sizeof(date) , "%y/%m/%d %H:%M:%S" , time_info);
    fprintf(file,"%s\n" , date); // time in year month day hour minute second format
    fprintf(file ,"%s\n" , user_name );
    fprintf(file , "%s\n" , user_email);
    fclose(file);
    char fourth_path[max_path_size];
    snprintf(fourth_path , max_path_size , "%s\\.neogit\\staging_area" , neogit_folder_location);
    delete_files(fourth_path);
    FILE * fs =fopen(commit_hash , "w");
    fprintf(fs , "%d" , num);
    fclose(fs);
    char tempe[max_command_size];
    strcpy(tempe , fourth_path);
    strcat(fourth_path , "\\staged_files.txt");
    strcat(tempe , "\\unstaged_files.txt");
    char fcommand[max_command_size];
    snprintf(fcommand , max_command_size , "echo. > %s", fourth_path);
    system(fcommand);
    char second_command[max_command_size];
    snprintf(second_command, max_command_size , "echo. > %s" , temp);
    system(second_command);
}
//set shortcut
int run_set(char *argv[] , int argc){
    finding_neogit_path();
    char path33[max];
    strcpy(path33 , neogit_folder_location);
    strcat(path33 , "\\.neogit\\branch_name.txt");
    // FILE *fil = fopen("C:\\.neogit_info\\branch_name.txt" , "r");
    FILE *fil = fopen(path33 , "r");
    fscanf(fil , "%s" , branch_name);
    fclose(fil);
    char path55[max];
    FILE *fp = fopen(path55 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);
    char path[max_path_size];
    strcpy(path , neogit_folder_location);
    strcat(path , "\\.neogit\\commits\\message_shortcut.txt");
    FILE *ft = fopen(path , "a");
    fprintf(ft , "%s\n" , argv[argc-1]);
    fprintf(ft ,"%s" , argv[3]);
    fprintf(ft , "\n");
    fclose(ft);
    return 0;
}
// replace functions
int run_replace(char *argv[] , int argc){
    finding_neogit_path();
    char path33[max];
    strcpy(path33 , neogit_folder_location);
    strcat(path33 , "\\.neogit\\branch_name.txt");
    // FILE *fil = fopen("C:\\.neogit_info\\branch_name.txt" , "r");
    FILE *fil = fopen(path33 , "r");
    fscanf(fil , "%s" ,  branch_name);
    fclose(fil);
    char path55[max];
    FILE *fp = fopen(path55 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);



    char path[max_path_size];
    strcpy(path , neogit_folder_location);
    strcat(path , "\\.neogit\\commits\\message_shortcut.txt");
    FILE *ft = fopen(path , "r");

    char temp[max_path_size];
    strcpy(temp , neogit_folder_location);
    strcat(temp ,"\\.neogit\\commits\\temp.txt");
    FILE *temp_short = fopen(temp , "w");

    int shortcut_found_ot_not=0;

    char line[max_path_size];
    while (fgets(line , max_path_size , ft))
    {
        fprintf(temp_short , "%s" , line);
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        // printf("line = %s , argv[argc-1] = %s\n" ,line , argv[argc-1]);
        if(strcmp(line , argv[argc-1]) == 0){
            shortcut_found_ot_not = 1;
            fgets(line , max_path_size , ft);
            fprintf(temp_short , "%s" , argv[3]);
            fprintf(temp_short , "\n");
        }
    }
    fclose(ft);
    fclose(temp_short);
    remove(path);
    rename(temp,path);
    if(shortcut_found_ot_not ==0){
        printf("\n\tThe command you entered is not a valid NeoGit Shortcut.\n");
    }
    // deleting old  message_shortcut.txt and renaming temp.txt to message_shortcut.txt
    return 0;
}
// remove functions
int run_remove(char *argv[] , int argc){
    finding_neogit_path();
    char path33[max];
    strcpy(path33 , neogit_folder_location);
    strcat(path33 , "\\.neogit\\branch_name.txt");
    // FILE *fil = fopen("C:\\.neogit_info\\branch_name.txt" , "r");
    FILE *fil = fopen(path33 , "r");
    fscanf(fil , "%s" , branch_name);
    fclose(fil);
    char path55[max];
    FILE *fp = fopen(path55 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);



    char path[max_path_size];
    strcpy(path , neogit_folder_location);
    strcat(path , "\\.neogit\\commits\\message_shortcut.txt");
    FILE *ft = fopen(path , "r");

    char temp[max_path_size];
    strcpy(temp , neogit_folder_location);
    strcat(temp ,"\\.neogit\\commits\\temp.txt");
    FILE *temp_short = fopen(temp , "w");

    int shortcut_found_or_not = 0;
    char line[max_path_size];
    while (fgets(line , max_path_size , ft))
    {
        // fprintf(temp_short , "%s" , line);
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , argv[argc-1]) == 0){
            shortcut_found_or_not = 1 ;
            fgets(line , max_path_size , ft);
        }
        else{
            fprintf(temp_short , "%s\n" , line);
        }
    }
    fclose(ft);
    fclose(temp_short);
    remove(path);
    rename(temp,path);

    if(shortcut_found_or_not ==0){
        printf("\n\tThe command you entered is not a valid NeoGit Shortcut.\n");
    }
    return 0;
    // deleting old  message_shortcut.txt and renaming temp.txt to message_shortcut.txt
}
//functions to copy files in folders
void copy_files(char *srcpath, char *destpath) {
    FILE *src, *dest;
    char buffer[max_path_size];
    int n;

    src = fopen(srcpath, "rb");
    if (src == NULL) {
        perror("Error opening source file");
        exit(1);
    }

    dest = fopen(destpath, "wb");
    if (dest == NULL) {
        perror("Error opening destination file");
        exit(1);
    }

    while ((n = fread(buffer, 1, max_path_size, src)) > 0) {
        fwrite(buffer, 1, n, dest);
    }

    fclose(src);
    fclose(dest);
}
void copy_dir(char *srcpath, char *destpath) {
    DIR *dp;
    struct dirent *entry;
    char tempsrc[max_path_size], tempdest[max_path_size];
    dp = opendir(srcpath);
    if (dp == NULL) {
        perror("Error opening source directory");
        exit(1);
    }
    strcpy(tempsrc, srcpath);
    strcpy(tempdest, destpath);
    while ((entry = readdir(dp)) != NULL) {
        if (strcmp(entry->d_name, "unstaged_files.txt") == 0) {
            continue;
        }

        strcat(tempsrc, "\\");
        strcat(tempsrc, entry->d_name);

        strcat(tempdest, "\\");
        strcat(tempdest, entry->d_name);

        struct stat st;
        stat(tempsrc, &st);

        // if (S_ISDIR(st.st_mode)) {
        //     mkdir(tempdest);
        //     copy_dir(tempsrc, tempdest);
         if (S_ISREG(st.st_mode)) {
            copy_files(tempsrc, tempdest);
        }

        strcpy(tempdest, destpath);
        strcpy(tempsrc, srcpath);
    }

    closedir(dp);
}
//function to delete some files in folder
void delete_files(char *folder_b) {
    DIR *dir;
    struct dirent *entry;

    if ((dir = opendir(folder_b)) == NULL) {
        perror("Error opening directory");
        return;
    }

    while ((entry = readdir(dir)) != NULL) {
        if(strcmp(entry->d_name , "staged_files.txt") !=0 &&  strcmp(entry->d_name , "unstaged_files.txt") != 0){
        char file_path_b[1024];
        snprintf(file_path_b, sizeof(file_path_b), "%s/%s", folder_b, entry->d_name);

        if (remove(file_path_b) == 0) {
            printf("Deleted file: %s\n", file_path_b);
        } else {
            perror("Error deleting file");
        }
        }
    }

    closedir(dir);
}

//log functions
int run_log(char *argv[] , int argc){
    finding_neogit_path();
    char path33[max];
    strcpy(path33 , neogit_folder_location);
    strcat(path33 , "\\.neogit\\branch_name.txt");
    // FILE *fil = fopen("C:\\.neogit_info\\branch_name.txt" , "r");
    FILE *fil = fopen(path33 , "r");
    fscanf(fil , "%s" , branch_name);
    fclose(fil);
    char path55[max];
    FILE *fp = fopen(path55 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);

    
    
    if(argc == 2) {
        normal_log();
    }
    else if(argc == 4 && strcmp(argv[2] , "-n") == 0){
        n_log(argv[3]);
    }
    else if(argc == 4 && strcmp(argv[2] , "-branch") == 0){
        branch_author_log(argv[3] , 0);
    }
    else if(argc == 4 && strcmp(argv[2] , "-author") == 0){
        branch_author_log(argv[3] , 1);
    }
    else if((argc == 5) && strcmp(argv[2] , "-since") == 0){
        since_before_log(argv , 0);
    }
    else if(argc == 5 && strcmp(argv[2] , "-before") == 0){
        since_before_log(argv , 1);
    }
    else if(argc == 5 && strcmp(argv[2] , "-search") == 0){
        if(strchr(argv[3] , '*') != NULL) wildcard_log(argv[3]);
        branch_author_log(argv[3] , 2);
    }
    else if(argc>5 && strcmp(argv[2] , "-search") == 0){
        multi_search(argv , argc);
    }
    return 0;
}
void normal_log(){
    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
    while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
        
    }
    fclose(fp);
    for(int i = num_of_all_commits -1 ; i>=0 ; i--){
        print_commit_log(all_commits[i]);
    }
}
void print_commit_log(struct commit commit_info) {
    printf("\nDate and Time :\t\t%s\n" , commit_info.date);
    printf("Message :\t\t\t%s\n" , commit_info.message);
    printf("Author :\t%s\n" , commit_info.author);
    printf("Commit Hash :\t\t\t%d\n" , commit_info.id);
    printf("Branch :\t\t\t%s\n", commit_info.branch );
    printf("Number of files commited :\t%d\n" , commit_info.num_of_file);
    printf("---------------------------------------------");
}
void n_log(char * num){
    char *en;
    long n = strtol(num , &en , 20);


    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
        while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
        
    }
    fclose(fp);
    for(int i = num_of_all_commits -1 ; i>= num_of_all_commits - n ; i--){
        print_commit_log(all_commits[i]);
    }
}
void branch_author_log(char *branch_author , int mode){
    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
    while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
        
    }
    fclose(fp);
    if(mode == 0){
        for(int i = num_of_all_commits -1 ; i>= 0 ; i--){
            // printf("%s , %s\n" , all_commits[i].branch , branch);
            if(strcmp(all_commits[i].branch , branch_author) == 0){
            print_commit_log(all_commits[i]);
            }
        }   
    }
    if(mode == 1){
        for(int i = num_of_all_commits -1 ; i >= 0 ; i--){
            if(strcmp(all_commits[i].author , branch_author) == 0) 
            print_commit_log(all_commits[i]);
        }
    }
    if(mode == 2){
        for(int i = num_of_all_commits - 1 ; i>=0 ; i--){
            if(strstr(all_commits[i].message , branch_author) == 0){
                print_commit_log(all_commits[i]);
            }
        }
    }
}
        void wildcard_log(char * str){
                char path[max_path_size];
            snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
            FILE *fp = fopen(path, "r");
            // printf("%s\n" , path);
            char line[max];
            struct commit all_commits[max];
            int num_of_all_commits = 0;
            //getting the first empty line
            fgets(line , max  , fp);
            while (fgets(line , max  , fp))
            {
                // printf("ss");
                int n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                if(strcmp(line , "") == 0) continue;
                char *end;
                long num = strtol(line , &end , 20);
                all_commits[num_of_all_commits].id = num;
                strcpy(all_commits[0].message , line);
                //message
                fgets(line , max , fp);
                n = strlen(line);
                
                if(line[n-1] == '\n') line[n-1]= '\0';
                strcpy(all_commits[num_of_all_commits].message , line);
                //name of branch
                fgets(line , max , fp);
                n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                strcpy(all_commits[num_of_all_commits].branch , line);
                //last commit number
                fgets(line , max , fp);
                n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                long last = strtol(line , &end , 20);
                all_commits[num_of_all_commits].last_commit_id = last;
                //num of files
                fgets(line , max , fp);
                n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                long file = strtol(line , &end , 20);
                all_commits[num_of_all_commits].num_of_file = file;
                //seconds
                fgets(line , max , fp);
                n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                long seconds = strtol(line , &end , 20);
                all_commits[num_of_all_commits].seconds_1970 = seconds;
                //date
                fgets(line , max , fp);
                n = strlen(line);
                if(line[n-1] == '\n') line[n-1]= '\0';
                strcpy(all_commits[num_of_all_commits].date , line);
                num_of_all_commits++;
            }
            fclose(fp);
            
            for(int i = num_of_all_commits - 1 ; i>=0 ; i--){
                if(wildcardMatchSentence(str , all_commits[i].message) == 1 ){
                    print_commit_log(all_commits[i]);
                }
            }
        }
        int wildcardMatchWord(const char *pattern, const char *word) {
            while (*pattern) {
                if (*pattern == '*') {
                    while (*pattern == '*') {
                        pattern++;
                    }

                    if (*pattern == '\0') {
                        return 1;
                    }

                    while (*word) {
                        if (wildcardMatchWord(pattern, word)) {
                            return 1;
                        }
                        word++;
                    }
                    return 0;
                } else if (*word && (*pattern == *word || *pattern == '?')) {
                    // Match a character or '?' in the pattern with the corresponding character in the word
                    pattern++;
                    word++;
                } else {
                    // Mismatch
                    return 0;
                }
            }

            return *word == '\0';
        }
        int wildcardMatchSentence(const char *pattern, const char *sentence) {
            // Split the sentence into words
            const char *start = sentence;
            while (*sentence) {
                if (*sentence == ' ') {
                    // Found a word boundary
                    if (wildcardMatchWord(pattern, start)) {
                        return 1;
                    }
                    while (*sentence == ' ') {
                        sentence++;
                    }
                    start = sentence;
                } else {
                    sentence++;
                }
            }
            // Check the last word in the sentence
            return wildcardMatchWord(pattern, start);
        }
int multi_search(char*argv[] , int argc){
    for(int i = 3 ; i < argc ; i++){
        branch_author_log(argv[i] , 2);
    }
}

void since_before_log(char *argv[] , int mode){
//note : your given time supposed to be in this format : 
// year: with 2 nums
// month : if <10 should typed with 0+num
//day : if <10 should typed with 0+zero also hour minuts and seconds
char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
    while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
        
    }

    for(int i = num_of_all_commits -1 ; i >= 0 ; i--){
        char time[max_date_size];
        strcpy(time , argv[3]);
        strcat(time," ");
        strcat(time , argv[4]);
        if(mode == 1 && strcmp(time , all_commits[i].date) >= 0 ){//since
            print_commit_log(all_commits[i]);
        }
        else if(mode == 0 && strcmp(time, all_commits[i].date) <= 0){
            print_commit_log(all_commits[i]);
        }
    }
}
// run branch
int run_branch(char *argv[], int argc) {
    finding_neogit_path();
    char path[max];
    snprintf(path ,max ,"%s\\.neogit\\head_or_not.txt" , neogit_folder_location);
    FILE *lf = fopen(path , "r");
    fscanf(lf , "%s" , head_or_not);
    fclose(lf);

    int len = strlen(argv[3]);
    int exist = 0;
    for(int i = 0 ; i < len ; i++){
        if(argv[3][i] >= '0' && argv[3][i] <= '9')
        exist = 1;
    }
    if(exist){
        printf("You can not use numbers in your branch name\n");
        return 0;
    }
    char ll[max];
    snprintf(ll,max , "%s\\.neogit\\branch_name.txt" , neogit_folder_location);
    FILE *fil = fopen(ll , "r");
    fgets(branch_name , max_path_size , fil);
    fclose(fil);

    char pp[max];
    snprintf(pp , max , "%s\\.neogit\\last_commit.txt" , neogit_folder_location);
    FILE *fp = fopen(pp , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);


    if(argc == 3){
        char path[max_path_size];
        strcpy(path , neogit_folder_location);
        strcat(path , "\\.neogit\\commits");
        // strcat(path , argv[2]);
        char command[max_command_size];
        // strcpy(command , "mkdir ");
        // strcat(command , path);
        // system(command);
        char cwd[max_path_size];
        getcwd(cwd , max_path_size);
        // printf(".%s." , path);
        chdir(path);
        mkdir(argv[2]);
        chdir(cwd);
        // printf("%s\n" , command);
        char ww[max];
        snprintf(ll,max , "%s\\.neogit\\branch_name.txt" , neogit_folder_location);
        FILE *filll = fopen(ww , "w");
        fprintf(filll , "%s" , argv[2]);
        fclose(filll);

    }
    else if(argc == 2){
        char path[max_path_size];
        strcpy(path , neogit_folder_location);
        strcat(path , "\\.neogit\\commits");
        DIR *d;
        struct dirent *dir;
        d = opendir(path);
        if(d){
            printf("ALL BRANCHES:\n");
            while((dir = readdir(d)) != NULL){
                if(strcmp(dir->d_name , "commit_hash.txt") != 0 && strcmp(dir->d_name , "commit_massages.txt")!=0 && strcmp(dir->d_name , "message_shortcut.txt") != 0 && strcmp(dir->d_name , ".") != 0 && strcmp(dir->d_name , "..") != 0)
                    printf("\t%s\n" , dir->d_name);
            }
            closedir(d);
        }
    }
    return 0;
}

// checkout functions
int run_checkout(char *argv[] , int argc){
    finding_neogit_path();
    char path33[max];
    strcpy(path33 , neogit_folder_location);
    strcat(path33 , "\\.neogit\\branch_name.txt");
    // FILE *fil = fopen("C:\\.neogit_info\\branch_name.txt" , "r");
    FILE *fil = fopen(path33 , "r");
    fscanf(fil , "%s" , branch_name);
    fclose(fil);
    char path55[max];
    FILE *fp = fopen(path55 , "r");
    fscanf(fp , "%d" , &last_commit);
    fclose(fp);
    if(is_staging_area_empty() != 0){ //returns the number of files in staging mode
        printf("Cannot checkout to another commit because staging area is not empty\n");
        return 0;
    }
    if(strcmp(argv[2] , "HEAD")==0){
        checkout_branch("master");

        return 0;
    }

    int len = strlen(argv[2]);
    int exist = 0;
    for(int i = 0 ; i < len ; i++){
        if(argv[2][i] >= '0' && argv[2][i] <= '9')
        exist = 1;
    }
    if(exist){ //means it is a commit hash
    // printf("salam\n");
        checkout_commit(argv[2]);
    }
    else // means it is a branch name
        checkout_branch(argv[2]);
    
}
// void checkout_commit(char *)
void checkout_branch(char *bran){
        char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line2[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line2 , max  , fp);
    while (fgets(line2 , max  , fp))
    {
        // printf("ss");
        int n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        if(strcmp(line2 , "") == 0) continue;
        char *end;
        long num = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line2);
        //message
        fgets(line2 , max , fp);
        n = strlen(line2);
        
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line2);
        //name of branch
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line2);
        //last commit number
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long last = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long file = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long seconds = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line2);
        num_of_all_commits++;
    }

    int place;
    for(int i = num_of_all_commits - 1; i >= 0 ; i--){

        if(strcmp(all_commits[i].branch , bran) == 0){
            place = i;
        }
    }

    char path2[max];
    snprintf(path2 , max , "%s\\.neogit\\commits\\%s\\%d" , neogit_folder_location , all_commits[place].branch , all_commits[place].id );
    char command[max];
    snprintf(command , max , "dir /B %s >temp.txt" , path2);
    system(command);
    strcat(path2 , "\\staged_files.txt");
    FILE *staged_path = fopen(path2 , "r");
    char all_staged_path[50][max];
    char line[max];
    int num = 0;
        fgets(line , max , staged_path);
    while (fgets(line , max , staged_path)) 
    {
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        strcpy(all_staged_path[num] , line);
        num++;
        fgets(line , max , staged_path);
    }
    fclose(staged_path);
    
    
    
    char path3[max];
    for(int i = 0 ; i < num ; i++){
        char copy[max];
        strcpy(copy , all_staged_path[i]);
        char *str = strtok(all_staged_path[i] , "\\");
        char path55[max];
        strcpy(path55 , "");
        // strcp/y(path , str);
        char name[max];
        while(str!=NULL){
            strcpy(name , str);
            if(strstr(str , ".") ==NULL)
            {
                strcat(path55 , "\\");
                strcat(path55 , str);
            }
            str = strtok(NULL, "\\");
        }
        int len = strlen(path55);
        if( path55[0] == '\\')
        {
        for(int l = 0 ; l < len ; l++){
            path55[l] = path55[l+1];
        }
        path55[len-1] = '\0';
        }
        printf("%s\n" , path55);
        char com[max];
        snprintf(com , max , "mkdir %s" , path55);
        system(com);
        char command2[max];
        snprintf(command2 , max , "copy %s\\.neogit\\commits\\%s\\%d\\%s %s" , neogit_folder_location , all_commits[place].branch , all_commits[place].id, name , copy);
        printf("%s\n" , command2);
        system(command2);
        // system("Y");
    }

    char pathh[max];
    snprintf(pathh , max , "%s\\.neogit\\head_or_not.txt" , neogit_folder_location);
    FILE *fopa = fopen(pathh , "w");
    fprintf(fopa , "HEAD");
    fclose(fopa);





}
void checkout_commit(char *commit_name){
    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line2[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line2 , max  , fp);
    while (fgets(line2 , max  , fp))
    {
        // printf("ss");
        int n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        if(strcmp(line2 , "") == 0) continue;
        char *end;
        long num = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line2);
        //message
        fgets(line2 , max , fp);
        n = strlen(line2);
        
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line2);
        //name of branch
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line2);
        //last commit number
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long last = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long file = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        long seconds = strtol(line2 , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line2 , max , fp);
        n = strlen(line2);
        if(line2[n-1] == '\n') line2[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line2);
        num_of_all_commits++;
    }

    int place;
    for(int i = num_of_all_commits - 1; i >= 0 ; i--){
        char temp[max];
        sprintf(temp , "%d" , all_commits[i].id);
        if(strcmp(temp , commit_name) == 0){
            place = i;
        }
    }

    printf("%d %d" , place , num_of_all_commits);
    char path2[max];
    snprintf(path2 , max , "%s\\.neogit\\commits\\%s\\%d" , neogit_folder_location , all_commits[place].branch , all_commits[place].id );
    char command[max];
    snprintf(command , max , "dir /B %s >temp.txt" , path2);
    system(command);
    strcat(path2 , "\\staged_files.txt");
    FILE *staged_path = fopen(path2 , "r");
    char all_staged_path[50][max];
    char line[max];
    int num = 0;
        fgets(line , max , staged_path);
    while (fgets(line , max , staged_path)) 
    {
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        strcpy(all_staged_path[num] , line);
        num++;
        fgets(line , max , staged_path);
    }
    fclose(staged_path);
    

    char path3[max];
    for(int i = 0 ; i < num ; i++){
        char copy[max];
        strcpy(copy , all_staged_path[i]);
        char *str = strtok(all_staged_path[i] , "\\");
        char path55[max];
        strcpy(path55 , "");
        // strcp/y(path , str);
        char name[max];
        while(str!=NULL){
            strcpy(name , str);
            if(strstr(str , ".") ==NULL)
            {
                strcat(path55 , "\\");
                strcat(path55 , str);
            }
            str = strtok(NULL, "\\");
        }
        int len = strlen(path55);
        if( path55[0] == '\\')
        {
        for(int l = 0 ; l < len ; l++){
            path55[l] = path55[l+1];
        }
        path55[len-1] = '\0';
        }
        printf("%s\n" , path55);
        char com[max];
        snprintf(com , max , "mkdir %s" , path55);
        system(com);
        char command2[max];
        snprintf(command2 , max , "copy %s\\.neogit\\commits\\%s\\%d\\%s %s" , neogit_folder_location , all_commits[place].branch , all_commits[place].id, name , copy);
        printf("%s\n" , command2);
        system(command2);
        // system("Y");
    }

    char pathh[max];
    snprintf(pathh , max , "%s\\.neogit\\head_or_not.txt" , neogit_folder_location);
    FILE *fopa = fopen(pathh , "w");
    fprintf(fopa , "NOT");
    fclose(fopa);

}

int is_staging_area_empty(){
    char path[max];
    snprintf(path ,max , "%d\\.neogit\\staging_area\\staged_files" , neogit_folder_location);
    FILE *staged = fopen(path , "r");
    char line[max];
    int sum = 0;
    while(fgets(line, max ,staged)){
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(line == NULL){
            break;
        }
        else
            sum++;
    }
    fclose(staged);
    return sum;
}

//run tag
int run_tag(char *argv[] , int argc){
    finding_neogit_path();

    char pp[max];
    snprintf(pp , max , "%s\\.neogit\\local_useremail.txt" , neogit_folder_location);
    FILE* useridfile = fopen(pp , "r");
    fscanf(useridfile , "%s"  , user_email);
    fclose(useridfile);
    char ss[max];
    snprintf(ss,max,"%s\\.neogit\\local_username.txt" , neogit_folder_location);
    FILE * pas = fopen(ss , "r");
    fscanf(pas, "%s" , user_name);
    fclose(pas);
    // char path[max];

    char path[max];
    struct tag all_tags[20];
    snprintf(path, max , "%s\\.neogit\\tag.txt" ,neogit_folder_location );
    FILE* file = fopen(path,"r");
    char line[max];
    int count = 0;
    fgets(line, max , file);
    while(fgets(line, max , file)){
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] ='\0';
        strcpy(all_tags[count].name ,line );
        fscanf(file , "%d" , &all_tags[count].tag_commit.id);

        fgets(line , max , file);
        fgets(line , max , file);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1] ='\0';
        strcpy(all_tags[count].message , line);
        
        fgets(line , max , file);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1] ='\0';
        strcpy(all_tags[count].tagger , line) ;
        count ++;
    }

    if(argc == 2){
        normal_tag(all_tags , count);
        return 0;
    }
    if(argc == 4 && strcmp("show" , argv[2]) == 0 ){
        find_tag(all_tags , count , argv[3]);return 0;
    }


    int dash_c = -1;
    int dash_f = -1;
    int dash_m = -1;
        printf("sakam\n");
    for(int i = 0 ; i < argc ; i++){
        if(strcmp(argv[i] , "-c") == 0)
        dash_c = i;
        if (strcmp(argv[i] , "-f") == 0 )
        dash_f = i;
        if(strcmp(argv[i] , "-m") == 0)
        dash_m = i;
    }
    int found = 0;
    for(int i = 0 ; i < count ; i++){
        if(strcmp(all_tags[i].name , argv[3]) == 0){
            found = 1;
            break;
        }
    }
    printf("%d\n" , found);
    if(dash_f == -1 && found == 1){
        printf("tag already exists\n");
        return 0;
    }
    if(dash_f != -1 && found == 1){
        printf("resid");
        replace_tag(argv[dash_m +1] , argv[3]);

    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\tag.txt" , neogit_folder_location);
    char path2[max_path_size];
    snprintf(path2 , max_path_size , "%s\\.neogit\\temp.txt" , neogit_folder_location);
    char command2[max];
    snprintf(command2 , max , "del %s" , path);
    system(command2);
    char command[max];
    snprintf(command , max , "move %s %s" , path2 , path);
    system(command);



        return 0;
    }
    if(dash_m != -1){
        strcpy(all_tags[count].message , argv[dash_m + 1]);
    }
    else all_tags[count].name == NULL;
    strcpy(all_tags[count].name , argv[3]);
    strcpy(all_tags[count].tagger , user_name);
    if(dash_c != -1){
        all_tags[count].tag_commit.id = strtoll(argv[dash_c + 1],NULL,16);
    }
    
    FILE *fp = fopen(path , "a");
    fprintf(fp,"%s\n",all_tags[count].name);
    fprintf(fp,"%d\n" , all_tags[count].tag_commit.id);
    fprintf(fp,"%s\n" , all_tags[count].message);
    fprintf(fp , "%s\n" , all_tags[count].tagger);
    fclose(fp);
}
void normal_tag(struct tag * all_tags , int count){
    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
    while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
    }
    // for(int i = 0; i < count ; i++){
    //     for(int j = i ; j < count ; j++)
    //     {
    //     if(strcmp(all_tags[j].name , all_tags[j+1].name) > 0){
    //         struct tag temp = all_tags[j];
    //         all_tags[j] = all_tags[j+1];
    //         all_tags[j+1]= temp;
    //     }
    //     }
    // }
    for(int q = 0 ; q < count ; q++){
        int place = -1;
        for(int w = 0 ; w< num_of_all_commits ; w++){
            if(all_commits[w].id == all_tags[q].tag_commit.id){
                place = w;
                break;
            }
        }
        printf("tag %s\n" , all_tags[q].name);
        printf("commit %d\n" , all_commits[place].id);
        printf("Author: %s\n" ,all_tags[q].tagger);
        printf("Date: %s\n" , all_commits[place].date);
        printf("message: %s\n" , all_tags[q].message);
        printf("------------------------\n");
    }
}
void find_tag(struct tag * all_tags , int count , char *name){

    int place_int_all_tags = -1;
    for(int i = 0 ; i<count ;i++){
        if(strcmp(all_tags[i].name , name) == 0){
            place_int_all_tags = i;
            break;
        }
    }
    int place_in_all_commits = -1;

    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\commits\\commit_massages.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    // printf("%s\n" , path);
    char line[max];
    struct commit all_commits[max];
    int num_of_all_commits = 0;
    //getting the first empty line
    fgets(line , max  , fp);
    while (fgets(line , max  , fp))
    {
        // printf("ss");
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        if(strcmp(line , "") == 0) continue;
        char *end;
        long num = strtol(line , &end , 20);
        if(num == all_tags[place_int_all_tags].tag_commit.id){
            place_in_all_commits = num_of_all_commits;
        }
        all_commits[num_of_all_commits].id = num;
        strcpy(all_commits[0].message , line);
        //message
        fgets(line , max , fp);
        n = strlen(line);
        
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].message , line);
        //name of branch
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].branch , line);
        //last commit number
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long last = strtol(line , &end , 20);
        // printf("%s\n%ld\n" , line, last);
        all_commits[num_of_all_commits].last_commit_id = last;
        //num of files
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long file = strtol(line , &end , 20);
        all_commits[num_of_all_commits].num_of_file = file;
        //seconds
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        long seconds = strtol(line , &end , 20);
        all_commits[num_of_all_commits].seconds_1970 = seconds;
        //date
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].date , line);
        // num_of_all_commits++;
        //author
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].author , line);
        //useremail
        fgets(line , max , fp);
        n = strlen(line);
        if(line[n-1] == '\n') line[n-1]= '\0';
        strcpy(all_commits[num_of_all_commits].email , line);
        // num_of_all_commits++;
        num_of_all_commits++;
    }
    // printf("%d %d %d" , place_in_all_commits , place_int_all_tags ,all_tags[place_int_all_tags].tag_commit.id);
        printf("tag %s\n" , all_tags[place_int_all_tags].name);
        printf("commit %d\n" , all_commits[place_in_all_commits].id);
        printf("Author: %s \n" ,all_tags[place_int_all_tags].tagger );
        printf("Date: %s\n" , all_commits[place_in_all_commits].date);
        printf("message: %s\n" , all_tags[place_int_all_tags].message);
        printf("------------------------\n");
}
void replace_tag(char *message , char* name){
    char path[max_path_size];
    snprintf(path , max_command_size , "%s\\.neogit\\tag.txt" , neogit_folder_location);
    FILE *fp = fopen(path, "r");
    
    char path2[max_path_size];
    snprintf(path2 , max_path_size , "%s\\.neogit\\temp.txt" , neogit_folder_location);
    FILE *temp = fopen(path2 , "w");
    
    char line[max];
    
    fgets(line , max , fp);
    fprintf(temp , "%s" , line);
    while (fgets(line , max ,fp))
    {
        int n = strlen(line);
        if(line[n-1] == '\n') line[n-1] = '\0';
        if(strcmp(line , name) == 0){
            fprintf(temp , "%s\n", name);
            fgets(line, max ,fp);
            fprintf(temp , "%s" , line);
            fgets(line , max ,fp);
            fprintf(temp ,"%s\n" , message);
            fgets(line, max ,fp);
            fprintf(temp , "%s" , line);
        }
        else {
            fprintf(temp , "%s\n" , line);
        }
    }
    fclose(temp);
    fclose(fp);
    // char command2[max];
    // snprintf(command2 , max , "del %s" , path);
    // system(command2);
    // char command[max];
    // snprintf(command , max , "move %s %s" , path2 , path);
    // system(command);
}


//grep
int run_grep(char *argv[] , int argc){
    FILE *file = fopen(argv[3] , "r");
    int dash_n = -1;
    for(int i = 0; i<argc ;i++){
        if(strcmp("-n" , argv[i])== 0)
        dash_n = i;
    }
    char line[max];
    int num_of_line = 0;
    while(fgets(line , max , file) != NULL){
        num_of_line++;
        if(strstr(line , argv[5]) != NULL){
            if(dash_n != -1){
                printf("%d . %s\n" , num_of_line , line);
            }
        }
    }
}
//pre commit
int run_precommit(char *argv[] , int argc){
    finding_neogit_path();
    if(strcmp("hooks" , argv[2]) == 0 && strcmp("list" , argv[3]) == 0 ){
        char path[max];
        snprintf(path , max ,"%s\\.neogit\\hooks\\all_hooks.txt" , neogit_folder_location);
        FILE * file =fopen(path , "r");
        char line[max];
        while(fgets(line , max , file)){
            printf(line);
        }
    }

    if(strcmp("hooks" , argv[3]) == 0 && strcmp("applied" , argv[2]) == 0){
        char path[max];
        snprintf(path , max ,"%s\\.neogit\\hooks\\all_hooks.txt" , neogit_folder_location);
        FILE * file =fopen(path , "r");
        char line[max];
        printf("applied hooks:\n");
        while(fgets(line , max , file)){
            char path2[max];
            int n = strlen(line);
            if(line[n-1] == '\n') line[n-1] = '\0';
            snprintf(path2 , max ,"%s\\.neogit\\hooks\\%s" , neogit_folder_location , line);
            FILE *fp = fopen(path2 , "r");
            // int n;
            fscanf(fp , "%d" ,n);
            if(n == 1) printf("%s\n" , line); 
        }
    }

    if(strcmp("add" , argv[2]) == 0 && strcmp("hook" , argv[3]) == 0){
        char path[max];
        snprintf(path , max, "%s\\.neogit\\hooks\\%s.txt" , neogit_folder_location ,   argv[4]);
        FILE* file=fopen(path , "w");
        fprintf(file , "1\n");
        fclose(file);
        return 0;
    }
    if(strcmp("remove" , argv[2]) == 0 && strcmp("hook" , argv[3]) == 0){
        char path[max];
        snprintf(path , max, "%s\\.neogit\\hooks\\%s.txt" , neogit_folder_location ,   argv[4]);
        FILE* file=fopen(path , "w");
        fprintf(file , "0\n");
        fclose(file);
        return 0;
    }
    if(argc == 2){
        char command[max];
        snprintf(command, max , "chdir %s\\.neogit\\staging_area" , neogit_folder_location );
        system(command);
        char command2[max];
        snprintf(command2 , max , "dir /B > temp.txt");
        system(command2);
        FILE * file = fopen( "temp.txt", "r") ;
        char line[max];
        while(fgets(line , max , file)) {
            int n = strlen(line);
            if(line[n-1] == '\n') line[n-1] = '\0';
            if(strcmp(line , "staged_files.txt") == 0 || strcmp(line , "unstaged_files.txt")== 0) continue;
            char path[max];
            snprintf(path , max ,"%s\\.neogit\\staging_area\\%s" , neogit_folder_location , line);
            
            printf("%s:\n" , line);

            char path5[max];
            snprintf(path5 , max ,"%s\\.neogit\\hooks\\all_hooks.txt" , neogit_folder_location);
            FILE * file =fopen(path5 , "r");
            char line[max];
            printf("applied hooks:\n");
            while(fgets(line , max , file)){
                char path2[max];
                int n = strlen(line);
                if(line[n-1] == '\n') line[n-1] = '\0';
                snprintf(path2 , max ,"%s\\.neogit\\hooks\\%s" , neogit_folder_location , line);
                FILE *fp = fopen(path2 , "r");
                // int n;
                fscanf(fp , "%d" ,&n);
                if(n == 1){
                    if(strcmp(line , "eof_blank_space.txt") == 0){
                        if(eof_blank_space(path) == 1){
                            printf("eof_blank_space : passed\n");
                        }
                        else{
                            printf("eof_blank_space : failed\n");
                        }
                    }
                    if(strcmp(line , "character_limit.txt") == 0){
                        if(more_than_2000(path) == 1){
                            printf("character_limit : passed \n");
                        }
                        else printf("character_limit : failed \n");
                    }
                    if(strcmp(line , "file_size_check.txt") == 0){
                        if(checkFileSize(path) == 1){
                            printf("file-size-check : passed\n");
                        }
                        else printf("file-size-check : failed \n");
                    }
                    if(strcmp(line , "todo_check.txt") == 0){
                        if(TODO_file(path) == 1){
                            printf( "todo_check : passed\n");
                        }
                        else{
                            printf("todo_ckeck : failed\n");
                        }
                    }
                } 
                    
            }
        }
    }
}
int eof_blank_space(char *path){
    const char *extension = strrchr(path, '.'); 
    if (extension == NULL || (strcmp(extension , ".cpp") != 0 && strcmp(extension , ".c") != 0 && strcpy(extension , ".txt")!=0)) { 
        printf("eof_blank_space : wrong format");
        return 0; 
    }
        FILE *file = fopen(path, "r"); 
        char buffer[1000]; 
 
 
        fseek(file, 0, SEEK_END); 
        int len = ftell(file); 
        fseek(file, 0, SEEK_SET); 
 
        if (len > 0) { 
            fread(buffer, 1, len, file); 
            while (len > 0 && isspace(buffer[len - 1])) { 
                len--; 
            } 
            fclose(file); 
            if (len > 0) { 
                return -1; 
            } else { 
                return 1;
            } 
        } 
 
        fclose(file); 
        return -1;
}
int more_than_2000(const char *filename) { 
    const char *extension = strrchr(filename, '.'); 
    if (extension == NULL) { 
        return 0; 
    } 
    char lowercaseExtension[10]; 
    int i; 
    for (i = 0; i < 10 && extension[i] != '\0'; i++) { 
        lowercaseExtension[i] = tolower(extension[i]); 
    } 
    lowercaseExtension[i] = '\0'; 
    if (strcmp(lowercaseExtension, ".txt") == 0 ||  strcmp(lowercaseExtension, ".c") == 0 || strcmp(lowercaseExtension, ".cpp") == 0) { 
        FILE *file = fopen(filename, "r"); 
        fseek(file, 0, SEEK_END); 
        long fileSize = ftell(file); 
        fclose(file); 
        if (fileSize > 20000) { 
            return -1; 
        } else { 
            return 1;
        } 
    } else { 
        return 0;
    } 
}
int checkFileSize(const char *filename) { 

    FILE *file = fopen(filename, "rb"); 

    fseek(file, 0, SEEK_END); 
    long fileSize = ftell(file); 
    fclose(file); 
 
    if (fileSize > 5 * 1024 * 1024) {
        return -1; 
    } else { 
        return 1; 
    } 
}
int TODO_file(const char *filename) 
 { 
    const char *extension = strrchr(filename, '.'); 
    char lowercase[10]; 
    int i; 
    for (i = 0; i < 10 && extension[i] != '\0'; i++) { 
        lowercase[i] = tolower(extension[i]); 
    } 
    lowercase[i] = '\0'; 
 
    if (strcmp(lowercase, ".c") == 0 ||  strcmp(lowercase, ".cpp") == 0 || strcmp(lowercase, ".text") == 0) { 
        FILE *file = fopen(filename, "r"); 

        char buffer[1000]; 
        while (fgets(buffer, sizeof(buffer), file) != NULL) { 
            if (strstr(buffer, "todo") != NULL || strstr(buffer, "TODO") != NULL) { 
                fclose(file); 
                return -1; 
            } 
        } 
 
        fclose(file); 
        return 1;  
    } 
 
    return 0;
}
