#pragma once

#define SGL_CU_CHECK(call)                                                                                             \
    {                                                                                                                  \
        CUresult _result_ = call;                                                                                      \
        if (_result_ != CUDA_SUCCESS)                                                                                  \
            SGL_THROW("CUDA error: {} failed with {}", #call, int(_result_));                                          \
    }
