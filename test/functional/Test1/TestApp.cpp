#ifdef HAVE_CONFIG_H
#include <config.h>
#endif

// STD
#include <cstdio>

// local
#include "TestApp.h"
#include "TestAppIntro.h"

using namespace std;

DBus::BusDispatcher dispatcher;
TestAppIntro *g_testComIntro;
DBus::Pipe *mTestToDBusPipe;
bool testResult = false;

pthread_mutex_t clientMutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t clientCondition = PTHREAD_COND_INITIALIZER;

TestApp::TestApp ()
{
  cout << "initialize DBus..." << endl;
  initDBus ();
}

void TestApp::initDBus ()
{
  DBus::_init_threading();

  DBus::default_dispatcher = &dispatcher;

  new DBus::DefaultTimeout(100, false, &dispatcher);

  DBus::Connection conn = DBus::Connection::SessionBus();

  TestAppIntro testComIntro (conn, clientCondition, testResult);
  g_testComIntro = &testComIntro;

  cout << "Start server..." << endl;
  TestAppIntroProvider testComProviderIntro (conn, &testComIntro);
  conn.request_name ("DBusCpp.Test.Com.Intro");
  
  mTestToDBusPipe = dispatcher.add_pipe (TestApp::testHandler, NULL);

  cout << "Start client thread..." << endl;
  pthread_create (&testThread, NULL, TestApp::testThreadRunner, &conn);

  dispatcher.enter();

  pthread_join (testThread, NULL);

  cout << "Testresult = " << string (testResult ? "OK" : "NOK") << endl;
}

void *TestApp::testThreadRunner (void *arg)
{
  char idstr[16];

  snprintf (idstr, sizeof(idstr), "%lu", pthread_self());

  mTestToDBusPipe->write (idstr, strlen (idstr) + 1);

  struct timespec abstime;
  
  clock_gettime(CLOCK_REALTIME, &abstime);
  abstime.tv_sec += 1;
  
  pthread_mutex_lock (&clientMutex);
  if (pthread_cond_timedwait (&clientCondition, &clientMutex, &abstime) == ETIMEDOUT)
  {
    cout << "client timeout!" << endl;
    testResult = false;
  }  
  pthread_mutex_unlock (&clientMutex);

  cout << "leave!" << endl;
  dispatcher.leave ();
  
  return NULL;
}

void TestApp::testHandler (const void *data, void *buffer, unsigned int nbyte)
{
  char *str = (char*) buffer;
  cout << "buffer1: " << str << ", size: " << nbyte << endl;
  
  cout << "run it!" << endl;
  g_testComIntro->test1 ();
}
