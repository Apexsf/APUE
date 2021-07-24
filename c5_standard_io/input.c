#include <stdio.h>

struct foo{
    int age;
    int id;
    float height;
};

int main(){
    char buf[20000];
    FILE* fd_w = fopen("data","w");
    if(fwrite(buf, sizeof(struct foo), 1000, fd_w)!=1000){  //sizeof(struct foo) = 12
        printf("write wrong!\n");
    }
    fclose(fd_w);
    FILE* fd_r = fopen("data","r");
    for(int i = 0 ; i < 20; i++){
        fread(buf + sizeof(struct foo) * i, sizeof(struct foo), 10, fd_r);
    }
    long current_position = ftell(fd_r);
    printf("the current position of fd_r is : %d\n", current_position);


    fseek(fd_r, 12 * 20 , SEEK_SET);  //以开头位置为基准
    current_position = ftell(fd_r);
    printf("the current position of fd_r is : %d\n", current_position);

    fseek(fd_r, -180, SEEK_CUR); //以当前位置为基准
    current_position = ftell(fd_r);
    printf("the current position of fd_r is : %d\n", current_position);

    fseek(fd_r, -400, SEEK_END); //以结束位置为基准
    current_position = ftell(fd_r);
    printf("the current position of fd_r is : %d\n", current_position);
    return 0;
    
}