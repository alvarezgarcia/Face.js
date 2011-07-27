var Face = require('../build/default/face.node'),
  recognizer = new Face.init();

function getFaces(res) {
  recognizer.img = './samples/frame1.png';
  recognizer.pathto = '../cascades/';
  recognizer.oncomplete = function(faces){
    res.end("I found " + faces.length + " faces <BR>");
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
