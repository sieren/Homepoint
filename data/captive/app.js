function reboot() {
  var http = new XMLHttpRequest();
  http.open("POST", "updateConfig.htm", true);
  http.withCredentials = true;
  http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  var params = "ssid=" + document.getElementsByName("ssid")[0].value +
  "&ssidPassword=" + document.getElementsByName("ssidPassword")[0].value +
  "&loginName=" + document.getElementsByName("loginName")[0].value +
  "&loginPassword=" + document.getElementsByName("loginPassword")[0].value
  http.send(params);
  alert("Homepoint rebooted. Please wait a few seconds and check the new IP on the status bar of the display.");
}