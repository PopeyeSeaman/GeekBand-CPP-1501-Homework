#ifndef Map_DBDefs_h
#define Map_DBDefs_h

#include <string>
#include <map>
#include <vector>
#include "KeyValDBDefs.h"

namespace NiuMoWang {
    
    using std::string;
    using std::map;
    using std::vector;
    
    const string CST_MAPFNM_PREFIX = "DATAMAP";
    const short CST_MAPF_KEYLEN = 10;
    const short CST_MAPF_VALLEN = 10;
    
    // 与DB所使用的map的size相关联的类型
    typedef unsigned long mapsize_t;
    
    // map索引相关信息map
    typedef struct {
        mapsize_t cntKeyes;     // map中所包含key的总数
        bool blIsLoaded;        // 该map是否已载入内存
    } stru_map_info;
    
    
    // key索引map类型
    typedef map<const string, string> map_indexes_t;
    
    // map索引相关信息map
    typedef map<const string, stru_map_info> map_mapinfoes_t;
    
    // KeyValue数据map
    typedef map<const string, string> map_data_t;
    
    // KeyValue数据map结构体
    typedef struct {
        map_data_t mapData;     // KeyValue数据map
        //string sMapId;          // KeyValue数据map的ID（对应的外存数据文件名）
        time_t timeLastAccess;  // 最后访问时间
    } stru_map_data;
    
    // KeyValue数据map（每一个map对应一个外存的数据文件）的map
    typedef map<const string, stru_map_data> map_dataes_t;
    
    // 容量未满的KeyValue数据map结构体
    typedef struct {
        map_data_t* p_mapData;  // KeyValue数据map的指针
        string sMapId;          // KeyValue数据map的ID（对应的外存数据文件名）
    } stru_unfull_mapinfo;
    
    // 保存容量未满的KeyValue数据map指针的vector
    typedef vector<stru_unfull_mapinfo> v_unfull_map;
    
    // KeyValue数据map的最大size（元素个数）
    const mapsize_t CST_MAXSIZE_DATAMAP = 1024 * 1024;
    
    // KeyValue数据map的最大数量（map个数）
    const mapsize_t CST_MAXCNT_DATAMAP = 10;
    
}

#endif /* Map_DBDefs_h */
