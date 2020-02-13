function reboot() {
  var http = new XMLHttpRequest();
  http.open("POST", "reboot.htm", true);
  http.withCredentials = true;
  http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  var params = "reboot=" + "true"; // probably use document.getElementById(...).value
  http.send(params);
  alert("Homepoint rebooted. Please wait a few seconds.");
}