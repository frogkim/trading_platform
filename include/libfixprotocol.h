#ifndef LIBFIXPROTOCOL_H_
#define LIBFIXPROTOCOL_H_

#ifdef LINUX
#ifdef EXPORT
#define LIBNETWORK __attribute__((visibility("default")))
#else
#define LIBNETWORK
#endif

#else  // Windows
#ifdef EXPORT
#define LIBNETWORK __declspec(dllexport)
#else
#define LIBNETWORK __declspec(dllimport)
#endif

#endif

#endif