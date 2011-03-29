var Face = require('../build/default/face.node'),
	recognizer = new Face.init(),
	Canvas = require('../deps/canvas/canvas.js'),
	Image = Canvas.Image,
	canvas = new Canvas(800, 600),
	ctx = canvas.getContext('2d');


function boundFaces(input, faces) {
	var img = new Image;

	img.onload = function(){
		ctx.drawImage(img, 0, 0);
	}

	img.src = input;

	ctx.strokeStyle = 'rgb(255, 0, 0)';
	for(var i = 0; i < faces.length; i++) {
		ctx.strokeRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
	}
}


function getFaces(res) {
	recognizer.img = '/home/sebastian/mtv.png';

	recognizer.oncomplete = function(faces){

		boundFaces(recognizer.img, faces);
		res.write("I found " + faces.length + " faces <BR>");
		res.end('<img src="' + canvas.toDataURL() + '" />');

	};

	recognizer.run();
	
}

var http = require('http');
http.createServer(function (req, res) {
				res.writeHead(200, { 'Content-Type': 'text/html' });
				res.write("Getting faces...<BR>");
				getFaces(res);
				
}).listen(3000);

console.log('Server started on port 3000');
