#include "IKeyValDB.h"

using namespace NiuMoWang;
using std::string;
using std::ofstream;
using std::ios;
using std::endl;

string IKeyValDB::_sDataPath = "";

result_val IKeyValDB::SetDataPath() {
    
    //string sDataPath;
    const size_t BUFF_SIZE = 256;
    char csCwdPath[BUFF_SIZE];
    getcwd(csCwdPath, BUFF_SIZE);
    
    _sDataPath.append(csCwdPath)
            .append(CST_PATH_DELIMIT)
            .append(CST_DBDATA_DIR);
    mkdir(_sDataPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH);
    
    return RET_SUCCESS;
}

string IKeyValDB::GetDataPath() {
    return _sDataPath;
}

result_val IKeyValDB::CreateDB(const string& sDBName, const string& sDBType) {
    
    string sDBPath(_sDataPath);
    sDBPath.append(CST_PATH_DELIMIT).append(sDBName);
    if (mkdir(sDBPath.c_str(), S_IRWXU | S_IRGRP | S_IROTH) != 0)
        return RET_MKDBDIR_FAIL;
    
    ofstream ofs;
    ofs.open(_sDataPath + CST_PATH_DELIMIT + CST_DBNAME_LIST, ios::app);
    ofs << sDBName << CST_KEYVAL_DELIMIT << sDBType << endl;
    ofs.close();

    ofs.open(sDBPath + CST_PATH_DELIMIT + CST_INDEX_FNM);
    ofs.close();
    
    return RET_SUCCESS;
}

result_val IKeyValDB::DisposeDB(const string& sDBName) {

    string sDBPath(_sDataPath);
    sDBPath.append(CST_PATH_DELIMIT).append(sDBName);
    if (rmdir(sDBPath.c_str()) != 0)
        return RET_RMDBDIR_FAIL;
    
    return RET_SUCCESS;
}
