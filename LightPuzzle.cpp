#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <string.h>

void initMatrix(int* mptr, int width, int height, int size, int mw){
    int nw = width - 1;
    int nh = height - 1;
    int swidth = mw * width;
    int* ptr = mptr;
    int mw1 = mw + 1;
    for(int y = 0; y < height; y++){
        for(int x = 0; x < width; x++){
            *ptr = 1;
            if(x > 0)
                *(ptr - mw) = 1;
            if(x < nw)
                *(ptr + mw) = 1;
            if(y > 0)
                *(ptr - swidth) = 1;
            if(y < nh)
                *(ptr + swidth) = 1;
            ptr += mw1;
        }
    }
    mptr += size;
    for(int s = 0; s < size; s++){
        *mptr = 1;
        mptr += mw;
    }
}

int add(int a, int b){
    return (a + b) % 2;
}

int mult(int a, int b){
    return (a * b) % 2;
}

int trignize(int * ptr, int width, int height, int * pZeros){
    int * temp = new int[width];
    memset(pZeros, 0, sizeof(int) * height);
    int* pm = ptr;
    int sc = 0;
    for(int i = 0; i < height; i++){
        int js = height - i - 1;
        int * jptr = ptr + width;
        int width1 = width + 1;
        if(*ptr == 0){
            int * jjptr = jptr;
            int slen = sizeof(int) * (width - i);
            for(int j = 0; j < js; j++){
                if(*jjptr == 1){
                    memcpy(temp, jjptr, slen);
                    memcpy(jjptr, ptr, slen);
                    memcpy(ptr, temp, slen);
                    goto succ;
                }
                jjptr += width;
            }
            ptr+=width1;
            pZeros[sc++] = i;
            continue;
        }
        succ:
        for(int j = 0; j < js; j++){
            if(*jptr){
                int ks = js + 2;
                for(int k = 0; k < ks; k++){
                    int * kptr = jptr + k;
                    *kptr = add(*(ptr + k), *kptr);
                }
            }
            jptr += width;
        }
        ptr += width1;
    }
    delete [] temp;
    return sc;
}

int checkPara(int* ptr, int width, int height, int* pZeros, int sc){
    for(int i = 0; i < sc; i++){
        if(ptr[pZeros[i] * width + height])
            return -1;
    }
    return 0;
}

void solve(int* ptr, int width, int height){
    for(int i = height - 1; i >= 0; i--){
        for(int j = height - 1; j > i; j--){
            if(ptr[j + i * width] != 0){
                ptr[i * width + width - 1] = add(ptr[i * width + width - 1], ptr[j * width + width - 1]);
            }
        }
    }
}

int mSolve(int* ptr, int width, int height){
    for(int i = height - 1; i >= 0; i--){
        for(int j = height - 1; j > i; j--){
            if(ptr[j + i * width] != 0){
                for(int k = i; k < width; k++){
                    ptr[i * width + k] = add(ptr[i * width + k], ptr[j * width + k]);
                }
            }
        }
    }
    return 0;
}

void mRead(int* ptr, int* pZeros, int sc, int idx, int mw, int size){
    for(int i = 0; i < sc; i++){
        int bit = (idx >> i) & 1;
        ptr[pZeros[i] * mw + size] = bit;
    }
    for(int y = size - 1; y >= 0; y--){
        int * p = ptr + y * mw;
        for(int x = y + 1; x < size; x++){
                *(p + size) = add(*(p + size),mult(p[x], ptr[x * mw + size]));
        }
    }
}

int main()
{
    int width;
    int height;
    int * pMatrix;
    int * pZeros;
    scanf("%d", &width);
    scanf("%d", &height);
    int size = width * height;
    int mw = size + 1;
    int matrixSize = mw * size;
    pMatrix = new int[matrixSize];
    pZeros = new int[size];
    memset(pMatrix, 0, matrixSize * sizeof(int));
    initMatrix(pMatrix, width, height, size, mw);
    
    int rs = trignize(pMatrix, mw, size, pZeros);

    if(rs == 0){
        solve(pMatrix, mw, size);
        for(int y = 0; y < height; y++){
            for(int x = 0; x < width; x++){
                printf("%2d", pMatrix[(x + y * width) * mw + mw - 1]);
            }
            printf("\n");
        }
    } else {
        mSolve(pMatrix, mw, size);
        if(checkPara(pMatrix, mw, size, pZeros, rs)){
            printf("No solution.\n");
            return 0;
        }
        printf("%d solutions.\n",1 << rs);
        int idx;
        scanf("%d", &idx);
        mRead(pMatrix, pZeros, rs, idx, mw, size);
        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                printf("%2d", pMatrix[(x + y * width) * mw + mw - 1]);
            }
            printf("\n");
        }
    }
    return 0;
}
