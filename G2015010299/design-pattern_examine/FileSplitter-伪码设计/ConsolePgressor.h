#ifndef ConsolePgressor_h
#define ConsolePgressor_h

#include <iostream>
#include <cmath>
#include "IProgress.h"

using std::string;
using std::cout;

//////////////////////////////
// 命令行控制台标准输出进度表示器
// 继承自进度表示接口基类
//////////////////////////////
class ConsolePgressor : public IProgress
{
protected:
    static const short PGRESS_SCALE;
    static const string PGRESS_SYMBOL;
    
public:
    virtual short do_progress(float state_val)
    {
        // 以百分比和标准输出方式表示进度
        float state = floorf(state_val * PGRESS_SCALE);
        state = state > PGRESS_SCALE ? PGRESS_SCALE : state;
        cout << "..." << state << PGRESS_SYMBOL;
        return 0;
    }
};

const short ConsolePgressor::PGRESS_SCALE = 100;
const string  ConsolePgressor::PGRESS_SYMBOL = "%";

#endif

