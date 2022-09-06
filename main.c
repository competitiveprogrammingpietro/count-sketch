#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MAX 100

struct record
{
    // I could parse the IP straight into an int
    unsigned char ip[4]; // 4 byes
    // int ip;
    unsigned long long int bytes; // 8 bytes
};                                // Gran total (packed ?) = 12 bytes. 1024 / 12 =~ 100

struct record *find(unsigned char *ip, struct record *records)
{
    for (int i = 0; i < MAX; i++)
    {
        struct record *r = &records[i];
        if (r->ip[0] == ip[0] &&
            r->ip[1] == ip[1] &&
            r->ip[2] == ip[2] &&
            r->ip[3] == ip[3])
        {
            return r;
        }
    }
    return NULL;
}

static int cmp(const void *p1, const void *p2)
{
    struct record *r1 = (struct record *)p1;
    struct record *r2 = (struct record *)p2;
    return r2->bytes - r1->bytes; // Descending order
}

int main(int argn, char **argv)
{

    char *line, *ipstr, *size, *stoken;
    size_t len = 0;
    ssize_t nread;
    long long bytes;
    struct record records[MAX] = {0};
    unsigned char ip[4];
    int count = 0;

    // Read stdin
    while ((nread = getline(&line, &len, stdin)) != -1)
    {
        count++;
        // First MAX times this is a waste
        qsort(records, MAX, sizeof(struct record), cmp);
        // printf("Retrieved line of length %zd:\n", nread);
        // fwrite(line, nread, 1, stdout);
        ipstr = strtok(line, " ");
        size = strtok(NULL, " ");
        bytes = atoi(size);

        // printf("IP %s\n", ipstr);
        // printf("Size %lld\n", atoi(size));
        stoken = strtok(ipstr, ".");
        int c = 0;
        while (stoken != NULL)
        {
            ip[c++] = atoi(stoken);
            stoken = strtok(NULL, ".");
            // CHECK IF C > 3
        }
        // printf("FIRST %hhu\n", ip[0]);
        // printf("FIRST %hhu\n", ip[1]);
        // printf("FIRST %hhu\n", ip[2]);
        // printf("FIRST %hhu\n", ip[3]);

        struct record *item = find(ip, records);

        // Found, add up
        if (item != NULL)
        {
            item->bytes += bytes;
            continue;
        }

        // We need to check if the new item has a higher byte count than the
        // one at the bottom of our sorted array: if yes we substitute it
        // otherwise we discard it.
        if (records[MAX - 1].bytes > bytes)
        {
            continue;
        }

        // Substitute
        records[MAX - 1].ip[0] = ip[0];
        records[MAX - 1].ip[1] = ip[1];
        records[MAX - 1].ip[2] = ip[2];
        records[MAX - 1].ip[3] = ip[3];
        records[MAX - 1].bytes = bytes;
    }
    printf("[\n");
    for (int i = 0; i < 10; i++)
    {
        if (records[i].bytes == 0)
        {
            break;
        }
        printf("{ \"ip\":\"%hhu.%hhu.%hhu.%hhu\", \"size\":%lld },\n",
               records[i].ip[0],
               records[i].ip[1],
               records[i].ip[2],
               records[i].ip[3],
               records[i].bytes);
    }
    printf("]\n");
    fprintf(stderr, "%d line processed\n", count);
    return 0;
}
