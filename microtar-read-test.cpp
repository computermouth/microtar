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

    while ((mtar_read_header(&tar, &h)) != MTAR_ENULLRECORD)
    {
        printf("%s (%d bytes)\n", h.name, (int)h.size);
        mtar_next(&tar);
    }

    if (int error = mtar_close(&tar))
    {
        printf("error: %d\n", error);
        return 3;
    }

    return 0;
}
