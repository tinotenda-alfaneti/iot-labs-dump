/**
 * based on code by Niel Cameron
 * this page forms the text for the HTML
 * Now it is one continous piece because there are no variables
 * Store array (page) data in flash (program) memory instead of SRAM, 
 * qualifeid by the PROGMEM modifier.
 * R"()" is a raw literal string.
 * 
 * This page is returned to client, on the client side, the javascript will
 * execute. THe JS/AJAX code is at end of the file
 */



char page[] PROGMEM = R"(
<!DOCTYPE html>
<html>
<head>
  <title>Local network AP mode</title>
  <style>
    body {margin-top:50px; font-family:Arial}
    body {font-size:20px; text-align:center}
    .btn {display:inline-block; width:280px; margin:10px; padding:30px}
    .btn {font-size:30px; color:black; text-decoration:none}
    .on {background-color:green}
    .off {background-color:red}
    td {font-size:30px; margin-top:50px; margin-bottom:5px}
    p {font-size:30px; margin-top:50px; margin-bottom:5px}
  </style>
</head>
<body>
  <h1>ESP32 Water Tank Monitor</h1>

  <div>
    <p>Pump Mode: <span id="mode">AUTO</span></p>
    <button class='btn off' id='pumpBtnId' onclick='controlPump()'>Press to switch pump</button>
    <button class='btn mode off' id='pumpModeId' onclick='controlPumpMode()'>Press to turn MANUAL mode</button>
  </div>

  <div id="content" style="font-family: Arial, sans-serif; font-size: 16px; color: #333; padding: 10px;"></div>

  <script>
    function controlPump() {
      var text;
      var state = document.getElementById('pumpBtnId').className;
      state = (state == 'btn on' ? 'btn off' : 'btn on');
      text = (state == 'btn on' ? 'PUMP OFF' : 'PUMP ON');
      document.getElementById('pumpBtnId').className = state;
      document.getElementById('pumpBtnId').innerHTML = 'Press to turn ' + text;
      const xhttp = new XMLHttpRequest();
      xhttp.onload = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('pumpState').innerHTML = this.responseText;
        }
      };
      xhttp.open('GET', 'startPump', true);
      xhttp.send();
    }

    // setInterval(controlPumpMode, 1000);
    function controlPumpMode() {
      var text;
      var state = document.getElementById('pumpModeId').className;
      state = (state == 'btn mode on' ? 'btn mode off' : 'btn mode on');
      text = (state == 'btn mode on' ? 'AUTO' : 'MANUAL');
      mode = (state == 'btn mode on' ? 'MANUAL' : 'AUTO');
      document.getElementById('pumpModeId').className = state;
      document.getElementById('pumpModeId').innerHTML = 'Press to turn ' + text + ' mode';
      document.getElementById('mode').innerHTML = mode; 
      const xhttp = new XMLHttpRequest();
      xhttp.onload = function() {
        if (this.readyState == 4 && this.status == 200) {
          document.getElementById('pumpMode').innerHTML = this.responseText;
          var el = document.querySelectorAll(".manual");
          if (this.responseText == "AUTO") {
            for (var i = 0; i < el.length; i++) {
              el[i].style.display = "none";
            }
          } else {
            for (var i = 0; i < el.length; i++) {
              el[i].style.display = "table";
            }
          }
        }
      };
      xhttp.open('GET', 'changeMode', true);
      xhttp.send();
    }

    setInterval(updateVars, 200);

    function updateVars() {
      var xhttp = new XMLHttpRequest();
      
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          state = this.responseText;
          text = (state == 'ON' ? 'OFF' : 'ON');
          cls = (state == 'ON' ? 'btn on' : 'btn off');
          document.getElementById('pumpBtnId').className = cls;
          document.getElementById('pumpBtnId').innerHTML = 'Press to turn ' + text;
        }
      };
      xhttp.open('GET', 'updateVars', true);
      xhttp.send();
    }

    setInterval(reload, 1000);

    function reload() {
      var xhttp = new XMLHttpRequest();
      xhttp.onreadystatechange = function() {
        if (this.readyState == 4 && this.status == 200) {
          var data = JSON.parse(this.responseText); // Parse JSON response
          var records = data.readings;
          var html = '';
          for (var i = 0; i < records.length; i++) {
            html += '<p style="margin: 5px 0; padding: 5px; border: 1px solid #ccc; border-radius: 5px;">' + "Water Level " + i + ": " + records[i] + '</p>';
          }
          document.getElementById('content').innerHTML = html;
        }
      };
      xhttp.open('GET', 'viewRecords', true);
      xhttp.send();
    }
  </script>
</body>
</html>

)";
