#ifndef __MODEL_BASE_H__
#define __MODEL_BASE_H__

#include <map>
#include <regex>
#include <string>
#include <memory>

using namespace std;

class ViewBase;

class ModelBase
{
  typedef unique_ptr<ViewBase> PtrView;
  typedef unique_ptr<ModelBase> PtrModel;

  friend ostream& operator<< ( ostream &os, ModelBase &m );

  protected:
  map<string, string> m_showData;
  map<string, PtrModel> m_subModels;
  PtrView m_pv;

  const string& GetView();

  public:
  ModelBase(const char* viewname);

  ModelBase() : m_pv(nullptr) { }

  void SetShowData(const string& key, const string& val)
  {
    m_showData[key] = val;
  }

  string GetShowData(const string& key)
  {
    auto pos = m_showData.find(key);
    if(pos != m_showData.end())
      return m_showData[key];
    else
      return "";
  }

  ModelBase& operator[](string key) 
  {
    return *(m_subModels[key]);
  }

  void Add(string key, ModelBase* subModel)
  {
    m_subModels[key] = PtrModel(subModel);
  }
};

#endif
