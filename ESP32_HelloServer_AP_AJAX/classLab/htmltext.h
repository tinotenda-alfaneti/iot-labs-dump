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
  .btn {display:block; width:100px; margin:auto; padding:20px}
  .btn {font-size:20px; color:black; text-decoration:none}
  .on {background-color:green}
  .off {background-color:red}
  td {font-size:30px; margin-top:50px; margin-bottom:5px}
  p {font-size:30px; margin-top:50px; margin-bottom:5px}
  </style>

</head>
  <body>
  <h1>ESP32 local area network</h1>
  <p>Temperature Reading: <span id="temp">---</span></p>
  <p>Humidity Reading: <span id="humi">---</span></p>
  <p>Pump Status: <span id="pump">---</span></p>
  
  <table style='width:100%; display=none;' class='manual'>
    <tr>
      <td> Pump Status <span id='pumpState'>OFF</span> now</td>
    </tr>
  </table>
  <table style='width:100%; display=none;' class='manual'>
    <tr>
      <td>
        <button class = 'btn off' id='pumpBtnId'
         onclick = 'controlPump()'>Press to turn Pump ON
        </button>
      </td>
    </tr>
  </table>

  <table style='width:100%'>
    <tr>
      <td> Pump Mode: <span id='pumpMode'>AUTO</span> now</td>
    </tr>
  </table>
  <table style='width:100%'>
    <tr>
      <td>
        <button class = 'mode btn off' id='pumpModeId'
         onclick = 'controlPumpMode()'>Press to turn ON Manual Control
        </button>
      </td>
    </tr>
  </table>

  <table style='width:100%'>
    <tr>
      <td>
        <button class = 'btn'
         onclick = 'getData()'>Press to get reading records
        </button>
      </td>
    </tr>
  </table>

  <div id="content"> </div>

<script>

function getData() {
  // Create a new XMLHttpRequest object
        var xhr = new XMLHttpRequest();

        // Configure the request
        xhr.open('GET', 'http://192.168.88.21/iot/classactivity/dataCollectionApi.php', true);

        // Set up a callback function to handle the response
        xhr.onreadystatechange = function() {
            if (xhr.readyState === XMLHttpRequest.DONE) {
                if (xhr.status === 200) {
                    // Update the content with the response data
                    document.getElementById('content').innerHTML = xhr.responseText;
                } else {
                    // Handle errors
                    console.error('Request failed: ' + xhr.status);
                }
            }
        };

        // Send the request
        xhr.send();
}


function controlPump(){
  var  text;
  
  var state = document.getElementById('pumpBtnId').className;
  state = (state == 'btn on' ? 'btn off' : 'btn on');
  text = (state == 'btn on' ? 'PUMP OFF' : 'PUMP ON');
  document.getElementById('pumpBtnId').className = state;
  document.getElementById('pumpBtnId').innerHTML = 'Press to turn ' + text;
  const xhttp=new XMLHttpRequest();
  xhttp.onload=function(){
    if (this.readyState == 4 && this.status == 200) {
    document.getElementById('pumpState').innerHTML = this.responseText;
    }
  };  

  xhttp.open('GET', 'start-pump', true);
  xhttp.send();
}





function controlPumpMode(){
  var text;
  
  
  var state = document.getElementById('pumpModeId').className;
  state = (state == 'btn on' ? 'btn off' : 'btn on');
  text = (state == 'mode btn on' ? 'MANUAL' : 'AUTO');

  document.getElementById('pumpModeId').className = state;
  document.getElementById('pumpModeId').innerHTML = 'Press to turn ON ' + text + ' mode';
  const xhttp=new XMLHttpRequest();
  xhttp.onload=function(){
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
  xhttp.open('GET', 'change-mode', true);
  xhttp.send();
 
  
}


setInterval(reload, 1000);
function reload()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200)
    var values = this.responseText.split(",");
      
      // Extract temperature and humidity values
      var temp = values[0];
      var humi = values[1];
    document.getElementById('temp').innerHTML = temp;
  };
  xhttp.open('GET', 'temp-humi', true);
  xhttp.send();
}
</script>
</body></html>  

)";
