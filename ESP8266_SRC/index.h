/**
 * @brief The web page which is loaded
 */
const char MAIN_page[] PROGMEM = R"=====(
  <html>
  <head>
    <title>Car Control</title>
    <meta name="viewport" content="width=device-width, initial-scale=1" />
    <script language="javascript">

  	var connection = new WebSocket('ws://' + location.hostname + ':81/', ['arduino']);
  	connection.onopen = function () {
  	  connection.send('Connect ' + new Date());
  	};
  	connection.onerror = function (error) {
  	  console.log('WebSocket Error ', error);
  	};
  	connection.onmessage = function (e) {
  	  console.log('Server: ', e.data);
  	};
  	connection.onclose = function () {
  	  console.log('WebSocket connection closed');
  	};

  	function isMobile() {
  		if( /Android|webOS|iPhone|iPad|iPod|BlackBerry|IEMobile|Opera Mini/i.test(navigator.userAgent) )
  			return true;
  		return false;
  	}

  	var goUp = false;
  	var goDown = false;
  	var goLeft = false;
  	var goRight = false;
  	var goAuto = false;
  	function btnClicked(clicked_id, act) {
  		if (isMobile()) return;
  		switch (clicked_id) {
  			case 'btnUp':
  				goUp = act;
  			break;
  			case 'btnDown':
  				goDown = act;
  			break;
  			case 'btnLeft':
  				goLeft = act;
  			break;
  			case 'btnRight':
  				goRight = act;
  			break;
  			case 'btnAuto':
  				goAuto = act;
  			break;
  		}

  		var mess = goAuto << 4 | goUp << 3 | goDown << 2 | goLeft << 1 | goRight << 0;
  		connection.send(mess & 127);
  	}
  	function btnClickedM(clicked_id, act) {
  		switch (clicked_id) {
  			case 'btnUp':
  				goUp = act;
  			break;
  			case 'btnDown':
  				goDown = act;
  			break;
  			case 'btnLeft':
  				goLeft = act;
  			break;
  			case 'btnRight':
  				goRight = act;
  			break;
  			case 'btnAuto':
  				goAuto = act;
  			break;
  		}

  		var mess = goUp << 3 | goDown << 2 | goLeft << 1 | goRight << 0;
  		if (goAuto) mess = 5;
  		connection.send(mess & 127);
  	}

    </script>
    <style>
    .button {
  	height:70px;
  	width:70px;
    }
    </style>
  </head>

  <body>
    <center>
      <header>
        <h3> Car Control </h3>
      </header>
  	<hr>
      <div>
        <button id="btnUp" class="button" onmousedown="btnClicked(this.id, true)" onmouseup="btnClicked(this.id, false)" ontouchstart="btnClickedM(this.id, true)" ontouchend="btnClickedM(this.id, false)">  </button> <br>
        <button id="btnLeft" class="button" onmousedown="btnClicked(this.id, true)" onmouseup="btnClicked(this.id, false)" ontouchstart="btnClickedM(this.id, true)" ontouchend="btnClickedM(this.id, false)">  </button>
        <button id="btnAuto" class="button" onmousedown="btnClicked(this.id, true)" onmouseup="btnClicked(this.id, false)" ontouchstart="btnClickedM(this.id, true)" ontouchend="btnClickedM(this.id, false)">  </button>
        <button id="btnRight" class="button" onmousedown="btnClicked(this.id, true)" onmouseup="btnClicked(this.id, false)" ontouchstart="btnClickedM(this.id, true)" ontouchend="btnClickedM(this.id, false)">  </button> <br>
        <button id="btnDown" class="button" onmousedown="btnClicked(this.id, true)" onmouseup="btnClicked(this.id, false)" ontouchstart="btnClickedM(this.id, true)" ontouchend="btnClickedM(this.id, false)">  </button> <br>
      </div>
    </center>
  </body>
  </html>

)=====";
