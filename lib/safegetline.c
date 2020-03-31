#include "safegetline.h"

int _safegetline(FILE *fp, char* buffer, size_t* buflen, size_t* off)
{
        char *end = buffer + *buflen - 1;
        char *dst = buffer + *off;
        int endfound = FALSE;
        int c = 0;

        while (!endfound && dst < end && (c = getc(fp)) != EOF)
        {
                *dst++ = c;
                if (c == '\r') {
                        c = getc(fp);
                        if (c != '\n' && c != EOF)
                                ungetc(c, fp);
                        endfound = TRUE;
                }
                if (c == '\n') {
                        /* Handles trailing EOL at EOF consistently */
                        c = getc(fp);
                        if (c != EOF)
                                ungetc(c, fp);
                        endfound = TRUE;
                }
        }

        if (endfound)
                *dst--; /* YES IT FUCKING IS */

        *dst = '\0';
        *off = dst - buffer;

        /* End of Buffer before End of Line */
        if (dst == end && c != '\n')
                return EOF - 1;

        if (c == EOF && dst == buffer) {
                if (ferror(fp)) {
                        perror("getc");
                        exit(EXIT_FAILURE);
                }
                return EOF;
        }

        return 0;
}


int sappline(FILE *f, char **buf, size_t* buflen, size_t* len, size_t off)
{
        size_t offset = off;
        int ret = 0;
        do {
                if(offset + 1 >= *buflen)
                        increase_buffer(buf, buflen);
                ret = _safegetline(f, *buf, buflen, &offset);
        } while (ret == EOF - 1);

        if (len)
                *len = offset;

        return ret;
}

int sgetline(FILE *fp, char **buf, size_t* buflen, size_t* linelen)
{
        return sappline(fp, buf, buflen, linelen, 0);
}
