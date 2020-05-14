const char MAIN_page[] PROGMEM = R"=====(
<!DOCTYPE html>
<html>
<style>
  .header {
    float: left;
    width: 100%;
  }
  .stats {
    float: left;
    width: 25%;
  }
  .driveway {
    float: left;
    width: 75px;
    background-color: gray;
  }
  .drivewaySpot{
    float: left;
    background-color: green;
    margin: 2px;
    width: 34px;
    padding-bottom: 50px;
  }
  .lvl {
    float: left;
    width: 100%;
    background-color: gray;
  }

  .spot{
    float: left;
    background-color: green;
    margin: 0.261%;
    width: 1.75%;
    padding-bottom: 45px;
  }
  .path{
    background-color: gray;
    padding-bottom: 25px;
  }
  .row{
    background-color: light gray;
    padding-bottom: 50px;
  }
  hr.line{
    border: 1px dashed yellow;
  }
  .right{
    position: relative;
    left: 95%;
    height: 25px;
  }
  .left{
    position: relative;
    left: 2%;
  }
  .enter{
    color: black;
    position: relative;
    left: -400px;
    top: 410px;
    transform: rotateZ(90deg);
  }
}
</style>
<body>

<div class="header">
  <div class="stats">
<!--  <h4>MQTT Message Value:<span id="ADCValue">0</span></h4>-->
  </div>  
  <div class="stats">
<!--  <h4>Spot: <span id="spot">0</span></h4>-->
  </div> 
  <div class="stats">
<!--  <h4>Status: <span id="spotStatus">0</span></h4>-->
  </div> 
  
  <div class="stats"><h2>Zane's Driveway</h2>
    <div class="driveway">
      <div id="z4" class="drivewaySpot"></div>
      <div id="z2" class="drivewaySpot"></div>
    </div>
  </div>
</div>
<h2><center>1st Floor</center></h2>
<div class="lvl">
  <div class="row">
   <div id="1" class="spot"></div>
   <div id="2" class="spot"></div>
   <div id="3" class="spot"></div>
   <div id="4" class="spot"></div>
   <div id="5" class="spot"></div>
   <div id="6" class="spot"></div>
   <div id="7" class="spot"></div>
   <div id="8" class="spot"></div>
   <div id="9" class="spot"></div>
   <div id="10" class="spot"></div>
   <div id="11" class="spot"></div>
   <div id="12" class="spot"></div>
   <div id="13" class="spot"></div>
   <div id="14" class="spot"></div>
   <div id="15" class="spot"></div>
   <div id="16" class="spot"></div>
   <div id="17" class="spot"></div>
   <div id="18" class="spot"></div>
   <div id="19" class="spot"></div>
   <div id="20" class="spot"></div>
   <div id="21" class="spot"></div>
   <div id="22" class="spot"></div>
   <div id="23" class="spot"></div>
   <div id="24" class="spot"></div>
   <div id="25" class="spot"></div>
   <div id="26" class="spot"></div>
   <div id="27" class="spot"></div>
   <div id="28" class="spot"></div>
   <div id="29" class="spot"></div>
   <div id="30" class="spot"></div>
   <div id="31" class="spot"></div>
   <div id="32" class="spot"></div>
   <div id="33" class="spot"></div>
   <div id="34" class="spot"></div>
   <div id="35" class="spot"></div>
   <div id="36" class="spot"></div>
   <div id="37" class="spot"></div>
   <div id="38" class="spot"></div>
   <div id="39" class="spot"></div>
   <div id="40" class="spot"></div>
   <div id="41" class="spot"></div>
   <div id="42" class="spot"></div>
   <div id="43" class="spot"></div>
   <div id="44" class="spot"></div>
  </div>
  <div class="path"></div>
  <hr class="line">
  <div class="path"></div>
  <div class="row">
      <div id="45" class="spot"></div>
     <div id="46" class="spot"></div>
     <div id="47" class="spot"></div>
     <div id="48" class="spot"></div>
     <div id="49" class="spot"></div>
     <div id="50" class="spot"></div>
     <div id="51" class="spot"></div>
     <div id="52" class="spot"></div>
     <div id="53" class="spot"></div>
     <div id="54" class="spot"></div>
     <div id="55" class="spot"></div>
     <div id="56" class="spot"></div>
     <div id="57" class="spot"></div>
     <div id="58" class="spot"></div>
     <div id="59" class="spot"></div>
     <div id="60" class="spot"></div>
     <div id="61" class="spot"></div>
     <div id="62" class="spot"></div>
     <div id="63" class="spot"></div>
     <div id="64" class="spot"></div>
     <div id="65" class="spot"></div>
     <div id="66" class="spot"></div>
     <div id="67" class="spot"></div>
     <div id="68" class="spot"></div>
     <div id="69" class="spot"></div>
     <div id="70" class="spot"></div>
     <div id="71" class="spot"></div>
     <div id="72" class="spot"></div>
     <div id="73" class="spot"></div>
     <div id="74" class="spot"></div>
     <div id="75" class="spot"></div>
     <div id="76" class="spot"></div>
     <div id="77" class="spot"></div>
     <div id="78" class="spot"></div>
     <div id="79" class="spot"></div>
     <div id="80" class="spot"></div>
     <div id="81" class="spot"></div>
     <div id="82" class="spot"></div>
     <div id="83" class="spot"></div>
     <div id="84" class="spot"></div>
     <div id="85" class="spot"></div>
     <div id="86" class="spot"></div>
     <div id="87" class="spot"></div>
     <div id="88" class="spot"></div>
  </div>
</div>

<h2><center>2nd Floor</center></h2>
<div class="lvl">
  <div class="row">
    <div id="89" class="spot"></div>
    <div id="90" class="spot"></div>
    <div id="91" class="spot"></div>
    <div id="92" class="spot"></div>
    <div id="93" class="spot"></div>
    <div id="94" class="spot"></div>
    <div id="95" class="spot"></div>
    <div id="96" class="spot"></div>
    <div id="97" class="spot"></div>
    <div id="98" class="spot"></div>
    <div id="99" class="spot"></div>
    <div id="100" class="spot"></div>
    <div id="101" class="spot"></div>
    <div id="102" class="spot"></div>
    <div id="103" class="spot"></div>
    <div id="104" class="spot"></div>
    <div id="105" class="spot"></div>
    <div id="106" class="spot"></div>
    <div id="107" class="spot"></div>
    <div id="108" class="spot"></div>
    <div id="109" class="spot"></div>
    <div id="110" class="spot"></div>
    <div id="111" class="spot"></div>
    <div id="112" class="spot"></div>
    <div id="113" class="spot"></div>
    <div id="114" class="spot"></div>
    <div id="115" class="spot"></div>
    <div id="116" class="spot"></div>
    <div id="117" class="spot"></div>
    <div id="118" class="spot"></div>
    <div id="119" class="spot"></div>
    <div id="120" class="spot"></div>
    <div id="121" class="spot"></div>
    <div id="122" class="spot"></div>
    <div id="123" class="spot"></div>
    <div id="124" class="spot"></div>
    <div id="125" class="spot"></div>
    <div id="126" class="spot"></div>
    <div id="127" class="spot"></div>
    <div id="128" class="spot"></div>
    <div id="129" class="spot"></div>
    <div id="130" class="spot"></div>
    <div id="131" class="spot"></div>
    <div id="132" class="spot"></div>
  </div>
  <div class="path"></div>
  <hr class="line">
  <div class="path"></div>
  <div class="row">
    <div id="133" class="spot"></div>
    <div id="134" class="spot"></div>
    <div id="135" class="spot"></div>
    <div id="136" class="spot"></div>
    <div id="137" class="spot"></div>
    <div id="138" class="spot"></div>
    <div id="139" class="spot"></div>
    <div id="140" class="spot"></div>
    <div id="141" class="spot"></div>
    <div id="142" class="spot"></div>
    <div id="143" class="spot"></div>
    <div id="144" class="spot"></div>
    <div id="145" class="spot"></div>
    <div id="146" class="spot"></div>
    <div id="147" class="spot"></div>
    <div id="148" class="spot"></div>
    <div id="149" class="spot"></div>
    <div id="150" class="spot"></div>
    <div id="151" class="spot"></div>
    <div id="152" class="spot"></div>
    <div id="153" class="spot"></div>
    <div id="154" class="spot"></div>
    <div id="155" class="spot"></div>
    <div id="156" class="spot"></div>
    <div id="157" class="spot"></div>
    <div id="158" class="spot"></div>
    <div id="159" class="spot"></div>
    <div id="160" class="spot"></div>
    <div id="161" class="spot"></div>
    <div id="162" class="spot"></div>
    <div id="163" class="spot"></div>
    <div id="164" class="spot"></div>
    <div id="165" class="spot"></div>
    <div id="166" class="spot"></div>
    <div id="167" class="spot"></div>
    <div id="168" class="spot"></div>
    <div id="169" class="spot"></div>
    <div id="170" class="spot"></div>
    <div id="171" class="spot"></div>
    <div id="172" class="spot"></div>
    <div id="173" class="spot"></div>
    <div id="174" class="spot"></div>
    <div id="175" class="spot"></div>
    <div id="176" class="spot"></div>
  </div>
</div>

<h2><center>3rd Floor</center></h2>
<div class="lvl">
  <div class="row">
    <div id="177" class="spot"></div>
    <div id="178" class="spot"></div>
    <div id="179" class="spot"></div>
    <div id="180" class="spot"></div>
    <div id="181" class="spot"></div>
    <div id="182" class="spot"></div>
    <div id="183" class="spot"></div>
    <div id="184" class="spot"></div>
    <div id="185" class="spot"></div>
    <div id="186" class="spot"></div>
    <div id="187" class="spot"></div>
    <div id="188" class="spot"></div>
    <div id="189" class="spot"></div>
    <div id="190" class="spot"></div>
    <div id="191" class="spot"></div>
    <div id="192" class="spot"></div>
    <div id="193" class="spot"></div>
    <div id="194" class="spot"></div>
    <div id="195" class="spot"></div>
    <div id="196" class="spot"></div>
    <div id="197" class="spot"></div>
    <div id="198" class="spot"></div>
    <div id="199" class="spot"></div>
    <div id="200" class="spot"></div>
    <div id="201" class="spot"></div>
    <div id="202" class="spot"></div>
    <div id="203" class="spot"></div>
    <div id="204" class="spot"></div>
    <div id="205" class="spot"></div>
    <div id="206" class="spot"></div>
    <div id="207" class="spot"></div>
    <div id="208" class="spot"></div>
    <div id="209" class="spot"></div>
    <div id="210" class="spot"></div>
    <div id="211" class="spot"></div>
    <div id="212" class="spot"></div>
    <div id="213" class="spot"></div>
    <div id="214" class="spot"></div>
    <div id="215" class="spot"></div>
    <div id="216" class="spot"></div>
    <div id="217" class="spot"></div>
    <div id="218" class="spot"></div>
    <div id="219" class="spot"></div>
    <div id="220" class="spot"></div>
  </div>
  <div class="path"></div>
  <hr class="line">
  <div class="path"></div>
  <div class="row">
    <div id="221" class="spot"></div>
    <div id="222" class="spot"></div>
    <div id="223" class="spot"></div>
    <div id="224" class="spot"></div>
    <div id="225" class="spot"></div>
    <div id="226" class="spot"></div>
    <div id="227" class="spot"></div>
    <div id="228" class="spot"></div>
    <div id="229" class="spot"></div>
    <div id="230" class="spot"></div>
    <div id="231" class="spot"></div>
    <div id="232" class="spot"></div>
    <div id="233" class="spot"></div>
    <div id="234" class="spot"></div>
    <div id="235" class="spot"></div>
    <div id="236" class="spot"></div>
    <div id="237" class="spot"></div>
    <div id="238" class="spot"></div>
    <div id="239" class="spot"></div>
    <div id="240" class="spot"></div>
    <div id="241" class="spot"></div>
    <div id="242" class="spot"></div>
    <div id="243" class="spot"></div>
    <div id="244" class="spot"></div>
    <div id="245" class="spot"></div>
    <div id="246" class="spot"></div>
    <div id="247" class="spot"></div>
    <div id="248" class="spot"></div>
    <div id="249" class="spot"></div>
    <div id="250" class="spot"></div>
    <div id="251" class="spot"></div>
    <div id="252" class="spot"></div>
    <div id="253" class="spot"></div>
    <div id="254" class="spot"></div>
    <div id="255" class="spot"></div>
    <div id="256" class="spot"></div>
    <div id="257" class="spot"></div>
    <div id="258" class="spot"></div>
    <div id="259" class="spot"></div>
    <div id="260" class="spot"></div>
    <div id="261" class="spot"></div>
    <div id="262" class="spot"></div>
    <div id="263" class="spot"></div>
    <div id="264" class="spot"></div>
  </div>
</div>

<script>

setInterval(function() {
  getData();
}, 250); //1000mSeconds update rate

function getData() {
  var xhttp = new XMLHttpRequest();
  xhttp.onreadystatechange = function() {
    if (this.readyState == 4 && this.status == 200) {
      var messageOrigin = parseInt(this.responseText.slice(1,3));

      if (messageOrigin == 42){
        var spot = parseInt(this.responseText.slice(4,7));
        var spotStatus = parseInt(this.responseText.slice(7,8));
        
        if (spotStatus == 0){
          document.getElementById(spot).style.backgroundColor = "green";
        }
        else if (spotStatus == 1){
          document.getElementById(spot).style.backgroundColor = "red";
        }
  
        
      }
      else{
        var spot = parseInt(this.responseText.slice(4,5));
        var spotStatus = parseInt(this.responseText.slice(5,6));
        
        if (spotStatus == 0){
          document.getElementById("z"+spot).style.backgroundColor = "green";
        }
        else if (spotStatus == 1){
          document.getElementById("z"+spot).style.backgroundColor = "red";
        }
      }
      
      document.getElementById("ADCValue").innerHTML = this.responseText;
      document.getElementById("spot").innerHTML = spot;
      document.getElementById("spotStatus").innerHTML = spotStatus;
    }
  };
  xhttp.open("GET", "readADC", true);
  xhttp.send();
}
</script>
</body>
</html>
)=====";
