#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <xlnt/xlnt.hpp>

#include "../ControllerBase.h"
#include "../models/ManageModel.h"
#include "../models/UploadModel.h"
#include "../ControllerFactoryBase.h"


class ManageController : public ControllerBase
{
  private:
  bool isExcel(const string& fileName){

    if (fileName.size() <= 5) return false;

    string ext = fileName.substr(fileName.size() - 5);
    for(char & c : ext){
      c = static_cast<char>(tolower(c));
    }
    return ext == ".xlsx";
  }

  public:
  ManageController() : ControllerBase("manage") { }

  void list(const Request &request)
  {
    Show(new ManageModel(ControllerFactoryBase::s_ctrltbl));
  }

  void listraw(const Request &request)
  {
    Show(new ManageModel(ControllerFactoryBase::s_ctrltbl, nullptr));
  }

  void upload(const Request &request) {
    if(request.requestMethod != "POST"){
      Show(new UploadModel("uploadexcel"));
      return;
    }

    auto f = request.GetFile("excelfile");
    auto name = f->getName();
    FILE * upf = f->getFile();
    if(!isExcel(name)){
      std::stringstream ss;
      ss << "Error: Uploaded file is not an excel(xlsx) file." << name << std::endl;
      ModelBase * pm = new UploadModel("uploadexcel");
      pm->SetShowData("upresult", ss.str());
      Show(pm);
      return;
    }

    std::vector<std::uint8_t> data;
    for(unsigned char buf[4096]; auto size=fread(buf, 1, 4096, upf);){
      data.insert(data.end(), &buf[0], &buf[size]);
    }

    // read the contents from the excel file.
    xlnt::workbook wb;
    wb.load(data);
    auto ws = wb.active_sheet();
    std::stringstream ss;
    ss << "Excel file is uploaded. " << std::endl;
    for(auto row : ws.rows(false)){
      for(auto cell : row){
        ss << cell.to_string() << '\t';
      }
      ss << std::endl;
    }
    
    ModelBase * pm = new UploadModel("uploadexcel");
    pm->SetShowData("upresult", ss.str());
    Show(pm);
  }
};
