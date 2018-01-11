#include<ctime>
#include<cstdio>
#include<cstdlib>
#include<cstring>

int main (int argc, char** argv){
    if (argc < 3){
        printf("too few argc\n");
        return 1;
    }

    srand(time(NULL));
    int var_num = atoi(argv[1]);
    int tuple_num = atoi(argv[2]);
    printf("%d %d\n", tuple_num, var_num);
    for (int i=0;i<tuple_num;i++){
        for (int j=0;j<3;j++){
            if (j) printf(" ");
            if (rand()%2) printf("-");
            unsigned int tmp = (unsigned int)rand() % var_num + 1;
            printf("%u", tmp);
        }
        printf("\n");
    }
}
