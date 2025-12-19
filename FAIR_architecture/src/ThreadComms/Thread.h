//
//  Thread.h
//
//  Created by Massimo Caccia on 13/10/2023.
//

#ifndef Thread_h
#define Thread_h

#include <chrono>
//#include <cinttypes>
//#include <csignal>
//#include <sched.h>
#include <stdlib.h>
#include <thread>
//#include <unistd.h>
#include <stdio.h>
#include <string>
//#include <pthread.h>
#include <inttypes.h>

using namespace std;

//
// thread time period
//
#define TIME_PERIOD_1HZ 1000 // time period in milliseconds at 1 Hz
#define TIME_PERIOD_10HZ 100 // time period in milliseconds at 10 Hz
#define TIME_PERIOD_20HZ 50 // time period in milliseconds at 20 Hz
#define TIME_PERIOD_100HZ 10 // time period in milliseconds at 100 Hz
#define TIME_PERIOD_200HZ 5 // time period in milliseconds at 100 Hz
#define TIME_PERIOD_NOT_VALID -1 // not valid time period for asynchronous threads

//
// thread priority
//
#define DEVICE_SIMULATOR_PRIORITY 60
#define RIGID_BODY_SIMULATOR_PRIORITY 60
#define NGC_PRIORITY 50
#define DRIVER_PRIORITY 55


class Thread {
protected:
	string mName;
    int mPolicy;
    int mPriority;

    bool mEnd;
    
    thread *mThread;
    
    virtual void execute(void) {printf("virtual Thread::execute()\n"); }

    void init(void);
    void delete_thread(void) { mThread->join(); mThread=NULL; }

//    virtual void execute_body(void) {printf("Thread::execute_body %s\n",mName.c_str());} // function to be overwritten

public:
    Thread(void);
    virtual ~Thread(void);
    
    void set_properties(string name,int policy,int priority);
    void set_name(string name) { mName=name; }

    virtual void make_thread(void);
    virtual void end(void) {mEnd=true;}
};

#endif /* SynchronousThread_h */
