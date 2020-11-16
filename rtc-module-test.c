#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

int main(int argc, char * argv[]){
    char str[10];
    int size = 8;
    int fd = open("/proc/clock", O_RDWR);
    
    printf("Choose one of the following options or enter a time to change the RTC with the following format 'HH:MM:SS'.\n");
    printf("r -> Read the Real Time Clock (RTC).\n");
    printf("t -> Get the linux provided time.\n");
    printf("q -> Exit the program.\n\n");

    scanf("%s", str);    
    while(str[0] != 'q'){
        lseek(fd, 0, SEEK_SET);
        if(str[0] == 'r'){
            char buffer[8];
            printf("\nRTC time:\n");
            read(fd, buffer, 100);
            int count = 0;
            char ch = buffer[count];
            
            while(count < size){
                printf("%c", ch);
                count++;
                ch = buffer[count];
            }
            printf("\n");

        }else if(str[0] == 't'){
            struct timeval tv;
            struct timezone tz;
            time_t rawtime;
            struct tm * currenttime;
            gettimeofday(&tv, &tz); 
            rawtime = tv.tv_sec;
            currenttime = localtime(&rawtime);
            printf("\nLinux provided time:\n");
            printf("%d:%d:%d\n\n", currenttime->tm_hour, currenttime->tm_min, currenttime->tm_sec);

        }else{
            int len = strlen(str);
            write(fd, str, len);
            printf("RTC updated!\n\n");
            size = len;
        }

        printf("\nChoose one of the following options or enter a time to change the RTC with the following format 'HH:MM:SS'.\n");
        printf("r -> Read the Real Time Clock (RTC).\n");
        printf("t -> Get the linux provided time.\n");
        printf("q -> Exit the program.\n\n");
        scanf("%s", str);
    }

    printf("\n\nEnd of Processing.\n");
    printf("Programmed by Manasseh Banda.\n");

    return 0;
}