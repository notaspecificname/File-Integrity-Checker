#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

uint32_t calculatehash(const uint8_t *data, size_t length){
    uint32_t hash = 2166743252U;

    const uint32_t randomprime = 16777619U;

    for(size_t i=0;i<length;i++){

        hash ^=data[i]

        hash*=randomprime;
    }

    return hash;
}