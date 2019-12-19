#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

void at_exit_handler(void);

int main()
{
    int exitCode = 0;
    pid_t pid;
    switch(pid = fork())
    {
        case -1:
            perror("Ошибка!");
            exit(1);
        case 0:
            printf("CHILD: Это процесс-потомок!\n");
            printf("CHILD: Мой PID -- %d\n", getpid());
            printf("CHILD: PID моего родителя -- %d\n", getppid());
            printf("Введите мой код возврата:");
            scanf("%d", &exitCode);
            printf("CHILD:Выход!\n");
            atexit(at_exit_handler);
            exit(exitCode);
        default:
            printf("PARENT: Это процесс родитель!\n");
            printf("PARENT:Мой PID -- %d\n", getpid());
            printf("PARENT:PID моего потомка -- %d\n", pid);
            printf("PARENT: Я жду, пока потомок не вызовет exit()...\n");
            waitpid(pid, &exitCode, 0);           
            printf("PARENT: Код возврата моего потомка %d\n", WEXITSTATUS(exitCode));
            printf("PARENT:Выход!\n");
            atexit(at_exit_handler);  
    }
    return 0;

}

void at_exit_handler(void)
{
    printf("atexit handler pid() = %d\n", getpid());
}
