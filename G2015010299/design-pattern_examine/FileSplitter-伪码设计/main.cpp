#include <iostream>

#include "PDFSplitter.h"
#include "BarPgressor.h"
#include "ConsolePgressor.h"

using std::cin;
using std::cout;
using std::endl;

int main()
{
    string file_path;   // 分割源文件的路径
    int file_number;    // 目标子文件数目
    
    cout << "Please input the file path which split from: ";
    cin >> file_path;
    cout << "and input the sub files number how much split to: ";
    cin >> file_number;
    
    // PDF文件分割器
    PDFSplitter pdf_splitter(file_path, file_number);
    // 进度条图形进度表示器
    BarPgressor bar_pgressor;
    // 命令行控制台标准输出进度表示器
    ConsolePgressor cnsl_pgressor;
    
    // 向PDF文件分割器附加以上两个进度表示器
    pdf_splitter.attach_progressor("BAR_PGRESSOR", &bar_pgressor);
    pdf_splitter.attach_progressor("CNSL_PGRESSOR", &cnsl_pgressor);
    
    // 执行PDF文件分割处理
    pdf_splitter.Split();
    
    cout << "process correctly finished." << endl;
    return 0;
}
