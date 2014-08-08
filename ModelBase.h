#ifndef __MODEL_BASE_H__
#define __MODEL_BASE_H__

#include <vector>
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
  typedef vector<PtrModel> ModelPtrs;
  typedef unique_ptr<ModelPtrs> PtrModelPtrs;

  friend ostream& operator<< ( ostream &os, ModelBase &m );

  protected:
  map<string, string> m_showData;
  map<string, PtrModelPtrs> m_subModels;
  PtrView m_pv;

  const string& GetView();

  ModelBase(const char* viewname);

  ModelBase() : m_pv(nullptr) { }

  string GetShowData(const string& key)
  {
    auto pos = m_showData.find(key);
    if(pos != m_showData.end())
      return m_showData[key];
    else
      return "";
  }

  ModelPtrs& operator[](string key) 
  {
    return *(m_subModels[key]);
  }

  public:

  void SetShowData(const string& key, const string& val)
  {
    m_showData[key] = val;
  }

  void Add(string key, ModelBase* subModel)
  {
    auto pos = m_subModels.find(key);
    if(pos == m_subModels.end())
    {
      ModelPtrs* pmodelPtrs = new ModelPtrs;
      pmodelPtrs->push_back(PtrModel(subModel));
      m_subModels.insert(make_pair(key, PtrModelPtrs(pmodelPtrs)));
    }
    else
    {
      pos->second->push_back(PtrModel(subModel));
    }
  }
};

#endif
