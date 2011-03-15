var Face = require('../build/default/face.node'),
	recognizer = new Face.init();

var input = '/tmp/face.png';

recognizer.run(input, function(faces){

	console.log("I found " + faces.length + " faces");
	for(var i = 0; i < faces.length; i++) {
			console.log(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
	}

});

