#include "microtar.h"
#include <cstring>
using namespace std;

int main(int argc, char **argv)
{
    mtar_t tar;
    mtar_header_t h;
    char *p;

    if (argc < 2)
    {
        printf("error: no argument\n");
        return 1;
    }

    if (int error = mtar_open(&tar, argv[1], "r"))
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

    fflush(stdout);
    return 0;
}
