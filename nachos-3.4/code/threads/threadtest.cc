// threadtest.cc 
//	Simple test case for the threads assignment.
//
//	Create two threads, and have them context switch
//	back and forth between themselves by calling Thread::Yield, 
//	to illustratethe inner workings of the thread system.
//
// Copyright (c) 1992-1993 The Regents of the University of California.
// All rights reserved.  See copyright.h for copyright notice and limitation 
// of liability and disclaimer of warranty provisions.

#include "copyright.h"
#include "system.h"

// testnum is set in main.cc
int testnum = 1;
char a[100];
//----------------------------------------------------------------------
// SimpleThread
// 	Loop 5 times, yielding the CPU to another ready thread 
//	each iteration.
//
//	"which" is simply a number identifying the thread, for debugging
//	purposes.
//----------------------------------------------------------------------

void
SimpleThread(int which)
{
    int num;
    
    for (num = 0; num < 5; num++) {
	printf("*** thread %d looped %d times\n", which, num);
        currentThread->Yield();
    }
}


//----------------------------------------------------------------------
// ThreadTest1
//  Set up a ping-pong between two threads, by forking a thread 
//  to call SimpleThread, and then calling SimpleThread ourselves.
//----------------------------------------------------------------------

void
ThreadTest1()
{
    DEBUG('t', "Entering ThreadTest1");

    Thread *t = new Thread("forked thread");

    t->Fork(SimpleThread, 1);
    SimpleThread(0);
}

//Roshan Piyush test lab8 program

void Vowels(int n)
{
    int va=0,ve=0,vi=0,vo=0,vu=0,i;
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='a' || a[i]=='A')
            va++;
    }
    printf("No of a --> %d\n ", va);
    currentThread->Yield();

    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='e' || a[i]=='E')
            ve++;
     }
    printf("No of e --> %d\n ", ve);
    currentThread->Yield();
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='i' || a[i]=='I')
            vi++;
     }
    printf("No of i --> %d\n ", vi);
    currentThread->Yield();
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='o' || a[i]=='O')
            vo++;
     }
    printf("No of o --> %d\n ", vo);
    currentThread->Yield();
    for (i=0;a[i]!='\0';i++) {
        if(a[i]=='u' || a[i]=='U')
            vu++;
    }
    printf("No of u --> %d\n ", vu);
    currentThread->Yield();
}

void Words(int n)
{
   int i=0;
   char words[100];
    for (i=0;a[i]!='\0';i++) {
        int j=0;
        while(a[i]!=' ' &&  a[i]!='\0')
        {
            words[j++]=a[i];
            i++;
        }
        words[j]='\0';

        printf("Word found --> %s\n", words);
        currentThread->Yield();
    }
}


void ThreadTest2()
{
    DEBUG('t', "Entering ThreadTest2");

    gets(a);
    Thread *t = new Thread("forked piyush");
    t->Fork(Vowels, 0);
    Words(0);
}

void TestProdCons()
{
    DEBUG('t', "Entering TestProdCons");

    gets(a);
    Thread *t = new Thread("forked piyush");
    t->Fork(Vowels, 0);
    Words(0);
}


//Roshan Piyush test programs end

//----------------------------------------------------------------------
// ThreadTest
// 	Invoke a test routine.
//----------------------------------------------------------------------

void
ThreadTest(int n)
{
    switch (n) {
    case 1:
	ThreadTest1();
    break;
    case 2:
    ThreadTest2();
	break;
    case 3:
    ThreadTest2();
    break;
    default:
	printf("No test specified.\n");
	break;
    }
}

