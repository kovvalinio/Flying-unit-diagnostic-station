const char webpage[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style type="text/css">
.button {
  background-color: #4CAF50; /* Green */
  border: none;
  color: white;
  padding: 15px 32px;
  text-align: center;
  text-decoration: none;
  display: inline-block;
  font-size: 16px;
}
</style>
<body style="background-color: #f9e79f ">
<center>
<div>
<h1>AJAX BASED ESP8266 WEBSERVER</h1>
</div>
 <br>
<div><h2>
  X axis: <span id="xaxis">0</span><br><br>
  Y axis: <span id="yaxis">0</span><br><br>
  Z axis: <span id="zaxis">0</span><br><br>
  Sound: <span id="soundsamp">0</span><br><br>
</h2>
</div>
<script>
setInterval(function() 
{
  getDatax();
  getDatay();
  getDataz();
  getDataSound();
}, 200);
function getDataSound() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("soundsamp").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "soundsamp", true);
  xhttp.send();
} 
function getDatax() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("xaxis").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "xaxis", true);
  xhttp.send();
}
function getDatay() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("yaxis").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "yaxis", true);
  xhttp.send();
}
function getDataz() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      document.getElementById("zaxis").innerHTML =
      this.responseText;
    }
  };
  xhttp.open("GET", "zaxis", true);
  xhttp.send();
}
</script>
</center>
</body>
</html>
)=====";
