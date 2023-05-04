```cpp
#include <unistd.h>
#include <fcntl.h>

int main(){
    int p[2];
    pipe(p);
    int pid;
    pid = fork();
    if(pid == 0){
        close(0);
        dup(p[0]);
        close(p[0]);
        close(p[1]);
        execl("/bin/wc", "wc", 0, NULL);
        //execl("/bin/wc", "wc", 0, NULL);
    }else{
        close(p[0]);
        write(p[1],"hello world\n",12);
        close(p[1]);
    }

}
```

    pip是一个管道，需要传入长度2的int数组，int[0]传出来输入端fd，int[1]传出输出端fd

1、输出端的fd关闭后，读端才可以读

2、在linux中如```cpp grep fork sh.c | wc -l```

   子进程创建一个管道来连接管道的左端和右端。 然后它为管道的左端调用 fork 和 runcmd，为右端调用 fork 和 runcmd，并等待两者完成。 管道的右端可能是一个命令，它本身包含一个管道（例如，a | b | c），它本身会派生两个新的子进程（一个用于 b，一个用于 c）。 因此，shell 可以创建一个进程树。 这棵树的叶子是命令，内部节点是等待左右子节点完成的进程。

左边的结果输入到右边中去