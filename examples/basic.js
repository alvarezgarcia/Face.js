var Face = require('../build/default/face.node'),
  detector = new Face.init();

detector.img = './samples/frame.jpg';
detector.maxsize = 20;
detector.pathto = '../cascades/haarcascade_frontalface_alt.xml'
console.log(detector.img);

detector.oncomplete = function(faces){
  console.log("I found " + faces.length + " faces");
    for(var i = 0; i < faces.length; i++) {
    console.log(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
  }
};

detector.run();
