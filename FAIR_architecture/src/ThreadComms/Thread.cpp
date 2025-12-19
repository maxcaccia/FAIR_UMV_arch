//
//  Thread.cpp
//
//  Created by Massimo Caccia on 13/10/2023.
//

#include "Thread.h"

Thread::Thread(void)
{
    mName="";
    mPriority=0;
    mPolicy=SCHED_OTHER;

    mThread=NULL;

    mEnd=false;
}

Thread::~Thread(void) {
    //printf("Thread::~Thread\n");
}


void Thread::set_properties(string name,int policy,int priority)
{
    mName=name;
    mPriority=priority;
    mPolicy=policy;

    mThread=NULL;

    mEnd=false;
}


void Thread::init(void) {
    sched_param mSchedParams;

    if(pthread_getschedparam(mThread->native_handle(),&mPolicy,&mSchedParams)!=0)
      {
      printf("Thread::execute pthread_getschedparam failed");
      return ;
      }
    mSchedParams.sched_priority=mPriority;
    if(pthread_setschedparam(mThread->native_handle(),SCHED_FIFO,&mSchedParams)!=0)
      {
      printf("Thread::execute pthread_setschedparam failed");
      return ;
      }

}

void Thread::make_thread()
{
    printf("Thread::make_thread %" PRIu64 "\n",(uint64_t)pthread_self());
    mThread = new thread(&Thread::execute,this);
}

