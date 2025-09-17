#ifndef MAIN_H
#define MAIN_H

#include <string>
#include <vector>
#include <cstdint>
#include "resource.h"

struct slide{
    vector<uint8_t> img;
    string txt;
    bool input;
    string in;
};

vector<string> stakeholder_check(sqlite3 *DB, string entry){
    vector<string> output;
    sqlite3_stmt *stmt;
    string s_sql = "SELECT parties FROM stakeholders WHERE ? LIKE '%' || parties || '%' OR ? LIKE '%' || word1 || '%' OR ? LIKE '%' || word2 || '%' OR ? LIKE '%' || word3 || '%' OR ? LIKE '%' || word4 || '%' OR ? LIKE '%' || word5 || '%' OR ? LIKE '%' || word6 || '%' OR ? LIKE '%' || word7 || '%' OR ? LIKE '%' || word8 || '%' OR ? LIKE '%' || word9 || '%' OR ? LIKE '%' || word10 || '%';";
    const char *sql = s_sql.c_str();
    sqlite3_prepare(DB,sql,-1,&stmt,NULL);
    for(int i = 1; i < sqlite3_bind_parameter_count(stmt); i ++)
        sqlite3_bind_text(stmt,i,entry.c_str(),-1,SQLITE_STATIC);
    while(sqlite3_step(stmt) != SQLITE_DONE)
        output.push_back((const char *)sqlite3_column_text(stmt,0));
    sqlite3_finalize(stmt);
    return output;
}

string printList(vector<string> output){
    string list;
    if(output.empty()){}
    else if(output.size() == 1)
        list += output.back();
    else if(output.size() == 2)
        list += output.front() + " and " + output.back();
    else{
        for(int i = 0; i < output.size() - 1; i ++)
            list += output[i] + ", ";
        list += "and " + output.back();
    }
    if(list[0])
        list[0] = toupper(list[0]);
    return list;
}

#endif