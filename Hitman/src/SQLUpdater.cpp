// gcc -c sqlite3.c && g++ SQLUpdater.cpp sqlite3.o && rm sqlite3.o
#include "sqlite3.h"
#include <iostream>
#include <string>
using namespace std;

int callback(void *nil, int argc, char **argv, char **azColName);

int main(){
    char DB[] = "../resources/hitman.db";
    sqlite3 *hitmanDB;
    int err = sqlite3_open(DB,&hitmanDB);
    if (err) 
        cerr << "Can't open database: " << sqlite3_errmsg(hitmanDB) << endl;

    const char *sql;
    char *errmsg;

    // EXAMPLE COMMAND
    /*
    sql = "UPDATE stakeholders SET word3 = \'employees\', word4 = \'staff\' WHERE word1 = \'laterals\'";
    err = sqlite3_exec(hitmanDB,sql,callback,NULL,&errmsg);
    if(err){
        cerr << "SQL error: " << errmsg << endl;
        sqlite3_free(errmsg);
    }
    */

    //sql = "SELECT * FROM stakeholders;";
    //err = sqlite3_exec(hitmanDB,sql,callback,NULL,&errmsg);
    //if(err){
    //    cerr << "SQL error: " << errmsg << endl;
    //    sqlite3_free(errmsg);
    //}

    sqlite3_close(hitmanDB);
}

int callback(void *nil, int argc, char **argv, char **azColName){
    for(int i = 0; i < argc; i ++){
        printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
    }
    printf("\n");
    return 0;
}