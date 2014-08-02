#include <iostream>

using namespace std;

class B
{
  public:
    void CmnFunc()
    {
      cout << "Common Func is called" << endl;
    }

    void BSpFunc()
    {
      cout << "B class specific func is called" << endl;
    }
};

class D : public B
{
  private:
    int x = 0;
  public:
    void DSpFunc()
    {
      ++x;
      cout << "D class specific func is called. x=" << x << endl;
    }
};

typedef void (B::* BFuncPtr)();

int main()
{
  D d;
  B* b = &d;

  BFuncPtr cmnFunc = &B::CmnFunc;
  BFuncPtr bspFunc = &B::BSpFunc;
  BFuncPtr dspFunc = static_cast<BFuncPtr>(&D::DSpFunc);

  (b->*cmnFunc)();
  (b->*bspFunc)();
  (b->*dspFunc)();
}
