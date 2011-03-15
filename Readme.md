
# Face.js

	by Tomas Alvarez (http://twitter.com/alvarez_tomas)
	Face.js is a OpenCV Haar algorithm implementation for Javascript, in-progress right now, which aims to provide a simple way 
	to detect faces in a picture.

## Installation

	git clone git://github.com/talvarez/Face.js.git
	node-waf configure build

Just check that you have installed the latest OpenCV stable tarball (currently 2.2).

## Basic example

	You simply give it a picture, you must check if it exists before , and use the run method then, it will asynchronously computate the image and try to find faces, when it is done it transfers an array of objects (an array of faces), each one with four properties, x, y, width and height.
	The basic idea is this:

	var Face = require('../build/default/face.node'),
    		recognizer = new Face.init();

	var input = '/home/sebastian/last.jpg';

	recognizer.run(input, function(faces){

		            console.log(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
		}

	});

	You can set a minimum size for a face, and give it to run method (after the string with the image), if you dont provide it, 
	Face.js will set it to 20 (it's a good value).

	NOTE: It uses node-canvas, but is not necessary, I used it (and I include it in deps) just for draw rectangles in the faces areas, but you can use the values that run method returns for each face (x, y, wdith, height) in another way.


## Complete example

	var Face = require('../build/default/face.node'),
			recognizer = new Face.init();
			Canvas = require('../deps/canvas/canvas.js'),
	   		Image = Canvas.Image,
	   		canvas = new Canvas(800, 600),
	   		ctx = canvas.getContext('2d');


	function getFaces(res) {
			var input = 'faces.jpg';

			res.write("Getting faces...<BR>");
			recognizer.run(input, function(faces){

						var img = new Image;

						img.onload = function(){
						ctx.drawImage(img, 0, 0);
						}

						img.src = input;

						ctx.strokeStyle = 'rgb(255, 0, 0)';
						for(var i = 0; i < faces.length; i++) {
						ctx.strokeRect(faces[i].x, faces[i].y, faces[i].width, faces[i].height);
						}

						res.write("I found " + faces.length + " faces <BR>");
						res.end('<img src="' + canvas.toDataURL() + '" />');
						});
	}

	var http = require('http');
	http.createServer(function (req, res) {
				res.writeHead(200, { 'Content-Type': 'text/html' });
				getFaces(res);

	}).listen(3000);

	console.log('Server started on port 3000')


## Versions

Tested with:

	- node 0.4.2
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
