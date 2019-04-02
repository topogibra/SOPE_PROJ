#include "fingerprinter.h"
#include "file.h"

static int8_t num_flags = 0;
static char * hash_passed[3];

/**
 * Sets global file variables representing the number of hashes passed to the
 * program and an array of of string with each hash.
 * @param hash array with the strings of each hash passed to the program
 * @param hash_flags number of hashes passed to the function
 */
void setFlags(char * hash[], int8_t hash_flags){
  num_flags = hash_flags;
  memcpy(hash_passed,hash,sizeof(char *) * num_flags);
}

/**
  * Displays files system info.
  * @param sb pointer to a struct stat of a file
  */
char * display_stat_info(struct stat *sb){
  char * str = malloc(sizeof(char) * 512);
  sprintf(str,"%lld, %x, %x, ",(long long) sb->st_size,sb->st_mode,sb->st_mode & 0x0777);
  for(int x = 0x00400,i = 0;x!=0;x>>=1,i++){   //print permissions
    if(sb->st_mode & x){
      switch (i%3) {
        case 0:
          strcat(str,"r");
          break;
          case 1 : strcat(str, "w");
          break;
        case 2:
          strcat(str,"x");
        break;
        default:
        break;
      }
    }
    else
        strcat(str,"-");
    if(i%3==2)
      x>>=1;
  }
  strcat(str,", ");
  char date[21];
  strftime(date, 21, "%Y-%m-%dT%H:%M:%S", localtime(&(sb->st_mtime))); //modification
  strcat(str,date);
  strcat(str,", ");
  strftime(date, 21, "%Y-%m-%dT%H:%M:%S", localtime(&(sb->st_ctime))); //status
  strcat(str,date);
  return str;
}

/**
  * Displays file fingerprints.
  * @param file_path file path of the file
  */
char * display_fingerprints(char * file_path){
  char * hash = malloc(sizeof(char) * 512);
  char * temp = NULL;
  for(int i = 0; i < num_flags;++i){
    if(!strcmp(hash_passed[i],"md5"))
      temp = gen_checksum(file_path,"md5sum");
    else if(!strcmp(hash_passed[i],"sha1"))
      temp = gen_checksum(file_path,"sha1sum");
    else if(!strcmp(hash_passed[i],"sha256"))
      temp = gen_checksum(file_path,"sha256sum");
    if(temp != NULL){
      strcat(hash,", ");
      strcat(hash,temp);
      free(temp);
      temp = NULL;
    }
  }
  return hash;
}

/**
  * Displays file type info.
  * @param file_path file path of the file
  */
char * display_file_type(char * file_path){
  int link[2];
  char temp[128];

  char * str = NULL;

  if(pipe(link) == -1)
    exit(EXIT_FAILURE);

  pid_t pid;
  int status;

  pid = fork();
  if(pid < 0){ // error
    perror("fork");
    exit(1);
  }else if(pid != 0){ // pai

    close(link[1]);
    waitpid(pid,&status,0);

    int nbytes = 0;
    int n_aux = 0;
    while((n_aux = read(link[0],temp + nbytes,sizeof(temp) - nbytes))>0){
      nbytes+=n_aux;
    }

    //write(STDOUT_FILENO,temp,nbytes-1);
    str = malloc(sizeof(char) * nbytes);
    memcpy(str,temp,sizeof(char) * (nbytes-1));
    str[nbytes-1] = '\0';
    close(link[0]);
    return str;

  }else{ //filho
    if(dup2(link[1],STDOUT_FILENO)==-1){
      perror("dup2");
      exit(1);
    }
    close(link[0]);
    close(link[1]);
    execlp("file","file",file_path,NULL);
    exit(1);
  }
  return str;
}


/**
 * Get file system status and open child process that redirects its output to
 * the father to get the file types. Then it displays its info and the fingerprints
 * specified by the file global variables.
 * @param path of the file in which the fuction will be applied
 */
void info(char * file_path){

  struct stat sb;
  if (lstat(file_path, &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }

  if(S_ISREG(sb.st_mode)){
    char * str = malloc(sizeof(char) * 1024);
    char * temp = NULL;

    temp = display_file_type(file_path);
    strcat(str,temp);
    free(temp);

    strcat(str,", ");

    temp = display_stat_info(&sb);
    strcat(str,temp);
    free(temp);

    temp = display_fingerprints(file_path);
    strcat(str,temp);
    free(temp);

    printf("%s\n",str );

    free(str);

  }
}
