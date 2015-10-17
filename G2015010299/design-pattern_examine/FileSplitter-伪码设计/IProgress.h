#ifndef IProgress_h
#define IProgress_h

/////////////////
// 进度表示接口基类
/////////////////
class IProgress
{
public:
    // 按照目前的进度值，处理进度表示逻辑（由子类实现）
    virtual short do_progress(float state_val) = 0;
    virtual ~IProgress() {};
};

#endif
