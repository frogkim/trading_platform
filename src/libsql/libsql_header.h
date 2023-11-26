#include "libsql.h"
// refer: https://www.easysoft.com/developer/languages/c/odbc_tutorial.html

#ifndef LIBSQL_HEADER_H_
#define LIBSQL_HEADER_H_

#include <sql.h>
#include <sqlext.h>

#ifdef LINUX
#else   // Windows
#endif  // --- end --- Windows
#endif

SQLHENV env;
char driver[256];
char attr[256];
SQLSMALLINT driver_ret;
SQLSMALLINT attr_ret;
SQLUSMALLINT direction;
SQLRETURN ret;

bool libsql_Init()
{
    //SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &env);
    //SQLSetEnvAttr(env, SQL_ATTR_ODBC_VERSION, (void*)SQL_OV_ODBC3, 0);
    //direction = SQL_FETCH_FIRST;
    //while (SQL_SUCCEEDED(ret = SQLDrivers(env, direction,
    //                                      driver, sizeof(driver), &driver_ret,
    //                                      attr, sizeof(attr), &attr_ret))) {
    //    direction = SQL_FETCH_NEXT;
    //    if (ret == SQL_SUCCESS_WITH_INFO) printf("\tdata truncation\n");
    //}
}

// Connect
// Send query
// Request procedure
// Recv data





