#ifndef _PTHREAD_H
#define _PTHREAD_H

#ifdef WIN32

#include <windows.h>
#include <stdio.h>

typedef void * pthread_t;
#if 1
#define pthread_create(ptid,pattr,pfn,parg) (!(*ptid = (pthread_t)CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)pfn,(LPVOID)parg,0,NULL)))
#else
inline int pthread_create( pthread_t *ptid, void *pattr, void *pfn, void *parg)
{
	int ret;
	DWORD tid;
	tid = (DWORD)CreateThread( 0, 0, (LPTHREAD_START_ROUTINE)pfn,parg,0,NULL);
	printf( "CreateThread returned ret=%d tid=%08lx\n", ret, tid);
	*ptid = (pthread_t)tid;
	ret = !tid;
	return ret;
}
#endif
#define pthread_cancel(tid)	(!TerminateThread( (HANDLE)tid, 0))
#if 1
#define pthread_self()		((pthread_t)GetCurrentThreadId())
#elif 0
#define pthread_self()		((pthread_t)GetCurrentThread())
#else
inline pthread_t pthread_self()
{
	HANDLE h;
	h = OpenThread( 0, 0, GetCurrentThreadId());
	CloseHandle( h);
	return (pthread_t)h;
}
#endif
#if 0
#define pthread_join(tid,foo) (WaitForSingleObject( (HANDLE)tid, INFINITE) != WAIT_FAILED)
#else
#if 0
#define pthread_join(tid,ptr) (!GetExitCodeThread( (HANDLE)tid, (LPDWORD)ptr))
#elif 0
inline int pthread_join( pthread_t tid, void **ptr)
{
	DWORD rc;
	int ret;

	printf( "About to GetExitCodeThread tid=%08lx\n", (DWORD)tid);
	ret = GetExitCodeThread( (HANDLE)tid, ptr ? (LPDWORD)ptr : &rc);
	printf( "ret=%d\n", ret);
	if (!ret)
	{
		printf( "failed : %08lx\n", GetLastError());
	}
	ret = !ret;

	return ret;
}
#else
inline int pthread_join( pthread_t tid, void **ptr)
{
	DWORD rc = STILL_ACTIVE;
	LPDWORD prc;
	int ret;

	if (ptr)
		prc = (LPDWORD)ptr;
	else
		prc = &rc;
//	printf( "About to GetExitCodeThread tid=%08lx\n", (DWORD)tid);
	while (ret && *prc == STILL_ACTIVE)
	{
		ret = GetExitCodeThread( (HANDLE)tid, prc);
	}
//	printf( "ret=%d\n", ret);
	if (!ret)
	{
		printf( "GetExitCodeThread failed : %08lx\n", GetLastError());
	}
	ret = !ret;

	return ret;
}
#endif
#endif

#define pthread_mutex_t HANDLE
#define pthread_mutex_init(ptr,foo) (!(*ptr=CreateMutex( 0, FALSE, 0)))
#define pthread_mutex_destroy(ptr) CloseHandle( *ptr)
#define pthread_mutex_lock(ptr) WaitForSingleObject( *ptr, INFINITE)
#define pthread_mutex_unlock(ptr) ReleaseMutex( *ptr)

#endif

#endif

