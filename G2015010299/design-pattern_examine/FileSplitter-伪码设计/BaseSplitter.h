#ifndef BaseSplitter_h
#define BaseSplitter_h

#include <string>
#include <map>
#include <pthread.h>
#include "IProgress.h"

using std::string;
using std::map;

// 目标子文件名信息的结构体
typedef struct
{
    目标子文件路径;
    目标子文件名的前缀;
    目标子文件名的后缀;
} stru_subfile_path;

// 进度显示处理控制信息结构体
typedef struct
{
    进度上文件分割已经处理的总字节数;
    分割源文件体总字节数;
    多种类进度显示器的map;
} stru_arg_runpgress;

/*
 * 进度显示处理
 * 作为进度显示子线程的主调函数
 */
void* on_progress(void* runpgress);

/////////////////////////////////////////////////////////
// 文件分割器基类。
// 设计思想：
// 1.模板方法模式（构建文件分割处理主流程框架）
// 2.观察者模式（扩展多种类分割进度显示器）
// 3.以独立子线程作分割进度显示处理（解耦文件分割和进度显示逻辑）
/////////////////////////////////////////////////////////
class BaseSplitter
{
protected:
    string file_path;                       // 源文件全路径
    int file_number;                        // 目标子文件数量
    FILE* istream;                          // 源文件流对象
    char* fhead_cache;                      // 源文件头信息的缓存
    long fhead_size;                        // 源文件头（格式）信息字节数
    long fbody_size;                        // 源文件体（内容）信息字节数
    long pgress_size;                       // 进度上文件分割已经处理的总字节数
    stru_subfile_path t_subfile_path;		// 目标子文件名信息的结构体
    map<string, IProgress*> map_pgressor;	// 多种类进度显示器的map
    pthread_t thread_pgress;                // 进度显示子线程
    
    /*
     * 打开源文件或目标子文件。以文本，二进制等不同形式在子类中实现。
     */
    virtual FILE* open_file(const string file_path, string flag_rw) = 0;
    
    /*
     * 读取格式化文件的头（格式）信息并缓存。以便输出多个子文件时使用。
     * 在子类中实现。（对于非格式化文件，可为空函数）
     */
    virtual long cache_fhead(const FILE* istream, char* fhead_cache) = 0;
    
    /*
     * 获取格式化文件的文件体内容的字节数，在子类中实现。
     * （对于非格式化文件，等价于获取整个文件的字节数）
     */
    virtual long get_fbody_size(FILE* istream) = 0;
    
    /*
     * 根据源文件名获取目标子文件名的前缀，后缀和路径。可在子类中重载。
     */
    virtual short get_subfname(const string file_path, stru_subfile_path& t_subfile_path);
    
    /*
     * 将源文件分割成子文件。
     * 定义输出子文件处理的流程框架。 可在子类中重载。
     * 参数 offset：源文件中的字节偏移量
     *      block_size：子文件的大小字节数
     *      idx_sub：第几个子文件的数字标示
     *      pgress_size：进度上文件分割已经处理的总字节数（输出参数，实时更新）
     */
    virtual short split_to_subfile(long offset,
                                   long block_size,
                                   short idx_sub,
                                   long* pgress_size);
    
    /*
     * 输出格式化文件的头（格式）信息。
     * 在子类中实现。（对于非格式化文件，可为空函数）
     */
    virtual short output_fhead(const char* fhead_cache, FILE* ostream) = 0;
    
    /*
     * 输出文件体（内容）信息。在子类中实现。
     * 参数 istream：源文件流对象
     *      offset：源文件中的字节偏移量
     *      block_size：子文件的大小字节数
     *      ostream：目标文件流对象
     *      pgress_size：进度上文件分割已经处理的总字节数（输出参数，实时更新）
     */
    virtual short output_fbody(FILE* istream,
                               long offset,
                               long block_size,
                               FILE* ostream,
                               long* pgress_size) = 0;
    
    /*
     * 输出格式化文件的尾部信息。
     * 在子类中实现。（对于非格式化文件，可为空函数）
     */
    virtual short output_ftail(FILE* ostream) = 0;
    
public:
    BaseSplitter(const string file_path, int file_number) :
        // 初始化分割的源文件路径和目标子文件数量
        file_path(file_path), file_number(file_number)
    {}
    
    /*
     * 附加一个进度显示器
     */
    void attach_progressor(const string name, IProgress* progressor)
    {
        map_pgressor[name] = progressor;
    }
    
    /*
     * 取消一个进度显示器
     */
    void detach_progressor(const string name)
    {
        map_pgressor.erase(name);
    }
    
    /*
     * 分割文件
     * 定义文件分割处理的主流程框架
     */
    short Split();
    
    virtual ~BaseSplitter()
    {
        // 销毁文件流对象，文件头信息缓存数组等
    }
    
};

/*****************************
 * 分割文件
 * 定义文件分割处理的主流程框架
 ****************************/
short BaseSplitter::Split()
{
    // 打开源文件
    成员函数open_file();
    
    // 读取文件头信息并缓存
    成员函数cache_fhead();
    // 获取文件体内容的字节数
    成员函数get_fbody_size();
    // 计算分割子文件块的字节数
    ......;
    
    // 根据源文件名获取目标子文件名的前缀和后缀
    成员函数get_subfname();
    
    // 以on_progress为线程内的主运行函数/进度显示处理控制信息结构体为运行参数，
    // 以与当前线程的调度参数(优先级)相同的属性，创建进度显示处理子线程。
    多线程库函数pthread_create();
    
    for (子文件数目)
    {
        // 分割子文件
        成员函数split_to_subfile();
    }
    
    // 等待进度显示处理子线程终止，获取子线程退出状态
    多线程库函数pthread_join();
    
    // 关闭源文件
    fclose();
    
    return 处理状态值;
}

/*****************************
 * 将源文件分割成子文件。
 * 定义输出子文件处理的流程框架。
 ****************************/
short BaseSplitter::split_to_subfile(long offset, long block_size, short idx_sub)
{
    // 根据目标子文件名信息结构体，生成子文件全名
    ......;
    // 打开目标子文件
    成员函数open_file();
    // 输出文件头
    成员函数output_fhead();
    // 输出文件体
    成员函数output_fbody();
    // 输出文件尾部
    成员函数output_ftail();
    // 关闭目标子文件
    fclose();
    
    return 处理状态值;
}

/*****************************
 * 进度显示处理
 * 作为进度显示子线程的主调函数
 ****************************/
void* on_progress(void* runpgress)
{
    
    while (进度上文件分割已经处理的总字节数 < 源文件体总字节数)
    {
        if (进度上文件分割已经处理的总字节数发生改变)
        {
            for (遍历所有进度显示器)
                按照目前的处理进度值，执行各自的进度显示处理函数;
        }
    }
    // 子线程退出状态值
    pthread_exit((void*)RET_SUCCESS);
}

short BaseSplitter::get_subfname(const string file_path, stru_subfile_path& t_subfile_path)
{
    // 解析unix/linux/windows文件路径，得到目标子文件的路径／文件名前缀／后缀信息
    // ......
    return 处理状态值;
}

#endif

