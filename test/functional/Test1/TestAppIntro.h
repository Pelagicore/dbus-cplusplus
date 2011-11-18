#ifndef TEST_APP_INTRO_H
#define TEST_APP_INTRO_H

#include "TestAppIntroPrivate.h"

#include <iostream>

class TestAppIntro :
  public DBusCpp::Test::Com::Intro_proxy,
  public DBus::IntrospectableProxy,
  public DBus::ObjectProxy
{
public:
  TestAppIntro (DBus::Connection& connection, pthread_cond_t &condition, bool &testResult) :
    DBus::ObjectProxy (connection, "/DBusCpp/Test/Com/Intro", "DBusCpp.Test.Com.Intro"),
    mCondition (condition),
    mTestResult (testResult)
  {}

  void test1Result ()
  {
    std::cout << "Test1Result" << std::endl;
    mTestResult = true;
    pthread_cond_signal (&mCondition);
  }

private:
  pthread_cond_t &mCondition;
  bool &mTestResult;
};

#endif // TEST_COM_INTRO_H
