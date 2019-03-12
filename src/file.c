
#include "file.h"



void info(char * file_path, int8_t flags, char * hash[], int8_t hash_flags){

  struct stat sb;
  if (lstat(file_path, &sb) == -1) {
    perror("lstat");
    exit(EXIT_FAILURE);
  }



  int link[2];
  char temp[4096];

  if(pipe(link) == -1)
    exit(EXIT_FAILURE);

  if(S_ISREG(sb.st_mode)){

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


      write(STDOUT_FILENO,temp,nbytes-1);

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
    printf(", %lld",(long long) sb.st_size); //size
    printf(", %x", sb.st_mode);
    printf(", %x", sb.st_mode & 0x0777 ); //permisions
    printf(", " );
    //print permissions
    for(int x = 0x00400,i = 0;x!=0;x>>=1,i++){
      if(sb.st_mode & x){
        switch (i%3) {
          case 0:
            printf("r");
          break;
          case 1:
            printf("w");
          break;
          case 2:
            printf("x");
          break;
          default:
          break;
        }
      }
      else
          printf("-");
      if(i%3==2)
        x>>=1;

    }

    //printf(", %s",  asctime(gmtime(&sb.st_mtim.tv_sec))); //modification
    printf(", ");
    char date[21];
    strftime(date, 21, "%Y-%m-%dT%H:%M:%S", localtime(&(sb.st_mtime))); //modification
    printf("%s",date );
    printf(", ");
    strftime(date, 21, "%Y-%m-%dT%H:%M:%S", localtime(&(sb.st_ctime))); //status
    printf("%s",date );
    printf("\n" );



  }
}
