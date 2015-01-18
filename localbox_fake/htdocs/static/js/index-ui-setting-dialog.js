function SettingDialog(rootNode, parentNode, notifyNode) {

  var parentNode = parentNode
    , rootNode = rootNode
    , notify = new Notify(notifyNode)
    , onstart
    , retval = {};



  $(rootNode).dialog({
    appendTo: parentNode,
    autoOpen: true,
    beforeClose: function () {},
    closeOnEscape: false,
    create: function(event, ui) {
      var x = event.target.parentNode.getElementsByClassName('ui-dialog-titlebar-close')[0];
      x.parentNode.removeChild(x);
    },
    draggable: false,
    modal: true,
    position: {my: "center", at: "center", of: parentNode},
    resizable: false,
    title: 'Settings',

    width: '80%',
    height: 'auto',
    buttons: {
      'Start': start,
      'Reset': reset
    }
  });



  retval.open = function () {
    $(rootNode).dialog('open');
  };

  retval.__defineGetter__('onstart', function () {
    return onstart; 
  });
  retval.__defineSetter__('onstart', function (val) {
    onstart = val;
  });



  function start() {

    if (! ('WebSocket' in window)) {
      notify.error('WebSocket is NOT supported by your Browser!');
      return;
    }

    if (!onstart || typeof onstart !== 'function') {
      alert('"onstart" callback is invalid function!');
      return;
    }

    if (!onstart()) {
      alert('"onstart" callback function return false!');
      return;
    }

    $(rootNode).dialog('close');
  }

  function reset() {
    rootNode.getElementsByClassName('box-id')[0].value = 'BOX-ID-00';
  }



  return retval;
};