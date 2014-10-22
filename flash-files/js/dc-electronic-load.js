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
        { min: 0, axisLabel: 'Amperage (mA)', tickFormatter: function(val, axis) { return parseInt(val * 100); } },
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
      $('#current-set-amperage').html(json.targetAmps + "mA");
      data.voltage.push([json.time, json.voltage]);
      data.amperage.push([json.time, json.amperage]);
      data.power.push([json.time, json.voltage * json.amperage]);
      updateGraph();
    };
  }

  function ajaxForms() {
    ajaxSetAmperageForm();
  }

  function ajaxSetAmperageForm() {
    var setAmperageForm = $('#set-amperage-form');
    $("[type='submit']", setAmperageForm).click(function(event) {
      event.preventDefault();
      var valueField = $("[name='value']", setAmperageForm);
      var value = $.trim(valueField.val());
      if (value == '' || parseInt(value) < 0 || parseInt(value) > 5000) {
        valueField.closest('div.form-group').addClass('has-error');
        return;
      }
      valueField.closest('div.form-group').removeClass('has-error');

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
    });
  }

  updateGraph();
  openWebSocket();
  ajaxForms();
});
