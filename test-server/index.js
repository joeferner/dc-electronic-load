'use strict';

var WebSocketServer = require('ws').Server;
var http = require('http');
var express = require('express');
var path = require('path');
var bodyParser = require('body-parser');
var csv = require('csv-stringify');

var PORT = 10080;
var targetAmps = 0;
var recording = false;
var recordingInterval = null;
var data = [];

var app = express();
app.use(bodyParser.urlencoded({
  extended: true
}));

app.post('/amps/set', function(req, res, next) {
  console.log(req.body);
  targetAmps = parseInt(req.body.value);
  res.send('OK');
});

app.post('/recorder/start', function(req, res, next) {
  console.log(req.body);
  var rate = req.body.rate || 1000;

  if (recordingInterval) {
    clearInterval(recordingInterval);
  }

  if (recording) {
    res.send(500);
  } else {
    console.log('starting recording: ' + rate);
    data = [];
    recordingInterval = setInterval(function() {
      var msg = {
        time: new Date().getTime(),
        voltage: parseInt(5000 + (Math.random() * 10)),
        amperage: parseInt(200 + (Math.random() * 10)),
        targetAmps: parseInt(targetAmps)
      };
      data.push(msg);
    }, rate);
    recording = true;
    res.send('OK');
  }
});

app.post('/recorder/stop', function(req, res, next) {
  if (recordingInterval) {
    clearInterval(recordingInterval);
  }

  if (!recording) {
    res.send(500);
  } else {
    recording = false;
    res.send('OK');
  }
});

app.get('/recorder/download', function(req, res, next) {
  var result = [];

  res.setHeader('Content-disposition', 'attachment; filename=dc-electronic-load-data.csv');
  res.contentType('text/csv');
  res.write("Time,Volts (mV),Amps (mA),Set Amps (mA)\n");
  csv(data, function(err, data) {
    res.end(data);
  });
});

app.use(express.static(path.join(__dirname, '../flash-files')));

var server = http.createServer(app);
server.listen(PORT);
console.log('listening on http://localhost:' + PORT);

var wss = new WebSocketServer({server: server});
wss.on('connection', function(ws) {
  var id = setInterval(function() {
    var msg = {
      time: new Date().getTime(),
      voltage: parseInt(5000 + (Math.random() * 10)),
      amperage: parseInt(200 + (Math.random() * 10)),
      targetAmps: parseInt(targetAmps),
      recordingSamples: data.length,
      recording: recording
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
  ws.on('message', function() {
    console.log(arguments);
  });
});
