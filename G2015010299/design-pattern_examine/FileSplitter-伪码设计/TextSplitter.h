#ifndef TextSplitter_h
#define TextSplitter_h

#include "BaseSplitter.h"

////////////////////////
// 非格式化文本文件分割器
// 继承自文件分割器基类
////////////////////////
class TextSplitter : public BaseSplitter
{
protected:
    /*
     * 以文本形式打开文件。
     */
    virtual FILE* open_file(const string file_path, string flag_rw);
    
    // 对于非格式化文件定义为空函数
    virtual long cache_fhead(const FILE* istream, char* fhead_cache)
    {
        return 0;
    }
    
    /*
     * 获取整个文件的字节数
     */
    virtual long get_fbody_size(FILE* istream);
    
    // 对于非格式化文件定义为空函数
    virtual short output_fhead(const char* fhead_cache, FILE* ostream)
    {
        return 0;
    }
    
    /*
     * 输出文件体（内容）信息。
     */
    virtual short output_fbody(FILE* istream,
                               long offset,
                               long block_size,
                               FILE* ostream,
                               long* pgress_size);
    
    // 对于非格式化文件定义为空函数
    virtual short output_ftail(FILE* ostream)
    {
        return 0;
    }
    
public:
    TextSplitter(const string file_path, int file_number) :
        BaseSplitter(file_path, file_number)
    {}
    
    virtual ~TextSplitter() {}
    
};

/*
 * 以文本形式打开文件。
 */
FILE* TextSplitter::open_file(const string file_path, string flag_rw)
{
    return fopen(file_path.c_str(), flag_rw.c_str());
}

/*
 * 获取整个文件的字节数
 * 参数 istream：源文件流对象
 */
long TextSplitter::get_fbody_size(FILE* istream)
{
    fseek(istream, 0, SEEK_SET);
    fseek(istream, 0, SEEK_END);
    long size = ftell(istream);
    fseek(istream, 0, SEEK_SET);
    return size;
}

/*
 * 输出文件体（内容）信息。在子类中实现。
 * 参数 istream：源文件流对象
 *      offset：源文件中的字节偏移量
 *      block_size：子文件的大小字节数
 *      ostream：目标文件流对象
 *      pgress_size：进度上文件分割已经处理的总字节数（输出参数，实时更新）
 */
short TextSplitter::output_fbody(FILE* istream,
                                 long offset,
                                 long block_size,
                                 FILE* ostream,
                                 long* pgress_size)
{
    while (已写入目标文件的字总节数 < 子文件的总字节数 && 未读取到源文件末尾)
    {
        从输入流读取一定长度的字节数据;
        
        将从输入流读取到的数据写入输出流;

        更新已写入目标文件的总字节数;
        
        更新进度上文件分割已经处理的总字节数;
    }
    
    return 处理状态值;
}

#endif

