#ifndef IKeyValDB_h
#define IKeyValDB_h

#include <fstream>
#include <dirent.h>
#include <sys/stat.h>
#include "KeyValDBDefs.h"

namespace NiuMoWang {
    
    using std::string;
    
    class IKeyValDB {

    protected:
        static string _sDataPath;   // 各个数据库的数据文件夹的公共父目录
        string _sDBName;            // 数据库名

    public:
        IKeyValDB(const string& sDBName) : _sDBName(sDBName) {}
        
        static result_val SetDataPath();
        static string GetDataPath();
        
        static result_val CreateDB(const string& sDBName, const string& sDBType);
        static result_val DisposeDB(const string& sDBName);

        virtual result_val LoadInMemory() = 0;
        virtual result_val DumpToFile() = 0;
        
        virtual result_val InsertKeyVal(const KeyValEntry& entry) = 0;
        virtual result_val UpdateKeyVal(const KeyValEntry& entry) = 0;
        virtual result_val DeleteKeyVal(const string& sKey) = 0;
        virtual result_val QueryKeyVal(const string& sKey, OUTPARAM KeyValEntry& entry) = 0;
        
        virtual string GetDBName() const = 0;
        
        virtual ~IKeyValDB() {}
        
    };
}

#endif /* IKeyValDB_h */
