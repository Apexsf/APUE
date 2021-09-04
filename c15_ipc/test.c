#include <stdio.h>

int main () {
    char str1[20], str2[30];
    int a,b;
    scanf("%d",&a);
    printf("a : %d\n",a);
    scanf("%d",&b);
    printf("b : %d\n",b);

    scanf("%d %d",&a,&b);
    printf("a : %d\n",a);
    printf("b : %d\n",b);


    printf("Enter name: ");
    scanf("%s", str1);

    printf("Enter your website name: ");
    scanf("%s", str2);

    printf("Entered Name: %s\n", str1);
    printf("Entered Website:%s", str2);
    
    return(0);
}