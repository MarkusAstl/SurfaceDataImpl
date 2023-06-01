// Author: Ing. Ingo Gulyas, MMSc
// Date: 26.03.2021
// Medical University of Vienna / AKH Wien
// Department of Radiotherapy

// INFO: implementation of cpu based convolution for images


#include "convolution.h"
#include "fire_global.h"

#ifdef _WIN32
    #include <corecrt_math_defines.h>
#endif


static float kernel_weights_gauss9[] = {1./16, 2./16, 1./16, 2./16, 4./16, 2./16, 1./16, 2./16, 1./16};


void Kernel_Free(Kernel_T* kernel)
{
    kernel->dim = 0;
    if(kernel->weights)
    {
        delete kernel->weights;
        kernel->weights = NULL;
    }
}

#pragma message("this kernel is not corrected!")
void Kernel_Init_Gauss(Kernel_T* kernel, unsigned short dim, float sigma)
{
    int x, y, i;
    float val, sum=0.;

    Kernel_Free(kernel);

    if(dim%2 == 0)
    {
        DBG_ERROR("kernel allocation failed because dimension has to be an odd number!");
        return;
    }

    kernel->weights = new (std::nothrow) float[SQUARE(dim)];
    if(kernel->weights == NULL)
    {
        DBG_ERROR("memory allocation for kernel weights failed!");
        return;
    }

    kernel->dim = dim;

    for(x=-(dim-1)/2, i=0; x<=(dim-1)/2; x++)
    {
        for(y=-(dim-1)/2; y<=(dim-1)/2; y++, i++)
        {
            val = 1./(2.*M_PI*SQUARE(sigma))*exp(-(SQUARE(x)+SQUARE(y))/(2*SQUARE(sigma)));
            kernel->weights[i] = val;
            sum += val;
        }
    }

    DBG_PRINT("weight sum before correction: %.6f\n", sum);

    for(i=0; i<SQUARE(kernel->dim); i++)
    {
        kernel->weights[i] /= sum;
    }
}

void Kernel_Print(Kernel_T* kernel)
{
    int x, y, i;

    if(kernel->weights == NULL)
    {
        DBG_ERROR("kernel memory not allocated!");
        return;
    }

    DBG_PRINT("printing kernel data:\n\n");
    for(x=0, i=0; x<kernel->dim; x++)
    {
        for(y=0; y<kernel->dim; y++, i++)
        {
            DBG_PRINT("%.6f\t", kernel->weights[i]);
        }
        DBG_PRINT("\n");
    }
    DBG_PRINT("end\n");
}

void Kernel_Conv(unsigned short* img_in, unsigned short* img_out, unsigned short width, unsigned short height, Kernel_T* kernel)
{
    int i, x, y, kx, ky, kkx, kky;
    int w = width, h = height, offset;
    float val;

    if(!kernel->weights || kernel->dim == 0)
    {
        DBG_PRINT("kernel not initialized!");
        return;
    }


    offset = (kernel->dim - 1) / 2;

    for (y = 0; y < h; y++)
    {
        for (x = 0; x < w; x++)
        {
            val = 0.;

            for (ky = y - offset, i=0; ky <= y + offset; ky++)
            {
                for (kx = x - offset; kx <= x + offset; kx++, i++)
                {
                    kkx = kx;
                    if (kx < 0)	kkx = 0;
                    if (kx > w-1) kkx = w - 1;

                    kky = ky;
                    if (ky < 0) kky = 0;
                    if (ky > h - 1) kky = h - 1;

                    val += img_in[kky*w + kkx] * kernel->weights[i];

                    //fprintf(stdout, "X/Y/i: %d/%d/%d\n", kkx, kky, i);
                }
            }

            img_out[y*width + x] = round(val);
        }
    }
}
