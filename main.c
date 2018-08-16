#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/sysinfo.h>
#include <stdbool.h>
#include <unistd.h>

bool reset;

void read_input(int byteAmount, char *infile) //https://stackoverflow.com/questions/21692936
{
	char ch;
	int count = 0;
	FILE *fp;
	fp = fopen(infile, "r");
	if (fp == NULL)
	{
		fprintf(stderr, "Error while opening the file.\n");
		exit(EXIT_FAILURE);
	}

	while ((ch = fgetc(fp)) != EOF || count < byteAmount)
	{
		count++;
	}
	fclose(fp);
}

void *mice_thread(void *p)
{
	while(1)
	{
		read_input(1, "/dev/input/mice");
		reset = true;
		sleep(1);
	}
}

void *keyboard_thread(void *p)
{
        while(1)
        {
                read_input(1, p);
                reset = true;
                sleep(1);
        }
}

int main(int argc, char **argv)
{
	if (argc != 3)
	{
		fprintf(stderr, "usage: dpms <OffTime> <Path to keyboard dev>\n");
		exit(EXIT_FAILURE);
	}

	uid_t uid=getuid();
	if (uid != 0)
	{
		fprintf(stderr, "Should be run as root\n");
		exit(EXIT_FAILURE);
	}

	int OffTime = atoi(argv[1]);
	char *keyboard_path = argv[2];
	
	long timer, prevtime;
	struct sysinfo s_info;
	sysinfo(&s_info);
	prevtime = s_info.uptime;

	pthread_t thread1, thread2;
	int  ret1, ret2;

	ret1 = pthread_create(&thread1, NULL, mice_thread, NULL);
	ret2 = pthread_create(&thread2, NULL, keyboard_thread, (char*) keyboard_path);

	while(1)
        {
                sysinfo(&s_info);
                timer = s_info.uptime - prevtime;

                if (reset)
                {
                        prevtime = s_info.uptime;
                        reset = false;
                }

                if (timer >= OffTime)
                        system("xset dpms force off  &>/dev/null");

                sleep(10);
        }
	return(0);
}

