#include <v8.h>
#include <node.h>

#include <unistd.h>
#include <string.h>

#include <cv.h>
#include <highgui.h>

#include "face_detection.h"

using namespace node;
using namespace v8;

class Face: ObjectWrap
{
private:
  unsigned int min_size;
  char *filename;
  IplImage* frame;
  CvMemStorage* memory;
  CvSeq* total_faces;
  CvHaarClassifierCascade* cascade;

public:

  static Persistent<FunctionTemplate> s_ct;
  static void Init(Handle<Object> target)
  {
    HandleScope scope;

    Local<FunctionTemplate> t = FunctionTemplate::New(New);

    s_ct = Persistent<FunctionTemplate>::New(t);
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("init"));

    NODE_SET_PROTOTYPE_METHOD(s_ct, "run", Run);

    target->Set(String::NewSymbol("init"), s_ct->GetFunction());

  }

  Face()
  {
  }

  ~Face()
  {
  }

  static Handle<Value> New(const Arguments& args)
  {
    HandleScope scope;
    Face* fc = new Face();
    fc->Wrap(args.This());

	fc->memory = cvCreateMemStorage(0);
	fc->cascade = (CvHaarClassifierCascade*)cvLoad("../cascades/haarcascade_frontalface_alt.xml", 0, 0, 0);

    return args.This();
  }

  struct face_baton_t {
    Face* fc;
    Persistent<Function> cb;
  };

  static Handle<Value> Run(const Arguments& args)
  {
    HandleScope scope;
	Face* fc = ObjectWrap::Unwrap<Face>(args.This());

	Local<Function> cb;
	face_baton_t *baton = new face_baton_t();

	if(!args[0]->IsString()) {
		return ThrowException(Exception::TypeError(String::New("Argument 0 must be a string")));
	}

	String::AsciiValue filename(args[0]);
	fc->filename = strdup(*filename);
	fc->frame = cvLoadImage(fc->filename, 3);

	if (!args[1]->IsInt32()) {
		fprintf(stderr, "No minsize provided, setting 20\n");
		fc->min_size = 20;
  	 	cb = Local<Function>::Cast(args[1]);
	} else {
		fc->min_size = args[1]->Uint32Value();
  	 	cb = Local<Function>::Cast(args[2]);
	}


    baton->fc = fc;
    baton->cb = Persistent<Function>::New(cb);

    fc->Ref();

    eio_custom(Analize, EIO_PRI_DEFAULT, AfterAnalize, baton);
    ev_ref(EV_DEFAULT_UC);

    return Undefined();
  }


  static int Analize(eio_req *req)
  {

    face_baton_t *baton = static_cast<face_baton_t *>(req->data);

	IplImage* gray_frame;
	gray_frame = nice_my_frame(baton->fc->frame);
	baton->fc->total_faces = detect_faces(gray_frame, baton->fc->cascade, baton->fc->memory, baton->fc->min_size);

	cvReleaseImage(&gray_frame);

    return 0;
  }

  static int AfterAnalize(eio_req *req)
  {
    HandleScope scope;
    face_baton_t *baton = static_cast<face_baton_t *>(req->data);
    ev_unref(EV_DEFAULT_UC);
    baton->fc->Unref();

	v8::Handle <v8::Array> arr = v8::Array::New();

    for(int i = 0; i < (baton->fc->total_faces ? baton->fc->total_faces->total : 0); i++ ) {

		CvRect* r = (CvRect*)cvGetSeqElem(baton->fc->total_faces, i );

		v8::Handle <v8::Object> obj = v8::Object::New();
		obj->Set(v8::String::New("x"), v8::Integer::New(r->x));
		obj->Set(v8::String::New("y"), v8::Integer::New(r->y));
		obj->Set(v8::String::New("width"), v8::Integer::New(r->width));
		obj->Set(v8::String::New("height"), v8::Integer::New(r->height));
		
		arr->Set(i, obj);

	}


	v8::Handle<v8::Value> args[1] = {arr};

    TryCatch try_catch;

    baton->cb->Call(Context::GetCurrent()->Global(), 1, args);

    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }

    baton->cb.Dispose();

    delete baton;
    return 0;
  }

};

Persistent<FunctionTemplate> Face::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    Face::Init(target);
  }

  NODE_MODULE(face, init);
}
