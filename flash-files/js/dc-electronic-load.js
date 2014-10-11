'use strict';

$(function() {
  var data = {
    voltage: [],
    amperage: [],
    power: []
  };

  function updateGraph() {
    $.plot("#graph", [
      { label: 'Voltage (V)', data: data.voltage, yaxis: 1 },
      { label: 'Amperage (mA)', data: data.amperage, yaxis: 2 },
      { label: 'Power (W)', data: data.power, yaxis: 3 }
    ], {
      xaxis: {
        mode: "time"
      },
      yaxes: [
        { min: 0, axisLabel: 'Voltage (V)' },
        { min: 0, axisLabel: 'Amperage (mA)', tickFormatter: function (val, axis) { return parseInt(val * 100); } },
        { min: 0, axisLabel: 'Power (W)', position: 'right' }
      ]
    });
  }

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
      data.voltage.push([json.time, json.voltage]);
      data.amperage.push([json.time, json.amperage]);
      data.power.push([json.time, json.voltage * json.amperage]);
      updateGraph();
    };
  }

  updateGraph();
  openWebSocket();
});