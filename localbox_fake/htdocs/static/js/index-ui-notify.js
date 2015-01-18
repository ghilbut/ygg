function Notify(rootNode) {

  var rootNode = rootNode
    , textNode = rootNode.getElementsByClassName('notify-text')[0];


  this.info = function (text) {
    rootNode.className = 'notify-info';
    textNode.textContent = text;
  };

  this.warning = function (text) {
    rootNode.className = 'notify-warnign';
    textNode.textContent = text;
  };

  this.error = function (text) {
    rootNode.className = 'notify-error';
    textNode.textContent = text;
  };
}