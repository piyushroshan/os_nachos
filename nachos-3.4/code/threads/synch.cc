// synch.cc 
//	Routines for synchronizing threads.  Three kinds of
//	synchronization routines are defined here: semaphores, locks 
//   	and condition variables (the implementation of the last two
//	are left to the reader).
//
// Any implementation of a synchronization routine needs some
// primitive atomic operation.  We assume Nachos is running on
// a uniprocessor, and thus atomicity can be provided by
// turning off interrupts.  While interrupts are disabled, no
// context switch can occur, and thus the current thread is guaranteed
// to hold the CPU throughout, until interrupts are reenabled.
//
// Because some of these routines might be called with interrupts
// already disabled (Semaphore::V for one), instead of turning
// on interrupts at the end of the atomic operation, we always simply
// re-set the interrupt state back to its original value (whether
// that be disabled or enabled).
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "synch.h"
#include "system.h"

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	Initialize a semaphore, so that it can be used for synchronization.
//
//	"debugName" is an arbitrary name, useful for debugging.
//	"initialValue" is the initial value of the semaphore.
//----------------------------------------------------------------------

Semaphore::Semaphore(char* debugName, int initialValue)
{
    name = debugName;
    value = initialValue;
    queue = new List;
}

//----------------------------------------------------------------------
// Semaphore::Semaphore
// 	De-allocate semaphore, when no longer needed.  Assume no one
//	is still waiting on the semaphore!
//----------------------------------------------------------------------

Semaphore::~Semaphore()
{
    delete queue;
}

//----------------------------------------------------------------------
// Semaphore::P
// 	Wait until semaphore value > 0, then decrement.  Checking the
//	value and decrementing must be done atomically, so we
//	need to disable interrupts before checking the value.
//
//	Note that Thread::Sleep assumes that interrupts are disabled
//	when it is called.
//----------------------------------------------------------------------

void
Semaphore::P()
{
    IntStatus oldLevel = interrupt->SetLevel(IntOff);	// disable interrupts
    
    while (value == 0) { 			// semaphore not available
	queue->Append((void *)currentThread);	// so go to sleep
	currentThread->Sleep();
    } 
    value--; 					// semaphore available, 
						// consume its value
    
    (void) interrupt->SetLevel(oldLevel);	// re-enable interrupts
}

//----------------------------------------------------------------------
// Semaphore::V
// 	Increment semaphore value, waking up a waiter if necessary.
//	As with P(), this operation must be atomic, so we need to disable
//	interrupts.  Scheduler::ReadyToRun() assumes that threads
//	are disabled when it is called.
//----------------------------------------------------------------------

void
Semaphore::V()
{
    Thread *thread;
    IntStatus oldLevel = interrupt->SetLevel(IntOff);

    thread = (Thread *)queue->Remove();
    if (thread != NULL)	   // make thread ready, consuming the V immediately
	scheduler->ReadyToRun(thread);
    value++;
    (void) interrupt->SetLevel(oldLevel);
}

// Dummy functions -- so we can compile our later assignments 
// Note -- without a correct implementation of Condition::Wait(), 
// the test case in the network assignment won't work!
Lock::Lock(char* debugName) {}
Lock::~Lock() {}
void Lock::Acquire() {
    l->P();
    t = currentThread;
}
void Lock::Release() {
    l->V();
    t = NULL;
}

bool isHeldByCurrentThread(){
    return t == currentThread;
}

Condition::Condition(char* debugName, Lock* conditionLock) {
    name = debugName; 
    l = Lock;
}
Condition::~Condition() { 
    delete l;
}
void Condition::Wait(Lock* conditionLock) { 
    ASSERT(l->isHeldByCurrentThread()); 
    queue->Append(t);
    l->Release();
    t->Sleep();
}
void Condition::Signal(Lock* conditionLock) {
    ASSERT(l->isHeldByCurrentThread());
    Thread thread = queue->Remove;
    if(thread != NULL) scheduler->ReadyToRun(thread);
 }
void Condition::Broadcast(Lock* conditionLock) {
    ASSERT(l->isHeldByCurrentThread());
    Thread thread;
    while(thread = queue->Remove())
    {
        scheduler->ReadyToRun(thread);
    }
 }


/*

Reader::Reader(char* DebugName)
{
  name = DebugName;
}   

Reader::~Reader()
{
}

void Reader::read(int num)
{

    int i=0;
    // Insert Code here
    // insert more code here.
    signal_counter.P();
    readercount++;
    if (readercount == 1)
        signal_writer.P();
    signal_counter.V();
    // For the simulation of the reading.  (Do not change)
    printf("%s starts reading.\n",currentThread->getName());
    while (i<num)
    {
        printf("%s is reading the page of %d in the database\n",currentThread->getName(),i);
        i++;
    }
    printf("%s finishes reading.\n",currentThread->getName());
    signal_counter.P();
    readercount --;
    if (readercount == 0 )
        signal_writer.V();
    signal_counter.V();
}

Writer::Writer(char* DebugName)
{
  name = DebugName;
}

Writer::~Writer()
{
}

void Writer::write( int num )
{
    int i = 0;
    // Insert Code here

    signal_writer.P();

    printf("%s starts writing.\n",currentThread->getName());
    // For the simulation of the writing.  (Do not change)
    while ( i < num )
    {
        printf( "%s is writing the page of %d in the database\n", currentThread->getName(), i );
        i++;
    }
    signal_writer.V();
    printf("%s finish writing\n",currentThread->getName());
}
*/