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
  .btn {display:block; width:280px; margin:auto; padding:30px}
  .btn {font-size:30px; color:black; text-decoration:none}
  .on {background-color:green}
  .off {background-color:red}
  .zero {background-color:gray}
  td {font-size:30px; margin-top:50px; margin-bottom:5px}
  p {font-size:30px; margin-top:50px; margin-bottom:5px}
  </style>

</head>
  <body>
  <h1>ESP32 local area network</h1>
  <table style='width:100%'>
    <tr>
      <td>Green LED is <span id='LEDG'>OFF</span> now</td>
      <td>Red LED is <span id='LEDR'>OFF</span> now</td>
    </tr>
  </table>
  <table style='width:100%'>
    <tr>
      <td>
        <button class = 'btn off' id='GreenLedBtnId'
         onclick = 'handleGreen()'>Press to turn Green LED ON
        </button>
      </td>
      <td>
        <button class = 'btn off' id='RedLedBtnId'
          onclick = 'handleRed()'>Press to turn Red LED ON
        </button>
      </td>
    </tr>
  </table>

<p>Counter is <span id='counter'>0</span> now</p>

<button class = 'btn zero' id = 'zero'
     onclick = 'handleZero()'>Press to zero counter</button>

<script>

function handleGreen(){
  var  text;
  
  var state = document.getElementById('GreenLedBtnId').className;
  state = (state == 'btn on' ? 'btn off' : 'btn on');
  text = (state == 'btn on' ? 'Green OFF' : 'Green ON');
  document.getElementById('GreenLedBtnId').className = state;
  document.getElementById('GreenLedBtnId').innerHTML = 'Press to turn ' + text;
  const xhttp=new XMLHttpRequest();
  xhttp.onload=function(){
    if (this.readyState == 4 && this.status == 200)
    document.getElementById('LEDG').innerHTML = this.responseText;
  };  
   // xhttp.open('GET', 'staticdata.html', true); 
  xhttp.open('GET', 'LEDGurl', true);
  xhttp.send();
}
function handleRed(){
  var URL, text;
  URL = 'LEDRurl';
  
  
  var state = document.getElementById('RedLedBtnId').className;
  state = (state == 'btn on' ? 'btn off' : 'btn on');
  text = (state == 'btn on' ? 'Red OFF' : 'Red ON');

  document.getElementById('RedLedBtnId').className = state;
  document.getElementById('RedLedBtnId').innerHTML = 'Press to turn ' + text;
  const xhttp=new XMLHttpRequest();
  xhttp.onload=function(){
    if (this.readyState == 4 && this.status == 200)
    document.getElementById('LEDR').innerHTML = this.responseText;
  };  
  // xhttp.open('GET', 'staticdata.html', true); 
  xhttp.open('GET', URL, true);
  xhttp.send();
 
  
}


function handleZero(){
  var URL, variab, text;
  URL = 'zeroUrl';
  variab = 'counter';
  const xhttp=new XMLHttpRequest();
  xhttp.onload=function(){
    if (this.readyState == 4 && this.status == 200)
    document.getElementById('counter').innerHTML = this.responseText;
  };  
  
  
  //xhttp.open('GET', 'staticdata.html', true); 
  xhttp.open('GET', URL, true);
  xhttp.send();
  
}




<!-- not needed, but enables periodic update of count -->
setInterval(reload, 1000);
function reload()
{
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function()
  {
    if(this.readyState == 4 && this.status == 200)
    document.getElementById('counter').innerHTML = this.
    responseText;
  };
  xhttp.open('GET', 'countUrl', true);
  xhttp.send();
}
</script>
</body></html>  

)";
