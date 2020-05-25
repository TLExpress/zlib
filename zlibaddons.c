#define ZLIB_INTERNAL
#include "zlibaddons.h"

int ZEXPORT compress4(dest, destLen, source, sourceLen, level, windowBits)
Bytef* dest;
uLongf* destLen;
const Bytef* source;
uLong sourceLen;
int level;
int windowBits;
{
    z_stream stream;
    int err;
    const uInt max = (uInt)-1;
    uLong left;

    left = *destLen;
    *destLen = 0;

    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = deflateInit2(&stream, level, Z_DEFLATED, windowBits, MAX_MEM_LEVEL, Z_DEFAULT_STRATEGY);
    if (err != Z_OK) return err;

    stream.next_out = dest;
    stream.avail_out = 0;
    stream.next_in = (z_const Bytef*)source;
    stream.avail_in = 0;

    do {
        if (stream.avail_out == 0) {
            stream.avail_out = left > (uLong)max ? max : (uInt)left;
            left -= stream.avail_out;
        }
        if (stream.avail_in == 0) {
            stream.avail_in = sourceLen > (uLong)max ? max : (uInt)sourceLen;
            sourceLen -= stream.avail_in;
        }
        err = deflate(&stream, sourceLen ? Z_NO_FLUSH : Z_FINISH);
    } while (err == Z_OK);

    *destLen = stream.total_out;
    deflateEnd(&stream);
    return err == Z_STREAM_END ? Z_OK : err;
}

int ZEXPORT compress3(dest, destLen, source, sourceLen, windowBits)
Bytef* dest;
uLongf* destLen;
const Bytef* source;
uLong sourceLen;
int windowBits;
{
    return compress4(dest, destLen, source, sourceLen, Z_DEFAULT_COMPRESSION, windowBits);
}

int ZEXPORT uncompress4(dest, destLen, source, sourceLen, windowBits)
Bytef* dest;
uLongf* destLen;
const Bytef* source;
uLong* sourceLen;
int windowBits;
{
    z_stream stream;
    int err;
    const uInt max = (uInt)-1;
    uLong len, left;
    Byte buf[1];    /* for detection of incomplete stream when *destLen == 0 */

    len = *sourceLen;
    if (*destLen) {
        left = *destLen;
        *destLen = 0;
    }
    else {
        left = 1;
        dest = buf;
    }

    stream.next_in = (z_const Bytef*)source;
    stream.avail_in = 0;
    stream.zalloc = (alloc_func)0;
    stream.zfree = (free_func)0;
    stream.opaque = (voidpf)0;

    err = inflateInit2(&stream, windowBits);
    if (err != Z_OK) return err;

    stream.next_out = dest;
    stream.avail_out = 0;

    do {
        if (stream.avail_out == 0) {
            stream.avail_out = left > (uLong)max ? max : (uInt)left;
            left -= stream.avail_out;
        }
        if (stream.avail_in == 0) {
            stream.avail_in = len > (uLong)max ? max : (uInt)len;
            len -= stream.avail_in;
        }
        err = inflate(&stream, Z_NO_FLUSH);
    } while (err == Z_OK);

    *sourceLen -= len + stream.avail_in;
    if (dest != buf)
        *destLen = stream.total_out;
    else if (stream.total_out && err == Z_BUF_ERROR)
        left = 1;

    inflateEnd(&stream);
    return err == Z_STREAM_END ? Z_OK :
        err == Z_NEED_DICT ? Z_DATA_ERROR :
        err == Z_BUF_ERROR && left + stream.avail_out ? Z_DATA_ERROR :
        err;
}

int ZEXPORT uncompress3(dest, destLen, source, sourceLen, windowBits)
Bytef* dest;
uLongf* destLen;
const Bytef* source;
uLong sourceLen;
int windowBits;
{
    return uncompress4(dest, destLen, source, &sourceLen, windowBits);
}