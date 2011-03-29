IplImage* nice_my_frame(IplImage* frame);
CvSeq* detect_faces(IplImage* frame, CvHaarClassifierCascade* cascade, CvMemStorage* memory, int min_size, int max_size);

