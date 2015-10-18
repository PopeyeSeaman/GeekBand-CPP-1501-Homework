#ifndef KeyValDBDefs_h
#define KeyValDBDefs_h

#include <string>

namespace NiuMoWang {
    
    using std::string;
    using std::pair;
    
    #ifndef INPARAM
    #define INPARAM
    #endif
    
    #ifndef OUTPARAM
    #define OUTPARAM
    #endif
    
#ifdef OS_WINDOWS
    const string CST_PATH_DELIMIT = "\\";
#else
    const string CST_PATH_DELIMIT = "/";
#endif
    
    const string CST_DBNAME_LIST = "DBLIST";
    const string CST_DBDATA_DIR = "DB_DATA";
    const string CST_INDEX_FNM = "INDEXS";

    const string CST_DBTYPE_MAP = "M";      // MAP
    const string CST_DBTYPE_MULTIMAP = "P"; // MULTIMAP
    const string CST_DBTYPE_SET = "S";      // SET
    const string CST_DBTYPE_MULTISET = "T"; // MULTISET
    const string CST_DBTYPE_VECTOR = "V";   // VECTOR
    const string CST_DBTYPE_LIST = "L";     // LIST

    const string CST_DBMNG_CREATE_L = "c";
    const string CST_DBMNG_CREATE_U = "C";
    const string CST_DBMNG_DISPOSE_L = "d";
    const string CST_DBMNG_DISPOSE_U = "D";
    const string CST_DBMNG_OPEN_L = "o";
    const string CST_DBMNG_OPEN_U = "O";
    const string CST_DBMNG_CLOSE_L = "l";
    const string CST_DBMNG_CLOSE_U = "L";

    const string CST_DBOP_QUERY_L = "q";
    const string CST_DBOP_QUERY_U = "Q";
    const string CST_DBOP_INSERT_L = "i";
    const string CST_DBOP_INSERT_U = "I";
    const string CST_DBOP_UPDATE_L = "u";
    const string CST_DBOP_UPDATE_U = "U";
    const string CST_DBOP_DELETE_L = "d";
    const string CST_DBOP_DELETE_U = "D";
    const string CST_DBOP_EXIT_L = "e";
    const string CST_DBOP_EXIT_U = "E";

    const string CST_PGNM_MAP = "Map_KeyValDB";
    const string CST_PGNM_MULTIMAP = "MulMap_KeyValDB";
    const string CST_PGNM_VECTOR = "Vec_KeyValDB";

    const string CST_KEYVAL_DELIMIT = " ";
    const char CST_DATAF_FILL_C = '0';
    
    const string CST_DBSERVER_ADDRESS = "127.0.0.1";
    const unsigned short CST_DBSERVER_PORT = 6000;
    const int CST_SOCKBUFF_SIZE = 1024;
    const unsigned short CST_INITCNT_THREAD = 100;
    const unsigned short CST_MAXCNT_THREAD = 1000;
    const unsigned int CST_SECOND_CNT_24H = 24 * 60 * 60;
    
    const string CST_INPUT_DBMNGTYPE =
        "Input manage type: C(Create) D(Dispose) O(Open) L(CLose) E(Exit)?";
    const string CST_INPUT_DBNAME = "Input DBName:";
    const string CST_INPUT_DBTYPE =
        "Input database type: M(Map) P(MultiMap) S(Set) T(MultiSet) V(Vector) L(List) ?";
    const string CST_INPUT_DBOPTYPE =
        "Input operate type: Q(Query) I(Insert) U(Update) D(Delete) E(Exit) ?";
    const string CST_INPUT_KEY = "Input Key:";
    const string CST_INPUT_VALUE = "Input Value:";
    
    const string CST_MSG_OK_CRE = "Create database successed.";
    const string CST_MSG_OK_DIS = "Dispose database successed.";
    const string CST_MSG_OK_OPE = "Open database successed.";
    const string CST_MSG_OK_CLO = "Close database successed.";
    const string CST_MSG_OK_INS = "Insert operate successed.";
    const string CST_MSG_OK_UPD = "Update operate successed.";
    const string CST_MSG_OK_DEL = "Delete operate successed.";
    
    enum db_operate_type {
        DBOPTYPE_INSERT,
        DBOPTYPE_UPDATE,
        DBOPTYPE_DELETE,
        DBOPTYPE_QUERY
    };
    
    enum result_val {
        RET_SUCCESS,
        RET_UNFIND,
        RET_EXIST,
        RET_UNEXIST,
        RET_FILEBAD,
        RET_MKDBDIR_FAIL,
        RET_RMDBDIR_FAIL,
        RET_FAIL
    };
    
    typedef pair<string, string> KeyValEntry;
    
    typedef struct {
        string sUserAddr;
        string sUserPort;
        string sDBName;
        db_operate_type dbOpType;
        KeyValEntry dataEntry;
        time_t lOpTime;
    } stru_dbop_log;
}

#endif /* KeyValDBDefs_h */
