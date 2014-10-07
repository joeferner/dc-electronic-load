'use strict';

$(function() {
  function openWebSocket() {
    var wsAddr = 'ws://' + location.hostname + (location.port ? ':' + location.port : '') + '/ws';
    console.log('connecting ws: ' + wsAddr);
    var wsConnection = new WebSocket(wsAddr);

    wsConnection.onopen = function() {
      console.log('connected ws: ' + wsAddr);
    };

    wsConnection.onerror = function(error) {
      console.log('WebSocket Error ' + error);
    };

    wsConnection.onmessage = function(e) {
      var json = JSON.parse(e.data);
      console.log('Server:', json);
    };
  }

  openWebSocket();
});
