# Writing Your Own Unix Shell

## 说明

- 这个lab的主要任务是编写一个shell程序，实现基本的shell功能，包括：
1. 提供内置的job控制指令(jobs, bg <job>, fg <job>)，以及退出(quit)。
2. 除了上述四个指令以外，其他命令以运行相应的可执行文件的形式执行。
3. 内置的四个指令直接在shell进程中执行，其他指令通过fork子进程的方式执行。

- 同时，这个shell需要提供job control的能力。shell进程需要维护一个全局的job列表，管理其中job的运行、休眠、终止后的回收。

- 这个lab的重点和难点在于并发进程的控制，包括信号发送、信号接收、信号处理和进程状态维护。

- 要做好上述几个环节的处理，必须认真阅读并理解第八章异常控制流的相关内容。引用lab指南的原话就是"Read every word of Chapter 8 (Exceptional Control Flow) in your textbook."。

- 要注意的一点是，本shell将来也会在linux内置的shell中作为一个子进程运行，因此要注意进程树、进程组和信号的关系。

## 分析与实现
- tsh.c中已提供shell程序的框架代码，需要完成的是eval、builtin cmd、do bgfg、waitfg、sigchld handler、sigint handler、sigtstp handler七个函数实现。
- 这个shell需要维护一个全局的job列表，其中每个job的数据结构是：
```c
struct job_t {              /* The job struct */
    pid_t pid;              /* job PID */
    int jid;                /* job ID [1, 2, ...] */
    int state;              /* UNDEF, BG, FG, or ST */
    char cmdline[MAXLINE];  /* command line */
};
```
- 命令行参数的接收和处理已在main()中提供，主要处理逻辑将在eval函数中实现。
### eval
- 这个函数主要实现命令的解析和执行。

- 首先如果是内置指令，则直接在builtin_cmd中执行。如果builtin_cmd返回为0，则以fork子进程execve的方式执行。

- 在fork之前需要注意的是，必须阻塞SIGCHLD信号，否则会形成竞争条件。在fork之后，在原进程和子进程分别取消对SIGCHLD信号的阻塞。

- 由于信号的接收和处理是基于进程组的，因此在子进程中，取消对SIGCHLD信号的阻塞之前，必须用setpgid使子进程的groupId区别于原进程，否则原进程也会接收子进程的信号，这显然是错误的。

- 当命令的结尾没有"&"时，该命令作为前台命令执行，shell进程必须等待命令子进程的结束。反之，不以"&"结尾的命令以后台job的形式执行，shell进程返回准备接收下一条指令。

```c
/* 
 * eval - Evaluate the command line that the user has just typed in
 * 
 * If the user has requested a built-in command (quit, jobs, bg or fg)
 * then execute it immediately. Otherwise, fork a child process and
 * run the job in the context of the child. If the job is running in
 * the foreground, wait for it to terminate and then return.  Note:
 * each child process must have a unique process group ID so that our
 * background children don't receive SIGINT (SIGTSTP) from the kernel
 * when we type ctrl-c (ctrl-z) at the keyboard.  
*/
void eval(char *cmdline) {

  char *argv[MAXARGS];
  char buf[MAXLINE];
  int bg;

  strcpy(buf, cmdline);
  bg = parseline(buf, argv);
  if (argv[0] == NULL)
    return;

  if (builtin_cmd(argv) == 0) {

    //block SIGCHLD signal
    sigset_t mask;
    sigemptyset(&mask);
    sigaddset(&mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &mask, NULL);

    pid_t pid = fork();
    if (pid == 0) {
      setpgid(0, 0);
      sigprocmask(SIG_UNBLOCK, &mask, NULL);
      if (execve(argv[0], argv, environ) < 0) {
        printf("%s: Command not found.\n", argv[0]);
        exit(0);
      }
    }

    int state = bg == 0 ? FG : BG;
    addjob(jobs, pid, state, cmdline);
    sigprocmask(SIG_UNBLOCK, &mask, NULL);

    if (bg == 0) {
      waitfg(pid);
      //printf("the foreground process %d is stopped\n",pid);
    } else printf("[%d] (%d) %s", pid2jid(pid), pid, cmdline);
  }
  return;
}
```
### builtin_cmd
- builtin_cmd函数检查命令是否为内置命令，如果是则直接执行，处理完成后返回1，如果不是则返回0。
- 实现较为简单，内置命令主要调用其他辅助函数完成。
```c
/* 
 * builtin_cmd - If the user has typed a built-in command then execute
 *    it immediately.  
 */
int builtin_cmd(char **argv) {
  if (strcmp(argv[0], "quit") == 0) {
    exit(0);
  } else if (strcmp(argv[0], "jobs") == 0) {
    listjobs(jobs);
    return 1;
  } else if (strcmp(argv[0], "bg") == 0) {
    do_bgfg(argv);
    return 1;
  } else if (strcmp(argv[0], "fg") == 0) {
    do_bgfg(argv);
    return 1;
  }
  return 0;     /* not a builtin command */
}
```
### do_dgfg
- do_dgfg主要完成内置的 bg <job>, fg <job> 两个命令，包括对命令的解析的参数检查。
- <job>参数可以是pid,或者以'%'开头的jobId。
- 利用系统调用kill向某个已停止的job发送SIGCONT信号，将其唤醒。
- 对于bg，在全局jobs数组中将该job唤醒，并将其state设为BG。
- 对于fg，在全局jobs数组中将该job唤醒，将其state设为FG， 并调用waitfg等待其结束。
- 特别要注意的是kill系统调用的第一个参数pid为负数的话，表示将该发送给|pid|所在进程组的每一个进程。由于这里的job有可能fork多个子进程来执行任务，因此调用kill时传入参数-pid。
```c
/* 
 * do_bgfg - Execute the builtin bg and fg commands
 */
void do_bgfg(char **argv) {
  struct job_t *job;
  if (argv[1] == NULL) {
    printf("%s command requires PID or %%jobid argument\n", argv[0]);
    return;
  }
  if (*argv[1] == '%') {
    if (strspn(argv[1] + 1, "0123456789") != strlen(argv[1] + 1)) {
      printf("%s: argument must be a PID or %%jobid\n", argv[0]);
      return;
    }
    job = getjobjid(jobs, atoi(argv[1] + 1));
    if (job == NULL) {
      printf("%s: No such job\n", argv[1]);
      return;
    }
  } else {
    if (strspn(argv[1], "0123456789") != strlen(argv[1])) {
      printf("%s: argument must be a PID or %%jobid\n", argv[0]);
      return;
    }
    job = getjobpid(jobs, atoi(argv[1]));
    if (job == NULL) {
      printf("(%s): No such process\n", argv[1]);
      return;
    }
  }

  if (strcmp(argv[0], "bg") == 0) {
    job->state = BG;
    kill(-job->pid, SIGCONT);
    printf("[%d] (%d) %s", job->jid, job->pid, job->cmdline);
  } else if (strcmp(argv[0], "fg") == 0) {
    job->state = FG;
    kill(-job->pid, SIGCONT);
    waitfg(job->pid);
  }
  return;
}
```

### waitfg
- 等待某个前台job结束。
- 这里用忙等待+sleep的形式实现，原则上用waitpid也可以实现，但是对job的回收最好一起在sigchld_handler中实现。
- 主要是判断job是否已被回收或者已被signal handler改变state。
```c
/* 
 * waitfg - Block until process pid is no longer the foreground process
 */
void waitfg(pid_t pid) {
  while (1) {
    struct job_t *job;
    job = getjobpid(jobs, pid);
    if (job == NULL || job->state != FG) break;
    //printf("the foreground child process (%d) is still running\n",pid);
    //listjobs(jobs);
    sleep(1);
  }
  return;
}
```

### sigchld_handler
- SIGCHLD处理函数。
- 当某个子进程终止或者停止时shell进程会接收到这个信号。
- 对于正常退出的子进程，只需要在全局job列表中删除对应的job。
- 对于因为用户按下ctrl-c而终止的子进程，需要在全局job列表中删除对应的job。需要注意的是，这里的SIGINT信号实际上是用户按下ctrl-c后，系统发送给shell进程的，然后由sigint_handler函数转发给了当前前台job对应的子进程组。
- 对于因为用户按下ctrl-z而停止的子进程，只需要在全局job列表中使对应的job的state置为ST。需要注意的是，这里的SIGTSTP信号实际上是用户按下ctrl-z后，系统发送给shell进程的，然后由sigtstp_handler函数转发给了当前前台job对应的子进程组。
```c
/* 
 * sigchld_handler - The kernel sends a SIGCHLD to the shell whenever
 *     a child job terminates (becomes a zombie), or stops because it
 *     received a SIGSTOP or SIGTSTP signal. The handler reaps all
 *     available zombie children, but doesn't wait for any other
 *     currently running children to terminate.  
 */
void sigchld_handler(int sig) {
  int i;
  //printf("get SIGCHLD : %d\n",sig);
  for (i = 0; i < MAXJOBS; i++) {
    int status;
    if (jobs[i].pid != 0 && waitpid(jobs[i].pid, &status, WNOHANG | WUNTRACED) != 0) {
      //printf("the status of waitpid %d is %d\n",jobs[i].pid,status);
      if (WIFEXITED(status)) {
        deletejob(jobs, jobs[i].pid);
      } else if (WIFSIGNALED(status)) {
        printf("Job [%d] (%d) terminated by signal 2\n", jobs[i].jid, jobs[i].pid);
        deletejob(jobs, jobs[i].pid);
      } else if (WIFSTOPPED(status)) {
        jobs[i].state = ST;
        printf("Job [%d] (%d) stopped by signal 20\n", jobs[i].jid, jobs[i].pid);
      }
    }
  }
  return;
}
```

### sigint_handler
- SIGINT处理函数，这里用户实际想要终止的是处于前台运行的job，因此将该信号转发给对应的子进程组。
- job的回收将在sigchld_handler中进行。
- 特别要注意的是kill系统调用的第一个参数pid为负数的话，表示将该发送给|pid|所在进程组的每一个进程。由于这里的前台job有可能fork多个子进程来执行任务，因此调用kill时传入参数-fpid。
```c
/* 
 * sigint_handler - The kernel sends a SIGINT to the shell whenver the
 *    user types ctrl-c at the keyboard.  Catch it and send it along
 *    to the foreground job.  
 */
void sigint_handler(int sig) {
  pid_t fpid = fgpid(jobs);
  if (fpid != 0) {
    kill(-fpid, SIGINT);
  }
  return;
}
```

### sigtstp_handler
- SIGTSTP处理函数，这里用户实际想要停止的是处于前台运行的job，因此将该信号转发给对应的子进程组。
- job的状态处理将在sigchld_handler中进行。
- 特别要注意的是kill系统调用的第一个参数pid为负数的话，表示将该发送给|pid|所在进程组的每一个进程。由于这里的前台job有可能fork多个子进程来执行任务，因此调用kill时传入参数-fpid。
```c
/*
 * sigtstp_handler - The kernel sends a SIGTSTP to the shell whenever
 *     the user types ctrl-z at the keyboard. Catch it and suspend the
 *     foreground job by sending it a SIGTSTP.  
 */
void sigtstp_handler(int sig) {
  pid_t fpid = fgpid(jobs);
  if (fpid != 0) {
    kill(-fpid, SIGTSTP);
  }
  return;
}
```