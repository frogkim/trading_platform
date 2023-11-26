#ifndef LIBSQL_H_
#define LIBSQL_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBSQL __attribute__((visibility("default")))
#else
#define LIBSQL
#endif

#else   // windows
#ifdef EXPORT
#define LIBSQL __declspec(dllexport)
#else
#define LIBSQL __declspec(dllimport)
#endif
#endif

extern "C"
{

}
#endif // end LIBSQL_H_
