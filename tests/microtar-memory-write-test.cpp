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

    FILE *fp = fopen(argv[1], "rb");
    if (!fp)
    {
        printf("error: unable to open\n");
        return 6;
    }

    static char buf[4000];
    size_t size = fread(buf, 1, 4000, fp);
    if (!size)
    {
        printf("error: unable to read\n");
        fclose(fp);
        return 7;
    }
    fclose(fp);

    if (int error = mtar_open_memory(&tar, NULL, 0))
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

    if (size != tar.memory_size)
    {
        printf("size differs\n");
        return 5;
    }

    if (memcmp(buf, tar.memory, size) != 0)
    {
        printf("data differs\n");
        return 6;
    }

    if (int error = mtar_close(&tar))
    {
        printf("error: %d\n", error);
        return 4;
    }

    puts("success");
    return 0;
}
