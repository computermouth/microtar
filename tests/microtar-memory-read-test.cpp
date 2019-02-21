#define _CRT_SECURE_NO_WARNINGS
#include "microtar.h"
#include <cstring>
using namespace std;

int main(int argc, char **argv)
{
    mtar_t tar;
    mtar_header_t h;

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

    if (int error = mtar_open_memory(&tar, buf, size))
    {
        printf("error: %d\n", error);
        return 2;
    }

    for (;;)
    {
        if (int error = mtar_read_header(&tar, &h))
        {
            if (error == MTAR_ENULLRECORD)
            {
                break;
            }
            printf("error: %d\n", error);
            return 5;
        }
        printf("%s (%d bytes)\n", h.name, (int)h.size);

        char data[256];
        if (h.size + 1 > sizeof(data))
        {
            printf("error: too large\n");
            return 6;
        }

        if (int error = mtar_read_data(&tar, data, h.size))
        {
            printf("error: %d\n", error);
            return 4;
        }
        data[h.size] = 0;

        puts(data);
        mtar_next(&tar);
    }

    if (int error = mtar_close(&tar))
    {
        printf("error: %d\n", error);
        return 3;
    }

    puts("success");
    return 0;
}
