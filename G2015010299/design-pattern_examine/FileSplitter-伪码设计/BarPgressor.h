#ifndef BarPgressor_h
#define BarPgressor_h

#include <iostream>
#include "IProgress.h"

//伪码：using OpenGL;

//////////////////////////////
// 进度条图形进度表示器
// 继承自进度表示接口基类
//////////////////////////////
class BarPgressor : public IProgress
{
    virtual short do_progress(float state_val)
    {
        // 显示进度条图形
        //伪码：OpenGL::draw_progressbar(state_val);
        return 0;
    }
};

#endif

