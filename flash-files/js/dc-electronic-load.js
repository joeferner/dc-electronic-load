'use strict';

$(function() {
  var setAmperageForm = $('#set-amperage-form');
  var connectButton = $('.btn-connect');
  var connectMessage = $('.connect-message');

  var data = {
    voltage: [],
    amperage: [],
    power: []
  };

  function round(v, places) {
    return parseFloat(v).toFixed(places);
  }

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
        { min: 0, axisLabel: 'Amperage (mA)', tickFormatter: function(val, axis) { return parseInt(val * 100); } },
        { min: 0, axisLabel: 'Power (W)', position: 'right' }
      ]
    });
  }

  function clearGraph() {
    data.voltage = [];
    data.amperage = [];
    data.power = [];
    updateGraph();
  }

  function toggleConnect() {
    if (document.wsConnection) {
      closeWebSocket();
    } else {
      openWebSocket();
    }
  }

  function closeWebSocket() {
    if (document.wsConnection) {
      document.wsConnection.send("CLOSE");
      document.wsConnection.close();
    }
    connectButton.html('Connect');
    connectButton.removeClass('disabled');
    document.wsConnection = null;
  }

  function openWebSocket() {
    connectMessage.html('');
    connectButton.html('Connecting...');
    connectButton.addClass('disabled');

    var wsAddr = 'ws://' + location.hostname + (location.port ? ':' + location.port : '') + '/ws';
    console.log('connecting ws: ' + wsAddr);
    document.wsConnection = new WebSocket(wsAddr);

    document.wsConnection.onopen = function() {
      console.log('connected ws: ' + wsAddr);
      connectButton.html('Disconnect');
      connectButton.removeClass('disabled');
    };

    document.wsConnection.onerror = function(error) {
      console.log('WebSocket Error ', error, document.wsConnection);
      connectMessage.html('Error connecting');
      closeWebSocket();
    };

    document.wsConnection.onmessage = function(e) {
      var json = JSON.parse(e.data);
      console.log('Server:', json);
      var power = (json.voltage / 1000.0) * (json.amperage / 1000.0);
      $('#current-set-amperage').html(round(json.targetAmps, 0) + "mA");
      $('#current-amperage').html(round(json.amperage, 0) + "mA");
      $('#current-voltage').html(round(json.voltage / 1000.0, 2) + "V");
      $('#current-power').html(round(power, 2) + "W");
      data.voltage.push([json.time, json.voltage / 1000.0]);
      data.amperage.push([json.time, json.amperage]);
      data.power.push([json.time, power]);
      updateGraph();
    };
  }

  function ajaxForms() {
    ajaxSetAmperageForm();
  }

  function setAmps(value) {
    $.ajax({
      type: "POST",
      url: setAmperageForm.attr('action'),
      data: {
        value: value
      },
      success: function() {
        console.log(arguments);
      }
    });
  }

  function ajaxSetAmperageForm() {
    $(".btn-zero-amps", setAmperageForm).click(function(event) {
      setAmps(0);
    });

    $(".btn-set-amps", setAmperageForm).click(function(event) {
      event.preventDefault();
      var valueField = $("[name='value']", setAmperageForm);
      var value = $.trim(valueField.val());
      if (value == '' || parseInt(value) < 0 || parseInt(value) > 5000) {
        valueField.closest('div.form-group').addClass('has-error');
        return;
      }
      valueField.closest('div.form-group').removeClass('has-error');

      setAmps(value);
    });
  }

  updateGraph();
  openWebSocket();
  ajaxForms();
  connectButton.click(toggleConnect);
  $('.btn-clear').click(clearGraph);
});
