'use strict';

var WebSocketServer = require('ws').Server;
var http = require('http');
var express = require('express');
var path = require('path');

var PORT = 10080;

var app = express();
app.use(express.static(path.join(__dirname, '../flash-files')));

var server = http.createServer(app);
server.listen(PORT);
console.log('listening on http://localhost:' + PORT);

var wss = new WebSocketServer({server: server});
wss.on('connection', function(ws) {
  var id = setInterval(function() {
    var msg = {
      time: new Date().getTime(),
      voltage: 5 + Math.random(),
      amperage: 4 +  + Math.random()
    };
    ws.send(JSON.stringify(msg), function() { /* ignore errors */ });
  }, 1000);
  console.log('started client interval');
  ws.on('close', function() {
    console.log('stopping client interval');
    clearInterval(id);
  });
});
