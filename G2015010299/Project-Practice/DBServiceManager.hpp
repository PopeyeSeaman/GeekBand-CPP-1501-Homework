#ifndef DBServiceManager_hpp
#define DBServiceManager_hpp

#include <map>
#include "KeyValDBDefs.h"
#include "Map_KeyValDB.hpp"

namespace NiuMoWang {
    
    using std::map;
    
    class DBServiceManager {
        
        map<string, pid_t> _mapChildPid;
        map<string, string> _mapExistDB;
        
    public:
        DBServiceManager() {
            IKeyValDB::SetDataPath();
            LoadDBList();
        }
        
        result_val LoadDBList();
        result_val CreateDB(const string& sDBName, const string& sDBType);
        result_val DisposeDB(const string& sDBName);
        result_val OpenDB(const string& sDBName);
        result_val CloseDB(const string& sDBName);
    };
}

#endif /* DBServiceManager_hpp */
