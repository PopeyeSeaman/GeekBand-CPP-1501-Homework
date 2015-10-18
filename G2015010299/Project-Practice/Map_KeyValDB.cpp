#include <iostream>
#include <dirent.h>
#include <sys/stat.h>
#include "Map_KeyValDB.hpp"

using std::cout;
using std::endl;
using std::ios;
using std::min_element;
using namespace NiuMoWang;


result_val Map_KeyValDB::LoadInMemory() {
    return LoadIndexF();
}

result_val Map_KeyValDB::DumpToFile() {
    
    DumpIndexF();

    for (map_dataes_t::iterator iter = _mapDataes.begin();
         iter != _mapDataes.end();
         ++iter) {
        if (iter->second.mapData.size() == 0)
            DeleteDataF(iter->first);
        else
            DumpDataF(iter->first);
    }
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::InsertKeyVal(const KeyValEntry& entry) {

    result_val retVal = RET_SUCCESS;
    string sMapId;     // 所要插入的map的Id
    
    // 在key索引map中查找是否已存在
    if (_mapIndexes.find(entry.first) == _mapIndexes.end()) {

        // 检查容量未满数据map指针vector（是否存在容量未满的数据map）
        if (_vUnfullMap.size() == 0)
            // 增加一个新的数据map，
            // 并设置map索引，容量未满数据map指针vector等信息
            sMapId = AddDataMap();
        else
            sMapId = _vUnfullMap.back().sMapId;

        // 插入KeyValue到数据map，
        // 并设置key索引，更新key计数，容量未满数据map指针vector等信息
        InsertDataToMaps(sMapId, entry);
    }
    else
        retVal = RET_EXIST;
    
    return retVal;
}

result_val Map_KeyValDB::UpdateKeyVal(const KeyValEntry& entry) {
    
    result_val retVal = RET_SUCCESS;

    // 在key索引map中查找是否已存在
    map_indexes_t::iterator iter = _mapIndexes.find(entry.first);
    if (iter != _mapIndexes.end()) {
        string sMapId = iter->second;
        
        // 在map索引中检查该map是否已载入
        if (!_mapMapInfoes[sMapId].blIsLoaded) {
            
            // 检查是否已达到数据map可载入内存最大数量值
            if (_mapDataes.size() < CST_MAXCNT_DATAMAP)
                // 载入该map对应的外存数据文件
                LoadDataF(sMapId);
            else
                // 换出最后访问时间离当前时间最久的map，换入要载入的map
                SwapDataF(sMapId);
        }

        // 在数据map中更新值
        _mapDataes[sMapId].mapData[entry.first] = entry.second;
        _mapDataes[sMapId].timeLastAccess = time(NULL);
    }
    else
        retVal = RET_UNFIND;
    
    return retVal;
}

result_val Map_KeyValDB::DeleteKeyVal(const string& sKey) {

    // 在key索引map中查找是否已存在
    map_indexes_t::iterator iter = _mapIndexes.find(sKey);
    if (iter != _mapIndexes.end()) {
        string sMapId = iter->second;

        // 在map索引中检查该map是否已载入
        if (!_mapMapInfoes[sMapId].blIsLoaded) {
            
            // 检查是否已达到数据map可载入内存最大数量值
            if (_mapDataes.size() < CST_MAXCNT_DATAMAP)
                // 载入该map对应的外存数据文件
                LoadDataF(sMapId);
            else
                // 换出最后访问时间离当前时间最久的map，换入要载入的map
                SwapDataF(sMapId);
        }
        
        // 在删除key的前一步记录对应的数据map的size（用于判断是否满）
        size_t preCntKeys = _mapDataes[sMapId].mapData.size();
        
        // 从数据map中删除KeyValEntry
        _mapDataes[sMapId].mapData.erase(sKey);
        _mapDataes[sMapId].timeLastAccess = time(NULL);
        
        // 在key索引map中删除key
        _mapIndexes.erase(sKey);

        // 递减map索引的key计数
        _mapMapInfoes[sMapId].cntKeyes -= 1;

        // 判断在删除key的前一步对应的数据map的size是否满的
        if (preCntKeys == CST_MAXSIZE_DATAMAP) {
            // 将数据map放入容量未满数据map指针vector中
            stru_unfull_mapinfo struUnfullMapIf;
            struUnfullMapIf.p_mapData = &_mapDataes[sMapId].mapData;
            struUnfullMapIf.sMapId = sMapId;
            _vUnfullMap.push_back(struUnfullMapIf);
        }
    }
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::QueryKeyVal(const string& sKey, OUTPARAM KeyValEntry& entry) {
    result_val retVal = RET_SUCCESS;

    // 在key索引map中查找key是否已存在
    map_indexes_t::iterator iter = _mapIndexes.find(sKey);
    if (iter != _mapIndexes.end()) {
        string sMapId = iter->second;

        // 在map索引中检查该map是否已载入
        if (!_mapMapInfoes[sMapId].blIsLoaded) {
            
            // 检查是否已达到数据map可载入内存最大数量值
            if (_mapDataes.size() < CST_MAXCNT_DATAMAP)
                // 载入该map对应的外存数据文件
                LoadDataF(sMapId);
            else
                // 换出最后访问时间离当前时间最久的map，换入要载入的map
                SwapDataF(sMapId);
        }
        // 从数据map中取得KeyVal
        entry.first = sKey;
        entry.second = _mapDataes[sMapId].mapData[sKey];
    }
    else
        retVal = RET_UNFIND;
    
    return retVal;
}

string Map_KeyValDB::GetDBName() const{
    return _sDBName;
}

const string Map_KeyValDB::AddDataMap() {

    // 生成mapId（对应的外存数据文件名）
    string sMapId = CST_MAPFNM_PREFIX + std::to_string(_mapDataes.size());
    
    // 新增一个数据map
    //map_data_t* const p_mapAdded = this->AddDataMap(sMapId);
    _mapDataes[sMapId].mapData = map_data_t();
    _mapDataes[sMapId].timeLastAccess = time(NULL);
    
    // 将新增的数据map放入容量未满数据map指针vector中
    stru_unfull_mapinfo struUnfullMapIf;
    struUnfullMapIf.p_mapData = &_mapDataes[sMapId].mapData;
    struUnfullMapIf.sMapId = sMapId;
    _vUnfullMap.push_back(struUnfullMapIf);
    
    // 设置map索引信息
    _mapMapInfoes[sMapId].cntKeyes = 1;
    _mapMapInfoes[sMapId].blIsLoaded = true;

    //return &_mapDataes[sMapId].mapData;
    return sMapId;
}

result_val Map_KeyValDB::InsertDataToMaps(const string& sMapId, const KeyValEntry& entry) {
    
    // 从容量未满数据map指针vector中取得末尾map，插入KeyValue数据
    _vUnfullMap.back().p_mapData->insert(entry);
    
    // 在key索引map中插入新key
    _mapIndexes[entry.first] = sMapId;
    
    // 将map索引信息的key集计数递增
    _mapMapInfoes[sMapId].cntKeyes += 1;
    
    // 刚插入数据的map容量满时
    if (_vUnfullMap.back().p_mapData->size() == CST_MAXSIZE_DATAMAP) {
        // 从容量未满数据map指针vector中删除
        _vUnfullMap.pop_back();
        
        // 保存map数据到外存文件
        DumpDataF(sMapId);
        
        // 修改map索引的是否载入信息
        _mapMapInfoes[sMapId].blIsLoaded = false;
    }
    return RET_SUCCESS;
}

result_val Map_KeyValDB::DeleteDataF(const string& sDataFName) {
    
    remove(_sDBPath.append(CST_PATH_DELIMIT).append(sDataFName).c_str());
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::SetDBPath() {

    if (_sDataPath.empty())
        IKeyValDB::SetDataPath();
    
    _sDBPath.append(_sDataPath)
            .append(CST_PATH_DELIMIT)
            .append(_sDBName);
    
    mkdir(_sDataPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
    mkdir(_sDBPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);

    return RET_SUCCESS;
}

result_val Map_KeyValDB::LoadIndexF() {
    
    DIR * dstream = opendir(_sDBPath.c_str());
    if (dstream == NULL)
        return RET_UNEXIST;
    
    struct dirent * p_struDirInfo;
    ifstream ifs;
    while ((p_struDirInfo = readdir(dstream)) != NULL) {
        string sDataFName = p_struDirInfo->d_name;
        
        if (sDataFName.find(CST_INDEX_FNM, 0) == 0)
            ReadFDtoIdxMap(ifs, sDataFName);
        
        else if (sDataFName.find(CST_MAPFNM_PREFIX, 0) == 0)
            ReadFDtoMapInfoMap(ifs, sDataFName);
        
        else
            ;
        
    }
    closedir(dstream);
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::LoadDataF(const string& sDataFName) {
    
    string sDataFPath(_sDBPath);
    sDataFPath.append(CST_PATH_DELIMIT).append(sDataFName);
    
    ifstream ifs;
    ifs.open(sDataFPath);
    
    mapsize_t cntKeys = CST_MAXSIZE_DATAMAP;
    short shKeyLen, shValLen;
    string sKey, sValue;
    if (ifs.good()) {
        ifs >> cntKeys;
    }
    while (ifs.good()) {
        ifs.width(CST_MAPF_KEYLEN);
        ifs >> shKeyLen;
        ifs.width(shKeyLen);
        ifs >> sKey;
        ifs.width(CST_MAPF_VALLEN);
        ifs >> shValLen;
        ifs.width(shValLen);
        ifs >> sValue;
        
        _mapDataes[sDataFName].mapData[sKey] = sValue;
        _mapDataes[sDataFName].timeLastAccess = time(NULL);
    }
    ifs.close();
    
    if (cntKeys < CST_MAXSIZE_DATAMAP) {
        
        // 将数据map放入容量未满数据map指针vector中
        stru_unfull_mapinfo struUnfullMapIf;
        struUnfullMapIf.p_mapData = &_mapDataes[sDataFName].mapData;
        struUnfullMapIf.sMapId = sDataFName;
        _vUnfullMap.push_back(struUnfullMapIf);
    }
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::DumpIndexF() {
    
    string sDataFPath(_sDBPath);
    sDataFPath.append(CST_PATH_DELIMIT).append(CST_INDEX_FNM);
    
    ofstream ofs;
    ofs.open(sDataFPath);
    
    if (!ofs.good())
        return RET_FILEBAD;
    
    for (map_indexes_t::iterator iter = _mapIndexes.begin();
         iter != _mapIndexes.end();
         ++iter) {
        ofs << iter->first
            << CST_KEYVAL_DELIMIT
            << iter->second
            << endl;
    }
    ofs.close();
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::DumpDataF(const string& sDataFName) {
    
    string sDataFPath(_sDBPath);
    sDataFPath.append(CST_PATH_DELIMIT).append(sDataFName);
    
    ofstream ofs;
    ofs.open(sDataFPath);
    
    if (!ofs.good())
        return RET_FILEBAD;
    
    ofs << _mapDataes[sDataFName].mapData.size() << endl;
    
    ofs.setf(ios::right);
    ofs.fill(CST_DATAF_FILL_C);
    for (map_data_t::iterator iter = _mapDataes[sDataFName].mapData.begin();
         iter != _mapDataes[sDataFName].mapData.end();
         ++iter) {
        ofs.width(CST_MAPF_KEYLEN);
        ofs << iter->first.size();
        ofs.width(iter->first.size());
        ofs << iter->first;
        
        ofs.width(CST_MAPF_VALLEN);
        ofs << iter->second.size();
        ofs.width(iter->second.size());
        ofs << iter->second;
    }
    ofs.close();
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::ReadFDtoMapInfoMap(ifstream& ifs,
                                            const string& sDataFName) {
    
    mapsize_t keyesCnt;
    ifs.open(_sDBPath + CST_PATH_DELIMIT + sDataFName);
    ifs >> keyesCnt;
    ifs.close();
    _mapMapInfoes[sDataFName].cntKeyes = keyesCnt;
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::ReadFDtoIdxMap(ifstream& ifs, const string& sIdxFName) {
  
    string sKey, sDataFName;
    ifs.open(_sDBPath + CST_PATH_DELIMIT + sIdxFName);
    while (!ifs.eof()) {
        ifs >> sKey >> sDataFName;
        if (!sKey.empty())
            _mapIndexes[sKey] = sDataFName;
    }
    ifs.close();
    
    return RET_SUCCESS;
}

result_val Map_KeyValDB::SwapDataF(const string& sDataFName) {
    
    map_dataes_t::iterator iterMinAcceTime = _mapDataes.begin();
    map_dataes_t::iterator iter = _mapDataes.begin();
    for (++iter; iter != _mapDataes.end(); ++iter) {
        if (iter->second.timeLastAccess < iterMinAcceTime->second.timeLastAccess)
            iterMinAcceTime = iter;
    }
    
    DumpDataF(iterMinAcceTime->first);
    
    LoadDataF(sDataFName);
    
    return RET_SUCCESS;
}
