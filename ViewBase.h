#ifndef __VIEW_BASE_H__
#define __VIEW_BASE_H__

#include <fstream>
#include <string>

using namespace std;

class ModelBase;

class ViewBase
{
  friend class ModelBase;

  private:
    string m_src;

  public:
    ViewBase(string viewName)
    {
      Load(viewName);
    }

    void Load(string viewName)
    {
      string viewPath = "views/" + viewName + ".html";
      // read the view file into source
      ifstream infile;
      try
      {
        infile.open(viewPath);
        infile.seekg(0, ios::end);
        m_src.reserve(infile.tellg());
        infile.seekg(0, ios::beg);
        
        m_src.assign(istreambuf_iterator<char>(infile), istreambuf_iterator<char>());
        infile.close();
      }
      catch (...)
      {
        m_src = "The view file can't be loaded. " + viewName;
        if(infile.is_open())
        {
          infile.close();
        }
      }
    }
};

#endif
