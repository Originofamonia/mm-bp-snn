#ifndef __CU_BASE_CU_H__
#define __CU_BASE_CU_H__

#include <helper_functions.h>
#include <helper_cuda.h>
#include "util.h"

__device__ float d_nonLinearity(float val, int NONLIN);
__device__ float d_dnonLinearity(float val,int NONLIN);
__device__ void   swap(float& val1, float& val2);


__global__ void   g_dnonLinearity(float* delta, float*acti, int len, int NONLIN);
__global__ void   g_nonLinearity(float* inputs, int len, int NONLIN);

__global__ void g_vecAdd(float**_v_w, float** _wgrad,float** _w,
	float** _v_b, float** _bgrad, float** _b, 
	int lenw, int lenb,
	float momentum, float lratew, float lrateb);

__global__ void g_vecAdd(float*v_w, float*wgrad,float* w,
	float* v_b, float* bgrad, float* b, 
	int lenw, int lenb,
	float momentum, float lratew, float lrateb);

__global__ void g_getBgrad(float* softMaxDelta, float* bgrad, float* dropb, int batch);
__global__ void g_getBgrad(float* softMaxDelta, float* bgrad, int batch);

__global__ void g_getCost_3(float* cost,
	float** weight,
	float lambda, int wlen);

__global__ void g_getCost_2(float* cost,
	float* weight,
	float lambda, int len);

__global__ void g_getCost_1(float* softMaxP,
	float* groundTruth, float* cost, int*y, int rows, int cols, int batch);

/*
* function: cuMatrix(batch, size, channel) to cuMatrix(batch, size * channel, 1)
* blocks  : dim3(batch)
* threads : dim3(min(512, cuPool[poolidx]->cols))
*/
__global__ void g_convert(float* cuPool, float*cuPoolToFlActi, int batch, int size, int channel);


/*
* 
* function: cast cuMatrix<bool>*(batch, inputDim * endTime) to cuMatrix<float>*(endTime, inputDim)
*           only use this function when batch = 1 !
* blocks  : dim3(endTime) endTime --> nrows
* threads : dim3(min(1024, inputDim)) inputDim --> ncols
*/
__global__ void g_cast_bool_2_float(bool* inputs, int endTime, int inputDim, float* input_f);


/*
function: g_preDeltaFormat
threads : <<<dim3(batch), dim3(512)>>> 
*/
__global__ void g_preDeltaFormat(float* cuPoolFlDelta, 
	float* cuPoolDelta, int batch, int size, int channels);

/*
function: transform the binary response matrix to the spike times
threads : <<<dim3(batch), dim3(min(outputDim, 1024))>>>
*/
__global__ void g_response_2_spiketime(bool* outputs, int* outputs_time, int ouputDim, int endTime);

/*
function: normalize the fire counts by the max count for SNN
threads : <<<dim3(batch), dim3(min(1024, inputDim))>>>
*/
__global__ void g_normalize_fireCount(int * inputs, float * inputs_float, int rows, int cols);

/*
function: compute the softmax prob of the softmax layer
threads : <<<dim3(batch), dim3(min(512, outputDim))>>>
*/
__global__ void g_getSoftMaxP(float* softMaxP, float* b, int cols);

__global__ void g_getSoftMaxDelta(float* softMaxDelta, float* softMaxP, float* groundTruth, int len);

__global__ void g_getSmrWgrad(float* wgrad, float* weight, float lambda, int len, int batch);
#endif


