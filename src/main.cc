#include <v8.h>
#include <node.h>

#include <unistd.h>
#include <string.h>

#include "cv.h"
#include "highgui.h"

#include "main.h"
#include "face_detection.h"

using namespace node;
using namespace v8;

  void Face::Init(Handle<Object> target)
  {
    HandleScope scope;

    s_ct = Persistent<FunctionTemplate>::New(FunctionTemplate::New(New));
    s_ct->InstanceTemplate()->SetInternalFieldCount(1);
    s_ct->SetClassName(String::NewSymbol("init"));

    Local<ObjectTemplate> proto = s_ct->PrototypeTemplate();
    proto->SetAccessor(String::NewSymbol("img"), GetImg, SetImg);
    proto->SetAccessor(String::NewSymbol("oncomplete"), GetOnComplete, SetOnComplete);
    proto->SetAccessor(String::NewSymbol("minsize"), GetMinSize, SetMinSize);
    proto->SetAccessor(String::NewSymbol("maxsize"), GetMaxSize, SetMaxSize);
    proto->SetAccessor(String::NewSymbol("pathto"), GetPathTo, SetPathTo);
    proto->SetAccessor(String::NewSymbol("checkSmile"), GetChkSmile, SetChkSmile);

    NODE_SET_PROTOTYPE_METHOD(s_ct, "run", Run);
    target->Set(String::NewSymbol("init"), s_ct->GetFunction());
  }

  Handle<Value> Face::New(const Arguments& args)
  {
    HandleScope scope;
    Face* fc = new Face();
    fc->Wrap(args.This());

    fc->memory = cvCreateMemStorage(0);
    fc->min_size = 20;
    fc->max_size = 0;
	fc->checkSmile = false;

    return args.This();
  }

  Handle<Value> Face::Run(const Arguments& args)
  {
    Face* fc = ObjectWrap::Unwrap<Face>(args.This());

	//TODO: Add checks of XML file, if it doesnt exist it dies
	char xmlCascade[255];
	sprintf(xmlCascade, "%shaarcascade_frontalface_alt2.xml", fc->pathto);

    fc->cascade = (CvHaarClassifierCascade*)cvLoad(xmlCascade, 0, 0, 0);
    fc->frame = cvLoadImage(fc->filename, 3);
    face_baton_t *baton = new face_baton_t();
    baton->fc = fc;
    fc->Ref();
    eio_custom(Analize, EIO_PRI_DEFAULT, AfterAnalize, baton);
    ev_ref(EV_DEFAULT_UC);
    return Undefined();
  }

  int Face::Analize(eio_req *req)
  {
    face_baton_t *baton = static_cast<face_baton_t *>(req->data);
    IplImage* gray_frame;
    gray_frame = nice_my_frame(baton->fc->frame);
    baton->fc->total_faces = detect_faces(gray_frame, baton->fc->cascade, baton->fc->memory, baton->fc->min_size, baton->fc->max_size);
	baton->fc->gray = gray_frame;

    //cvReleaseImage(&gray_frame);
    return 0;
  }

  int Face::AfterAnalize(eio_req *req)
  {
    HandleScope scope;
    face_baton_t *baton = static_cast<face_baton_t *>(req->data);
    ev_unref(EV_DEFAULT_UC);
    baton->fc->Unref();

	char xmlSmile[255];
	sprintf(xmlSmile, "%s/smiled_01.xml", baton->fc->pathto);

	CvHaarClassifierCascade* cascade_smile =  (CvHaarClassifierCascade*)cvLoad(xmlSmile, 0, 0, 0);

    v8::Handle <v8::Array> arr = v8::Array::New();

    for(int i = 0; i < (baton->fc->total_faces ? baton->fc->total_faces->total : 0); i++ ) {
      CvRect* r = (CvRect*)cvGetSeqElem(baton->fc->total_faces, i );
      v8::Handle <v8::Object> obj = v8::Object::New();
      obj->Set(v8::String::New("x"), v8::Integer::New(r->x));
      obj->Set(v8::String::New("y"), v8::Integer::New(r->y));
      obj->Set(v8::String::New("width"), v8::Integer::New(r->width));
      obj->Set(v8::String::New("height"), v8::Integer::New(r->height));


	  if(baton->fc->checkSmile == true) {
		cvSetImageROI(baton->fc->gray, cvRect(r->x+(r->width*1/7),r->y+(r->height*2/3),(int)r->width*6/7,(int)r->height/3));
		CvSeq* smile = cvHaarDetectObjects(baton->fc->gray, cascade_smile, baton->fc->memory, 1.1, 2, CV_HAAR_DO_CANNY_PRUNING, cvSize(22, 10), cvSize(0, 0));


		if(smile->total > 0) {
			CvAvgComp* comp = (CvAvgComp*)cvGetSeqElem(smile, 0);
      		CvRect* r = (CvRect*)cvGetSeqElem(baton->fc->total_faces, i );

			obj->Set(v8::String::New("smile"), v8::Boolean::New(true));
			obj->Set(v8::String::New("intensity"), v8::Integer::New(comp->neighbors));

		} else {
			obj->Set(v8::String::New("smile"), v8::Boolean::New(false));
		}

		cvResetImageROI(baton->fc->gray);
	  }

      arr->Set(i, obj);
    }

	cvClearMemStorage(baton->fc->memory);

    v8::Handle<v8::Value> args[1] = {arr};
    TryCatch try_catch;
    baton->fc->oncomplete->Call(Context::GetCurrent()->Global(), 1, args);
    if (try_catch.HasCaught()) {
      FatalException(try_catch);
    }
    baton->cb.Dispose();
    delete baton;
    return 0;
  }

Persistent<FunctionTemplate> Face::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
    Face::Init(target);
  }

  NODE_MODULE(face, init);
}
