#include <iostream>
#include <thread>
#include <memory>

#include "ViewBase.h"
#include "ModelBase.h"

using namespace std;

mutex m;

class SayHello{
public:
  void hello(const string& str) const
  {
    cout << "hello, welcome to the c++11 world. It is a very powerful language and has many amazing features." << endl << str << endl;
  }
};

int main()
{
  /*
  ModelBase m1("view1");
  m1.SetShowData("name", "view1 name");
  m1.SetShowData("age", "view1 age");

  m1.Add("sub_test1", new ModelBase("view11"));
  m1["sub_test1"].SetShowData("hellomsg", "view11 hello");

  m1.Add("sub_test2", new ModelBase("view11"));
  m1["sub_test2"].SetShowData("hellomsg", "view12 hello");

  ModelBase *m111 = new ModelBase("view111");
  m111->SetShowData("name", "view111 name");
  m111->SetShowData("dep", "view111 dep");

  ModelBase *m112 = new ModelBase("view111");
  m112->SetShowData("name", "view112 name");
  m112->SetShowData("dep", "view112 dep");

  ModelBase *m113 = new ModelBase("view111");
  m113->SetShowData("name", "view113 name");
  m113->SetShowData("dep", "view113 dep");

  ModelBase *m114 = new ModelBase("view111");
  m114->SetShowData("name", "view114 name");
  m114->SetShowData("dep", "view114 dep");

  m1["sub_test1"].Add("sub_test1", m111);
  m1["sub_test1"].Add("sub_test2", m112);
  m1["sub_test2"].Add("sub_test1", m113);
  m1["sub_test2"].Add("sub_test2", m114);

  cout << m1;
  */

  unique_ptr<SayHello> px(new SayHello());
  string str = "Have nothing to say about c++11.";
  thread worker = thread(&SayHello::hello, move(px), std::ref(str));
  worker.join();
}
