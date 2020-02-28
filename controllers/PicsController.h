#include <dirent.h>
#include <vector>
#include <algorithm>
#include <string>
#include <jpeglib.h>
#include <libexif/exif-data.h>
#include <sys/stat.h>

#include "../ControllerBase.h"
#include "../models/PicsListModel.h"
#include "../UploadFile.h"
#include "../models/UploadModel.h"

using namespace std;
class PicsController : public ControllerBase
{
private:

  unique_ptr<string> getDate(const char *injpg) {
  
    unique_ptr<ExifData, void(*)(ExifData*)> d(exif_data_new_from_file(injpg),
                                               [](ExifData *p) {
                                                 if (p) exif_data_unref(p);
                                               });
    unique_ptr<string> buf(new string);
    if (!d) {
      *buf = "Uknown";
      return buf;
    }
  
    unique_ptr<ExifEntry, void(*)(ExifEntry*)> e(exif_data_get_entry(d, EXIF_TAG_DATE_TIME_ORIGINAL),
                                                 [](ExifEntry *p) {
                                                   if (p) exif_entry_unref(p);
                                                 });
    if(!e){
      *buf = "Uknown";
      return buf;
    }
  
    buf->append((char*)(e->data), 10);
    (*buf)[4] = '-';
    (*buf)[7] = '-';
    return buf;
  }

  int thumbnail(const char *injpg, const char *outjpg, int ratio){
    
    unique_ptr<FILE, void(*)(FILE*)> inf(fopen(injpg, "rb"),
                                         [](FILE *p) {
                                           if(p) fclose(p);
                                         });
    if(!inf){
      return 0;
    }
  
    unique_ptr<FILE, void(*)(FILE*)> outf(fopen(outjpg, "wb+"),
                                         [](FILE *p) {
                                           if(p) fclose(p);
                                         });
    if(!outf){
      return 0;
    }
  
    struct jpeg_decompress_struct cinfo;
    struct jpeg_compress_struct ocinfo;
    struct jpeg_error_mgr jerr;
    struct jpeg_error_mgr ojerr;
    
    
    cinfo.err = jpeg_std_error(&jerr);
    ocinfo.err = jpeg_std_error(&ojerr);
    jpeg_create_decompress(&cinfo);
    jpeg_create_compress(&ocinfo);
    
    jpeg_stdio_src(&cinfo, inf.get());
    jpeg_stdio_dest(&ocinfo, outf.get());
  
    jpeg_read_header(&cinfo, true);
    
    cinfo.scale_num = 1;
    cinfo.scale_denom = ratio;
  
    jpeg_calc_output_dimensions(&cinfo);
    
    ocinfo.image_width = cinfo.output_width;
    ocinfo.image_height = cinfo.output_height;
    ocinfo.input_components = cinfo.output_components;
    ocinfo.in_color_space = cinfo.out_color_space;
    jpeg_set_defaults(&ocinfo);
  
    unique_ptr<unsigned char[]> buf(new unsigned char[cinfo.output_width * cinfo.output_components]);
    unsigned char* bufaddr = buf.get();
  
    jpeg_start_decompress(&cinfo);
    jpeg_start_compress(&ocinfo, true);
    while (cinfo.output_scanline < cinfo.output_height) {
      jpeg_read_scanlines(&cinfo, &bufaddr, 1);
      jpeg_write_scanlines(&ocinfo, &bufaddr, 1);
    }
  
    jpeg_finish_decompress(&cinfo);
    jpeg_destroy_decompress(&cinfo);
  
    jpeg_finish_compress(&ocinfo);
    jpeg_destroy_compress(&ocinfo);
  
    return 1;
  }

  bool isJpeg(const string& fileName){

    if (fileName.size() <= 4) return false;

    string ext = fileName.substr(fileName.size() - 4);
    for(char & c : ext){
      c = static_cast<char>(tolower(c));
    }
    return ext == ".jpg";
  }

public:

  PicsController() : ControllerBase("pics") { }

  void login(const Request &request){
  }

  void list(const Request &request)
  {
    string folder = request.GetParam("f");
    DIR *d;
    struct dirent *dir;
    d = opendir(("../www/static/pics/" + folder).c_str());
    vector<string> files;
    if(d){
      while ((dir = readdir(d)) != NULL){
        if(isJpeg(dir->d_name) && (dir->d_type == DT_REG || dir->d_type == DT_LNK)){
          files.push_back(dir->d_name);
        }
      }
    }
    
    Show(new PicsListModel(folder.c_str(), files));
  }

  void upload(const Request &request) {
    if(request.requestMethod != "POST"){
      Show(new UploadModel());
      return;
    }

    auto f = request.GetFile("picfile");
    auto name = f->getName();
    FILE * upf = f->getFile();
    if(!isJpeg(name)){
      Show(new UploadModel());
      return;
    }

    string savPath = "../www/static/pics/" + name;

    unique_ptr<FILE, void(*)(FILE*)> outf(fopen(savPath.c_str(), "wb+"),
                                         [](FILE *p) {
                                           if(p) fclose(p);
                                         });
    if(!outf){
      Show(new UploadModel());
      return;
    }

    for(char buf[4096]; auto size=fread(buf, 1, 4096, upf);){
      fwrite(buf, 1, size, outf.get());
    }

    auto date = getDate(savPath.c_str());

    string savDir = "../www/static/pics/test_" + *date;
    errno = 0;
    int re = mkdir(savDir.c_str(), 0755);
    if(re == 0 || errno == EEXIST){
      re = mkdir((savDir + "/s").c_str(), 0755);
      if(re == 0 || errno == EEXIST){
        re = mkdir((savDir + "/l").c_str(), 0755);
        if(re == 0 || errno == EEXIST){
          thumbnail(savPath.c_str(), (savDir + "/s/" + name).c_str(), 8);
          thumbnail(savPath.c_str(), (savDir + "/l/" + name).c_str(), 4);
          rename(savPath.c_str(), (savDir + "/" + name).c_str());
        }
      }
    }
    
    ModelBase * pm = new UploadModel();
    pm->SetShowData("upresult", "Picture is uploaded.");
    Show(pm);
  }

  void detail(const Request &request) {
  }
};

