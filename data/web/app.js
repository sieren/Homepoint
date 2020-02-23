function reboot() {
  var http = new XMLHttpRequest();
  http.open("POST", "reboot.htm", true);
  http.withCredentials = true;
  http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  var params = "reboot=" + "true"; // probably use document.getElementById(...).value

  http.onreadystatechange = function() {//Call a function when the state changes.
    if(http.readyState == 4 && http.status == 200) {
      alert(http.responseText);
    }
  }
  http.send(params);
  alert("Homepoint rebooted. Please wait a few seconds.");
}

function reload() {
  var http = new XMLHttpRequest();
  http.open("POST", "reload.htm", true);
  http.withCredentials = true;
  http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  var params = "reload=" + "true"; // probably use document.getElementById(...).value

  http.onreadystatechange = function() {//Call a function when the state changes.
    if(http.readyState == 4 && http.status == 200) {
      alert(http.responseText);
    }
  }
  http.send(params);
  alert("Homepoint restarted.");
}