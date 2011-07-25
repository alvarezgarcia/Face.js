#include <v8.h>
#include <node.h>

#include <unistd.h>
#include <string.h>

#include "cv.h"
#include "highgui.h"

using namespace node;
using namespace v8;

class Face: ObjectWrap
{
private:
  unsigned int min_size;
  unsigned int max_size;

  char *filename;
  char *pathto;
  IplImage* frame;
  CvMemStorage* memory;
  CvSeq* total_faces;
  CvHaarClassifierCascade* cascade;

  Persistent<Function> oncomplete;

public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target);

  static Handle<Value> New(const Arguments& args);
  static Handle<Value> Run(const Arguments& args);

  Face()
  {
  }

  ~Face()
  {
  }

  struct face_baton_t {
    Face* fc;
    Persistent<Function> cb;
  };

  static int Analize(eio_req *req);
  static int AfterAnalize(eio_req *req);

  static Handle<Value> GetImg(Local<String> name,	const AccessorInfo& info);
  static void SetImg(Local<String> name, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> GetOnComplete(Local<String> name, const AccessorInfo& info);
  static void SetOnComplete(Local<String> name, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> GetMinSize(Local<String> name, const AccessorInfo& info);
  static void SetMinSize(Local<String> name, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> GetMaxSize(Local<String> name, const AccessorInfo& info);
  static void SetMaxSize(Local<String> name, Local<Value> value, const AccessorInfo& info);
  static Handle<Value> GetPathTo(Local<String> name, const AccessorInfo& info);
  static void SetPathTo(Local<String> name, Local<Value> value, const AccessorInfo& info);
};
