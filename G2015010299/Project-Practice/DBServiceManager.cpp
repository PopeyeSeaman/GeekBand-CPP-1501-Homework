#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <signal.h>
#include "DBServiceManager.hpp"

using namespace NiuMoWang;

result_val DBServiceManager::LoadDBList() {
    
    string sDBName;
    string sDBType;
    ifstream ifs;
    ifs.open(IKeyValDB::GetDataPath() + CST_PATH_DELIMIT + CST_DBNAME_LIST);
    while (!ifs.eof()) {
        ifs >> sDBName >> sDBType;
        _mapExistDB[sDBName] = sDBType;
    }
    ifs.close();
    
    return RET_SUCCESS;
}

result_val DBServiceManager::CreateDB(const string& sDBName, const string& sDBType) {

    return IKeyValDB::CreateDB(sDBName, sDBType);
}

result_val DBServiceManager::DisposeDB(const string& sDBName) {
    
    return IKeyValDB::DisposeDB(sDBName);
}

result_val DBServiceManager::OpenDB(const string& sDBName) {
    
    string sDBType = _mapExistDB[sDBName];
    pid_t pid;
    if ((pid = fork()) == 0) {
        //close(sock_listen);
        //close(sock_conn);
        
        execl("./DBService", sDBName.c_str(), sDBType.c_str());
        exit(0);
    }
    _mapChildPid[sDBName] = pid;
    

    return RET_SUCCESS;
}

result_val DBServiceManager::CloseDB(const string& sDBName) {
    
    kill(_mapChildPid[sDBName], SIGQUIT);
    
    return RET_SUCCESS;
}

int mainm() {
    
    DBServiceManager dbManager;
    dbManager.OpenDB("DB1");
    
    return 0;
}
