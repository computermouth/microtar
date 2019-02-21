#include "microtar.h"
#include <cstring>
using namespace std;

int main(int argc, char **argv)
{
    mtar_t tar;
    const char *str1 = "Hello world";
    const char *str2 = "Goodbye world";

    if (argc < 2)
    {
        printf("error: no argument\n");
        return 1;
    }

    if (int error = mtar_open(&tar, argv[1], "w"))
    {
        printf("error: %d\n", error);
        return 2;
    }

    mtar_write_file_header(&tar, "test1.txt", strlen(str1));
    mtar_write_data(&tar, str1, strlen(str1));
    mtar_write_file_header(&tar, "test2.txt", strlen(str2));
    mtar_write_data(&tar, str2, strlen(str2));

    if (int error = mtar_finalize(&tar))
    {
        printf("error: %d\n", error);
        return 3;
    }

    if (int error = mtar_close(&tar))
    {
        printf("error: %d\n", error);
        return 4;
    }

    return 0;
}
