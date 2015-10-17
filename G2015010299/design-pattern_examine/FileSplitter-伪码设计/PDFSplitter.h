#ifndef PDFSplitter_h
#define PDFSplitter_h

#include "BaseSplitter.h"

////////////////////////
// PDF文件分割器
// 继承自文件分割器基类
////////////////////////
class PDFSplitter : public BaseSplitter
{
protected:
    /*
     * 以二进制形式打开文件。
     */
    virtual FILE* open_file(const string file_path, string flag_rw);
    
    /*
     * 读取PDF文件的头（格式）信息并缓存。以便输出多个子文件时使用。
     */
    virtual long cache_fhead(const FILE* istream, char* fhead_cache);
    
    /*
     * 获取文件体（内容）的字节数
     */
    virtual long get_fbody_size(FILE* istream);
    
    /*
     * 输出PDF文件的头（格式）信息。
     */
    virtual short output_fhead(const char* fhead_cache, FILE* ostream);
    
    /*
     * 输出文件体（内容）信息。
     */
    virtual short output_fbody(FILE* istream,
                               long offset,
                               long block_size,
                               FILE* ostream,
                               long* pgress_size);
    
    /*
     * 输出PDF文件的尾部信息。
     */
    virtual short output_ftail(FILE* ostream);
    
public:
    PDFSplitter(const string file_path, int file_number) :
        BaseSplitter(file_path, file_number)
    {}
    
    virtual ~PDFSplitter() {}
    
};

/*
 * 以二进制形式打开文件。
 */
FILE* PDFSplitter::open_file(const string file_path, string flag_rw)
{
    return fopen(file_path.c_str(), flag_rw.append("b").c_str());
}

long PDFSplitter::cache_fhead(const FILE* istream, char* fhead_cache)
{
    根据PDF格式信息定位到文件头的开始位置，并保存此位置值;
    定位到文件头的结束位置，并保存此位置值;
    从输入流读取文件头的开始位置到结束位置之间的字节数据，保存在文件头缓存中;
    return 写入到文件头缓存中的总字节数;
}

long PDFSplitter::get_fbody_size(FILE* istream)
{
    根据PDF格式信息定位到文件体的开始位置，并保存此位置值;
    定位到文件体的结束位置，并保存此位置值;
    return 文件体的结束位置值 - 文件体的开始位置值;
}

short PDFSplitter::output_fhead(const char* fhead_cache, FILE* ostream)
{
    将保存在文件头缓存中的数据写入输出文件流;
    return 输出是否成功的状态值;
}

/*
 * 输出文件体（内容）信息。在子类中实现。
 * 参数 istream：源文件流对象
 *      offset：源文件中的字节偏移量
 *      block_size：子文件的文件体字节数
 *      ostream：目标文件流对象
 *      pgress_size：进度上文件分割已经处理的总字节数（输出参数，实时更新）
 */
short PDFSplitter::output_fbody(FILE* istream,
                                 long offset,
                                 long block_size,
                                 FILE* ostream,
                                 long* pgress_size)
{
    while (已写入目标文件body的字总节数 < 子文件body的总字节数 && 未读取到源文件body末尾)
    {
        从输入流的字节偏移位置起读取一定长度的字节数据;
        
        将从输入流读取到的数据写入输出流;

        更新已写入目标文件body的总字节数;
        
        更新进度上文件分割已经处理的总字节数;
    }
    
    return 处理状态值;
}

short PDFSplitter::output_ftail(FILE* ostream)
{
    向输出文件流写入PDF文件尾部信息;
    return 输出是否成功的状态值;
}

#endif
