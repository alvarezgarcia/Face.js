#include <v8.h>
#include <node.h>

#include <unistd.h>
#include <string.h>

#include "main.h"


using namespace node;
using namespace v8;

  Handle<Value> Face::GetImg(Local<String> name,  const AccessorInfo& info) {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    return String::New(fc->filename);
  }
  void Face::SetImg(Local<String> name, Local<Value> value, const AccessorInfo& info) {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    String::AsciiValue filename(value);
    fc->filename = strdup(*filename);
  }


  Handle<Value> Face::GetOnComplete(Local<String> name, const AccessorInfo& info) 
  {
  }

  void Face::SetOnComplete(Local<String> name, Local<Value> value, const AccessorInfo& info) {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    fc->oncomplete = Persistent<Function>::New(Handle<Function>::Cast(value));
  }

  Handle<Value> Face::GetMinSize(Local<String> name, const AccessorInfo& info) 
  {
  }

  void Face::SetMinSize(Local<String> name, Local<Value> value, const AccessorInfo& info) {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    fc->min_size = value->Int32Value();
  }

  Handle<Value> Face::GetMaxSize(Local<String> name, const AccessorInfo& info)
  {
  }

  void Face::SetMaxSize(Local<String> name, Local<Value> value, const AccessorInfo& info)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    fc->max_size = value->Int32Value();
  }

  Handle<Value> Face::GetPathTo(Local<String> name, const AccessorInfo& info)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    return String::New(fc->pathto);
  }

  void Face::SetPathTo(Local<String> name, Local<Value> value, const AccessorInfo& info)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    String::AsciiValue ascii(value);
    fc->pathto = strdup(*ascii);
  }

  Handle<Value> Face::GetChkSmile(Local<String> name, const AccessorInfo& info)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
    return Boolean::New(fc->checkSmile);
  }

  void Face::SetChkSmile(Local<String> name, Local<Value> value, const AccessorInfo& info)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(info.This());
	fc->checkSmile = value->BooleanValue();
  }
