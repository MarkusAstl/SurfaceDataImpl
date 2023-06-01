// Author: Ing. Ingo Gulyas, MMSc
// Date: 26.03.2021
// Medical University of Vienna / AKH Wien
// Department of Radiotherapy

#ifndef COVOLUTION_H
#define COVOLUTION_H

#define KERNEL_INIT {0, NULL}

typedef struct
{
    unsigned short dim;
    float* weights;
} Kernel_T;

void Kernel_Free(Kernel_T* kernel);
void Kernel_Init_Gauss(Kernel_T* kernel, unsigned short dim, float sigma);
void Kernel_Print(Kernel_T* kernel);
void Kernel_Conv(unsigned short* img_in, unsigned short* img_out, unsigned short width, unsigned short height, Kernel_T* kernel);

#endif // COVOLUTION_H
