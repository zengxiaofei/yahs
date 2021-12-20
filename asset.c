/*********************************************************************************
 * MIT License                                                                   *
 *                                                                               *
 * Copyright (c) 2021 Chenxi Zhou <chnx.zhou@gmail.com>                          *
 *                                                                               *
 * Permission is hereby granted, free of charge, to any person obtaining a copy  *
 * of this software and associated documentation files (the "Software"), to deal *
 * in the Software without restriction, including without limitation the rights  *
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell     *
 * copies of the Software, and to permit persons to whom the Software is         *
 * furnished to do so, subject to the following conditions:                      *
 *                                                                               *
 * The above copyright notice and this permission notice shall be included in    *
 * all copies or substantial portions of the Software.                           *
 *                                                                               *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR    *
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,      *
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE   *
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER        *
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, *
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE *
 * SOFTWARE.                                                                     *
 *********************************************************************************/

/********************************** Revision History *****************************
 *                                                                               *
 * 02/09/21 - Chenxi Zhou: Created                                               *
 *                                                                               *
 *********************************************************************************/
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "asset.h"

double cputime(void)
{
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
    return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

long peakrss(void)
{
    struct rusage r;
    getrusage(RUSAGE_SELF, &r);
#ifdef __linux__
    return r.ru_maxrss * 1024;
#else
    return r.ru_maxrss;
#endif
}

double realtime(void)
{
    struct timeval tp;
    gettimeofday(&tp, NULL);
    return tp.tv_sec + tp.tv_usec * 1e-6;
}

long aslimit(void)
{
    long as;
    struct rlimit lim;
    getrlimit(RLIMIT_RSS, &lim);
    as = lim.rlim_cur;
    getrlimit(RLIMIT_AS, &lim);
    return MAX(as, (long) lim.rlim_cur);
}

int file_copy(char *fin, char *fout)
{
    FILE *fptr1, *fptr2;
    int c;
    fptr1 = fopen(fin, "r");
    fptr2 = fopen(fout, "w");
    c = fgetc(fptr1);
    while (c != EOF) {
        fputc(c, fptr2);
        c = fgetc(fptr1);
    }
    fclose(fptr1);
    fclose(fptr2);
    return 0;
}

int8_t is_read_pair(const char *rname0, const char *rname1)
{
    if (!strcmp(rname0, rname1))
        return 1;
    int n = strlen(rname0);
    if (n != strlen(rname1))
        return 0;
    if (!strncmp(rname0, rname1, n - 2))
        return 1;
    return 0;
}

inline uint32_t div_ceil(uint64_t x, uint32_t y)
{
    uint64_t b = (MAX(x, 1) - 1) / y;
    assert(b < UINT32_MAX);
    return 1 + b;
}

uint64_t linear_scale(uint64_t g, int *scale, uint64_t max_g)
{
    int s;
    s = 0;
    while (g > max_g) {
        ++s;
        g >>= 1;
    }
    
    *scale = s;
    return g;
}

