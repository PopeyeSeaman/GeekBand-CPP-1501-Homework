#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include "Map_KeyValDB.hpp"

using std::cout;
using std::endl;
using namespace NiuMoWang;

typedef struct {
    db_operate_type dbOpType;
    string sKey;
    string sValue;
} stru_sql_command;

typedef struct {
    //pthread_t tid;
    //bool is_enabled;
    int sock_conn;
    struct sockaddr_in addr_client;
    socklen_t len_sockaddr;
    char buff_recv[CST_SOCKBUFF_SIZE];
    char buff_send[CST_SOCKBUFF_SIZE];
    IKeyValDB* p_KeyValDB;
} stru_thread_param;

typedef vector<stru_thread_param> v_thread_param_t;
typedef vector<pthread_t> v_pthread_t;

void Run(int sock_listen, IKeyValDB* p_KeyValDB);
IKeyValDB * LoadDB(const string& db_name, const string& db_type);
result_val DumpDB();
int Listen();
void* Service(void* thread_params);
void* DumpDBTimer(void* p_KeyValDB);
void do_dumpdb(int p_KeyValDB);

pthread_mutex_t g_mutex = PTHREAD_MUTEX_INITIALIZER;


int main(int argc, const char * argv[]) {

    const string db_name = argv[1];
    const string db_type = argv[2];
    int sock_listen;
    IKeyValDB* p_KeyValDB;
    
    p_KeyValDB = LoadDB(db_name, db_type);

    sock_listen = Listen();
    
    signal(SIGQUIT, do_dumpdb);
    
    Run(sock_listen, p_KeyValDB);
    
    return 0;
}

IKeyValDB * LoadDB(const string& db_name, const string& db_type) {
    
    IKeyValDB* p_KeyValDB;
    switch (db_type[0]) {
        case 'm':
        case 'M':
            p_KeyValDB = new Map_KeyValDB(db_name);
            break;
        case 's':
        case 'S':
            break;
        default:
            break;
    }
    
    return p_KeyValDB;
}

result_val DumpDB(const string& db_name, IKeyValDB* p_KeyValDB) {
    
    p_KeyValDB->DumpToFile();
    return RET_SUCCESS;
}

int Listen() {

    int sock_listen;
    socklen_t len_sockaddr;
    struct sockaddr_in addr_server;
    addr_server.sin_addr.s_addr = htonl(INADDR_ANY);
    addr_server.sin_family = AF_INET;
    addr_server.sin_port = htons(CST_DBSERVER_PORT);
    
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    len_sockaddr = sizeof(struct sockaddr);
    if (bind(sock_listen, (struct sockaddr *)&addr_server, len_sockaddr) != 0) {
        cout << "bind failed." << endl;
        close(sock_listen);
        exit(EXIT_FAILURE);
    }
    if (listen(sock_listen, 10) != 0) {
        cout << "listen failed." << endl;
        close(sock_listen);
        exit(EXIT_FAILURE);
    }
    
    return sock_listen;
}

void Run(int sock_listen, IKeyValDB* p_KeyValDB) {
    
    cout << p_KeyValDB->GetDBName() << " database startuped." << endl;
    
    v_pthread_t* v_pthread = new v_pthread_t(CST_INITCNT_THREAD);
    v_thread_param_t* v_thread_param = new v_thread_param_t(CST_INITCNT_THREAD);
    
    pthread_t pthread;
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    
    pthread_create(&pthread, &attr, DumpDBTimer, NULL);
    
    for (int i = 0; i < CST_INITCNT_THREAD; ++i) {
        
        (*v_thread_param)[i].p_KeyValDB = p_KeyValDB;
        
        (*v_thread_param)[i].sock_conn = accept(sock_listen,
                                             (struct sockaddr *)&(*v_thread_param)[i].addr_client,
                                             &(*v_thread_param)[i].len_sockaddr);
        if ((*v_thread_param)[i].sock_conn != -1) {
            char pIPAddr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET,
                      &(*v_thread_param)[i].addr_client.sin_addr.s_addr,
                      pIPAddr,
                      INET6_ADDRSTRLEN);
            cout << "socket connect from client "
            << pIPAddr << ":"
            << ntohs((*v_thread_param)[i].addr_client.sin_port) << " success." << endl;
        }
        else {
            cout << "connect failed." << endl;
            close(sock_listen);
            exit(EXIT_FAILURE);
        }
        
        sprintf((*v_thread_param)[i].buff_send,
                "welcome to %s database.",
                p_KeyValDB->GetDBName().c_str());
        send((*v_thread_param)[i].sock_conn,
             (*v_thread_param)[i].buff_send,
             strlen((*v_thread_param)[i].buff_send) + 1,
             0);
        
        pthread_create(&(*v_pthread)[i], &attr, Service, &(*v_thread_param)[i]);
    }
    
    pthread_exit(0);
}

void* Service(void* thread_params) {
    
    stru_thread_param* p_thread_params = (stru_thread_param *)thread_params;
    for ( ; ; ) {
        
        sprintf(p_thread_params->buff_send, "%s", CST_INPUT_DBOPTYPE.c_str());
        send(p_thread_params->sock_conn,
             p_thread_params->buff_send,
             strlen(p_thread_params->buff_send) + 1,
             0);
        
        long len_msg = recv(p_thread_params->sock_conn,
                            p_thread_params->buff_recv,
                            sizeof(p_thread_params->buff_recv), 0);
        if (CST_DBOP_EXIT_L.compare(p_thread_params->buff_recv) == 0
            || CST_DBOP_EXIT_U.compare(p_thread_params->buff_recv) == 0) {

            char pIPAddr[INET6_ADDRSTRLEN];
            inet_ntop(AF_INET,
                      &p_thread_params->addr_client.sin_addr.s_addr,
                      pIPAddr,
                      INET6_ADDRSTRLEN);
            cout << "Client " << pIPAddr << ":"
                << ntohs(p_thread_params->addr_client.sin_port) << " quit." << endl;
            pthread_exit(0);
        }
        
        KeyValEntry entry;
        result_val ret;
        string sMsgSend;
        switch (p_thread_params->buff_recv[0]) {
            case 'i':
            case 'I':
                sprintf(p_thread_params->buff_send, "%s", CST_INPUT_KEY.c_str());
                send(p_thread_params->sock_conn,
                     p_thread_params->buff_send,
                     strlen(p_thread_params->buff_send) + 1, 0);
                
                memset(p_thread_params->buff_recv, '\0', CST_SOCKBUFF_SIZE);
                recv(p_thread_params->sock_conn,
                     p_thread_params->buff_recv,
                     sizeof(p_thread_params->buff_recv), 0);
                entry.first = p_thread_params->buff_recv;
                
                sprintf(p_thread_params->buff_send, "%s", CST_INPUT_VALUE.c_str());
                send(p_thread_params->sock_conn,
                     p_thread_params->buff_send,
                     strlen(p_thread_params->buff_send) + 1, 0);
                
                memset(p_thread_params->buff_recv, '\0', CST_SOCKBUFF_SIZE);
                recv(p_thread_params->sock_conn,
                     p_thread_params->buff_recv,
                     sizeof(p_thread_params->buff_recv), 0);
                entry.second = p_thread_params->buff_recv;
                
                pthread_mutex_lock(&g_mutex);
                ret = p_thread_params->p_KeyValDB->InsertKeyVal(entry);
                pthread_mutex_unlock(&g_mutex);
                
                if (ret == RET_SUCCESS)
                    sMsgSend = CST_MSG_OK_INS;
                
                sprintf(p_thread_params->buff_send, "%s", sMsgSend.c_str());
                send(p_thread_params->sock_conn,
                     p_thread_params->buff_send,
                     strlen(p_thread_params->buff_send) + 1, 0);
                break;
                
            default:
                break;
        }
    }
    close(p_thread_params->sock_conn);
    
}

void* DumpDBTimer(void* p_KeyValDB) {
    
    time_t pre_tm;
    pre_tm = time(NULL);
    for ( ; ; ) {
        if (time(NULL) - pre_tm > CST_SECOND_CNT_24H) {
            pthread_mutex_lock(&g_mutex);
            ((IKeyValDB*)p_KeyValDB)->DumpToFile();
            pthread_mutex_unlock(&g_mutex);
            pre_tm = time(NULL);
        }
    }
}

void do_dumpdb(int p_KeyValDB) {
    
    pthread_mutex_lock(&g_mutex);
    ((IKeyValDB*)p_KeyValDB)->DumpToFile();
    pthread_mutex_unlock(&g_mutex);
}
