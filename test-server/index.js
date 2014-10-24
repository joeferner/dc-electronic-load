'use strict';

var WebSocketServer = require('ws').Server;
var http = require('http');
var express = require('express');
var path = require('path');
var bodyParser = require('body-parser');

var PORT = 10080;
var targetAmps = 0;

var app = express();
app.use(express.static(path.join(__dirname, '../flash-files')));
app.use(bodyParser.urlencoded({
  extended: true
}));
app.post('/amps/set', function(req, res, next) {
  console.log(req.body);
  targetAmps = parseInt(req.body.value);
  res.send('OK');
});

var server = http.createServer(app);
server.listen(PORT);
console.log('listening on http://localhost:' + PORT);

var wss = new WebSocketServer({server: server});
wss.on('connection', function(ws) {
  var id = setInterval(function() {
    var msg = {
      time: new Date().getTime(),
      voltage: 5000 + (Math.random() * 10),
      amperage: 200 + (Math.random() * 10),
      targetAmps: targetAmps
    };
    ws.send(JSON.stringify(msg), function() {
      // ignore errors
    });
  }, 1000);
  console.log('started client interval');
  ws.on('close', function() {
    console.log('stopping client interval');
    clearInterval(id);
  });
});
