#pragma once
#ifndef ZLIBADDONS_H
#define ZLIBADDONS_H
#include"zlib.h"
#ifdef __cplusplus
extern "C"
{
#endif
    ZEXTERN int ZEXPORT compress3 OF((Bytef* dest, uLongf* destLen,
        const Bytef* source, uLong sourceLen, int windowBits));

    ZEXTERN int ZEXPORT compress4 OF((Bytef* dest, uLongf* destLen,
        const Bytef* source, uLong sourceLen,
        int level, int windowBits));

    ZEXTERN int ZEXPORT uncompress3 OF((Bytef* dest, uLongf* destLen,
        const Bytef* source, uLong sourceLen, int windowBits));

    ZEXTERN int ZEXPORT uncompress4 OF((Bytef* dest, uLongf* destLen,
        const Bytef* source, uLong* sourceLen, int windowBits));

#ifdef __cplusplus
}
#endif
#endif
