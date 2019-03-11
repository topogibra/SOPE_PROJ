#include "parser.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>

#include <time.h>
#include <sys/sysmacros.h>

#define NO_FLAGS 3

void info(char * file, int8_t flags, char * hash[], int8_t hash_flags){

  struct stat sb;
  if (lstat(file, &sb) == -1) {
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
      int nbytes = read(link[0],temp,sizeof(temp));
      write(STDOUT_FILENO,temp,nbytes-1);
      wait(&status);

    }else{ //filho
      dup2(link[1],STDOUT_FILENO);
      close(link[0]);
      close(link[1]);
      execl("/usr/bin/file","/usr/bin/file",file,NULL);
      exit(0);
    }
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

    printf(", %lld",(long long) sb.st_size); //size
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



int main(int argc, char *argv[]) {
    char *flagArguments[NO_FLAGS];
    for(size_t i = 0; i < NO_FLAGS; i++)
    {
        flagArguments[i] = "";
    }

    uint8_t flags = parseArguments(argc, argv, flagArguments, NO_FLAGS);

    uint8_t hash_flags = 0;
    char * hash[3];

    if(flags & CLCHASH){
      hash_flags = getHashArguments(flagArguments[0],hash);
      /*printf("Hashes : \n");
      for(int i = 0; i<hash_flags;i++){
        printf("%s\n",hash[i]);
      }*/
    }

    int output = STDOUT_FILENO;

    if(flags & SAVECSV){
      if((output = open(flagArguments[1], O_WRONLY | O_CREAT | O_TRUNC , 0777 ))==-1){
        perror("open");
        exit(1);
      }
      dup2(output,STDOUT_FILENO);
    }


    info(flagArguments[NO_FLAGS-1],flags,hash,hash_flags);


    return 0;
}
