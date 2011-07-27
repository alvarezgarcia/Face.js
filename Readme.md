
# Face.js

	by Tomas Alvarez (http://twitter.com/alvarez_tomas)
	Face.js is a OpenCV Haar algorithm implementation for Javascript, in-progress right now, which aims to provide a simple way 
	to detect faces in a picture.

## Installation

	git clone git://github.com/talvarez/Face.js.git
	node-waf configure build

Just check that you have installed the latest OpenCV stable tarball (currently 2.2).

## Basic example

	var Face = require('../build/default/face.node'),
	detector = new Face.init();

	detector.img = '/tmp/last.png';
	console.log(detector.img);

	detector.oncomplete = function(faces){

		console.log("I found " + faces.length + " faces");
		for(var i = 0; i < faces.length; i++) {
				console.log(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
		}

	};

	detector.run();



## Complete example

	var Face = require('../build/default/face.node'),
	recognizer = new Face.init(),
	Canvas = require('../deps/canvas/canvas.js'),
	Image = Canvas.Image,
	canvas = new Canvas(800, 600),
	ctx = canvas.getContext('2d');


	function boundFaces(input, faces, res) {
		var img = new Image;

		img.onload = function(){
				ctx.drawImage(img, 0, 0);
		}


		var threshold_false_smile = 15;
		img.src = input;

		for(var i = 0; i < faces.length; i++) {

				res.write('Intensity: '+faces[i].intensity+'<BR>');

				if(faces[i].smile) {
						if(faces[i].intensity > threshold_false_smile) { //Threshold possible false positives smiles
								ctx.strokeStyle = 'rgb(0, 255, 0)';
						} else {
								ctx.strokeStyle = 'rgb(255, 0, 0)';
						}
				} else {

						ctx.strokeStyle = 'rgb(255, 0, 0)';
				}

				ctx.strokeRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
		}
	}


	function getFaces(res) {

		recognizer.img = './samples/frame5.png';
		recognizer.pathto = '../cascades/';

		recognizer.checkSmile = true;
		recognizer.minsize = 20;

		recognizer.oncomplete = function(faces){

				boundFaces(recognizer.img, faces, res);
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


	Setting 'checkSmile' true, it will scan the previously detected face and search a possible smile gesture.
	The faces object now includes a property, called 'smile' in case it's true another new property will be added to the object, called 'intensity'.
	'intensity' property is used for control possible false positives smiles, maybe the detector detect a smile, but its wrong (this is common in computer
	vision programs, because the detectors handles a lot of variables like, illumination, pose, size) so you can threshold it.
	You can set a minimum, and a maximum too, size for a face, if you dont provide it, Face.js will set it to 20 for the min, and 0 for the max (0 means all the sizes in the maxsize property).

	NOTE: It uses node-canvas, but is not necessary, I used it (and I include it in deps) just for draw rectangles in the faces areas.



## Versions

Tested with:

	- node 0.4.8
	- opencv 2.2.0

## License

(The MIT License)

Copyright (c) 2011 talvarez

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
