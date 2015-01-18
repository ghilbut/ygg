


function LocalBox(props) {

  var ws
    , id = props.id;


  this.start = function () {
    if (ws) {
      return false;
    }

    ws = new WebSocket('ws://localhost:8070/');
    ws.onopen = onopen;
    ws.onmessage = onmessage;
    ws.onerror = onerror;
    ws.onclose = onclose;
    return true;
  };

  this.send = function (message) {
    if (message) {
      ws.send(message);
    }
  };

  this.stop = function () {
    finalize();
    ws.close();
  };


  function finalize() {

  };

  function onopen() {
    console.log('onopen');
    ws.send(JSON.stringify({'id': id}));
  }

  function onmessage(event) {
    console.log('onmessage', event);
  }

  function onerror(event) {
    console.log('onerror', event);
  }

  function onclose() {
    console.log('onclose');
    finalize();
  }
}