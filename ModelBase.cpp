#include "ModelBase.h"
#include "ViewBase.h"

ModelBase::ModelBase(const char* viewname)
{
  if(viewname) m_pv = move(PtrView(new ViewBase(viewname)));
}

const string& ModelBase::GetView()
{
  return m_pv->m_src;
}

ostream& operator<< (ostream& os, ModelBase& t)
{
  if(t.m_pv)
  {
    regex reg("<%\\s*(.*?)\\s*%>");
    auto pos=t.GetView().cbegin();
    auto end=t.GetView().cend();
    smatch m;

    for (; regex_search(pos,end,m,reg); pos=m.suffix().first)
    {
      // replace the place holder to the value saved in the model.
      auto pos2 = t.m_subModels.find(m.str(1));
      if(t.GetShowData(m.str(1)) != "")
      {
        os << m.prefix().str() << t.GetShowData(m.str(1));
      }
      else if(pos2 != t.m_subModels.end())
      {
        os << m.prefix().str() << *(t.m_subModels[m.str(1)]);
      }
      else
      {
        os << m.prefix().str();
      }
    }

    os << t.GetView().substr(pos - t.GetView().cbegin()) << endl;
  }
  else
  {
    // output the model data in raw (json) mode
    os << "{";
    for(auto showdata : t.m_showData)
    {
      os << showdata.first << ": \"" << showdata.second << "\",";
    }
    for(auto &sub : t.m_subModels)
    {
      os << sub.first << ": " << *sub.second << ",";
    }
    os << "}";
  }
  
  return os;
}
