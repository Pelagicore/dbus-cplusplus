#ifndef TEST_APP_INTRO_PROVIDER_H
#define TEST_APP_INTRO_PROVIDER_H

#include "TestAppIntroProviderPrivate.h"

#include "TestAppIntro.h"

#include <iostream>

class TestAppIntroProvider : 
  public DBusCpp::Test::Com::Intro_adaptor,
  public DBus::IntrospectableAdaptor,
  public DBus::ObjectAdaptor
{
public:
  TestAppIntroProvider (DBus::Connection& connection, TestAppIntro *testComIntro) :
    DBus::ObjectAdaptor(connection, "/DBusCpp/Test/Com/Intro"),
    mTestAppIntro (testComIntro)
  {}

  void test1 ()
  {
    std::cout << "Test1" << std::endl;
    mTestAppIntro->test1Result ();
  }

private:
  TestAppIntro *mTestAppIntro;
};

#endif // TEST_COM_INTRO_PROVIDER_H

