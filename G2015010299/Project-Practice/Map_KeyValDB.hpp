#ifndef Map_KeyValDB_hpp
#define Map_KeyValDB_hpp

#include <fstream>
#include "IKeyValDB.h"
#include "Map_DBDefs.h"

namespace NiuMoWang {
    
    using std::ifstream;
    using std::ofstream;

    class Map_KeyValDB : public IKeyValDB {
        
        string _sDBPath;                             // 保存数据库文件的目录路径
        
        map_dataes_t _mapDataes;                     // KeyValue数据map（每一个map对应一个外存的数据文件）的map
        map_indexes_t _mapIndexes;                   // key索引map
        map_mapinfoes_t _mapMapInfoes;               // map索引相关信息map
        v_unfull_map _vUnfullMap;                    // 保存容量未满的KeyValue数据map指针的vector
        
    public:
        Map_KeyValDB(const string& sDBName) : IKeyValDB(sDBName) {
            SetDBPath();
            LoadInMemory();
        }
        
        //result_val CreateDB();
        //result_val DisposeDB();
        
        result_val LoadInMemory();
        result_val DumpToFile();
        
        result_val InsertKeyVal(const KeyValEntry& entry);
        result_val UpdateKeyVal(const KeyValEntry& entry);
        result_val DeleteKeyVal(const string& sKey);
        result_val QueryKeyVal(const string& sKey, OUTPARAM KeyValEntry& entry);
        
        string GetDBName() const;
        
        ~Map_KeyValDB() {
            DumpToFile();
        }
        
    private:
        result_val SetDBPath();
        result_val LoadIndexF();
        result_val LoadDataF(const string& sDataFName);
        result_val DumpIndexF();
        result_val DumpDataF(const string& sDataFName);
        const string AddDataMap();
        result_val InsertDataToMaps(const string& sMapId, const KeyValEntry& entry);
        result_val DeleteDataF(const string& sDataFName);
        result_val ReadFDtoIdxMap(ifstream& ifs, const string& sIdxFName);
        result_val ReadFDtoMapInfoMap(ifstream& ifs, const string& sDataFName);
        result_val SwapDataF(const string& sDataFName);
    };

}

#endif /* Map_KeyValDB_hpp */
