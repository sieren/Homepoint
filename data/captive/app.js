function reboot() {
  var http = new XMLHttpRequest();
  http.open("POST", "updateConfig.htm", true);
  http.withCredentials = true;
  http.setRequestHeader("Content-type","application/x-www-form-urlencoded");
  var params = "ssid=" + encodeURIComponent(document.getElementsByName("ssid")[0].value) +
  "&ssidPassword=" + encodeURIComponent(document.getElementsByName("ssidPassword")[0].value) +
  "&loginName=" + encodeURIComponent(document.getElementsByName("loginName")[0].value) +
  "&loginPassword=" + encodeURIComponent(document.getElementsByName("loginPassword")[0].value)
  http.send(params);
  alert("Homepoint rebooted. Please wait a few seconds and check the new IP on the status bar of the display.");
}