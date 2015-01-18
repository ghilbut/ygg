

API = (function () {

  function Request(url, json, async, callback) {

    var xhr = new XMLHttpRequest()
      , json = json
      , async = async
      , callback = callback;

    function onreadystatechange(xhr) {
      if (xhr.readyState === xhr.DONE) {
        if (callback && typeof callback === 'function') {
          callback(xhr.status, xhr.responseText);
        }
      }
    }

    xhr.open('POST', url, async);
    xhr.setRequestHeader('Content-Type', 'application/json');
    if (async) {
      xhr.onreadystatechange = function () {
        onreadystatechange(this);
      };
    }

    this.send = function () {
      xhr.send(json);
      if (!async) {
        return {'status': xhr.status, 'responseText': xhr.responseText};
      }
    };
  }

  function regLocalBoxUserMapping(json, async, callback) {
    var r = new Request('http://127.0.0.1/reg/localbox/user/mapping', json, async, callback);
    r.send();
  }

  function unregLocalBoxUserMapping(json, async, callback) {
    var r = new Request('http://127.0.0.1/unreg/localbox/user/mapping', json, async, callback);
    r.send(); 
  }

  return {
    regLocalBoxUserMapping: regLocalBoxUserMapping,
    unregLocalBoxUserMapping: unregLocalBoxUserMapping
  };
})();


