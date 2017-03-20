#include "client.h"
void godaemon(void)
{
    int fs;

    printf("Initializing daemon mode\n");

    if (getppid() != 1)
    {
        fs = fork();

        if (fs > 0)
            exit(0); // parent

        if (fs < 0)
        {
            perror("fork");
            exit(1);
        }

        setsid();
    }

    // redirect stdin/stdout/stderr to /dev/null
    close(0);
    close(1);
    close(2);

    open("/dev/null", O_RDWR);
    int ret = dup(0);
    ret = dup(0);
    umask(0);

    return;
}

void monitor_mode()
{
    pid_t pid;
    while(1)
    {
        pid = fork();
        if(pid == 0)
            return;
        if(waitpid(pid, NULL, 0) == pid)
        {
            time_t now;
            struct tm *timenow;
            char strtemp[255];

            time(&now);
            timenow = localtime(&now);
            printf("child exit!recent time is : %s\n", asctime(timenow) );
            sleep(3);
        }
        else
            exit(1);
    }
}

int main(int argc, char *argv[])
{
	string addr = "";
	boost::shared_ptr<client> v(new client(grpc::CreateChannel(addr.c_str(), grpc::InsecureChannelCredentials())));
	v->start();
    return 0;
}

