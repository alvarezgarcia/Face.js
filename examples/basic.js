var Face = require('../build/default/face.node'),
  detector = new Face.init();

detector.img = './samples/frame1.png';
detector.maxsize = 20;
detector.pathto = '../cascades/'

detector.oncomplete = function(faces){
  console.log("I found " + faces.length + " faces");
    for(var i = 0; i < faces.length; i++) {
    console.log(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
  }
};

detector.run();
