#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h> 
#include <sys/wait.h>

void first_Print()
{
   printf("first_Print\n");
}

void second_Print()
{
   first_Print();
   printf("second_Print\n");
}

void third_Print()
{
   printf("third_Print\n");
   second_Print();
}

typedef enum
{
   TEST_FORK,
   TEST_VFORK,
   TEST_FORK_EXECVPE,
   TEST_VFORK_EXECVPE,
   TEST_FORK_EXECVPE2,
   TEST_VFORK_EXECVPE2,
   TEST_EXEC,
   TEST_SIGNAL,
   TEST_EXEC_1,
   TEST_EXEC_2
} command;


int main(int argc , char *argv[], char *envp[])
{
     unsigned int cmd;
     unsigned int pid, status, es;
     char *args[3] = {"/bin/ls", "-l", NULL};

     char * args_vfork[3] = {"clang", "main.c", NULL};
     if (argc != 2)
     {
        printf("Must supply one argument to the call %s\n", argv[0]);
     }

     cmd = atoi(argv[1]);
     
     if (cmd == TEST_FORK)
     {
         pid = fork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             third_Print();
             fflush(stdout);
             exit(EXIT_SUCCESS);
             
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }

     if (cmd == TEST_VFORK)
     {
         pid = vfork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             third_Print();
             fflush(stdout);
             exit(EXIT_SUCCESS);
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }

     if (cmd == TEST_FORK_EXECVPE)
     {
         pid = fork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             fflush(stdout);
             execvpe(args[0], args, envp);
             
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }


     if (cmd == TEST_VFORK_EXECVPE)
     {
         pid = vfork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             fflush(stdout);
             execvpe(args[0], args, envp);
             
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }

    if (cmd == TEST_FORK_EXECVPE2)
     {
         pid = fork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             fflush(stdout);
             execvpe("/home/victor/bin/clang", args_vfork, envp);
             execvpe("/home/victor/bin/clang", args_vfork, envp);
             exit(EXIT_FAILURE);
             
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }

    if (cmd == TEST_VFORK_EXECVPE2)
     {
         pid = vfork();
         if (pid == 0)
         {
             printf("We are in the child\n");
             fflush(stdout);
             execvpe("/home/victor/bin/clang", args_vfork, envp);
             execvpe("/home/victor/bin/clang", args_vfork, envp);
             exit(EXIT_FAILURE);
             
         } 
         else if (pid == -1)
         {

              fprintf(stderr, "Error while forking\n");
              fflush(stderr);

	 }
         else
         {
                  while (waitpid(pid, &status, 0) == -1) 
	          {
            		if (errno != EINTR) 
		        {      
                	      fprintf(stderr, "Wait broke with something else then EINTR\n");
                	      break;                 
            		}
                  }
		  if ( WIFEXITED(status)) 
                  {
                          es = WEXITSTATUS(status);
       			  if (es == EXIT_FAILURE)
			  {

				fprintf(stderr, "Child terminated with EXIT_FAILURE\n");
				fflush(stderr);

			  }
                          else if (es == EXIT_SUCCESS)
                          {
                                fprintf(stderr, "Child terminated normally\n");
				fflush(stderr);
                          }
                  }
         }
     }
    
     if (cmd == TEST_EXEC)
     {
        execvpe("/home/victor/bin/clang", args_vfork, envp);
     }
     printf("Finish Testing\n");

             
}
