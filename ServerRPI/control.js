		function updateSlider(slideAmount) {
			var sliderDiv = document.getElementById("sliderAmount");
			sliderDiv.innerHTML = slideAmount;
			var n = slideAmount.toString();
			KeyboardHelper = { 65: n+"l-", 81: n+"l+", 68: n+"r-", 69: n+"r+" };
		}

	function sendKey(key) {
		console.log(key);
		$.get("saver.php", {key:key});//,function success(e){/*$(".controls").append(key).append("  -  ");*/});
	}
	var keys = [];
	var KeyboardHelper = { 65: "600l-", 81: "600l+", 68: "600r-", 69: "600r+" };
	document.addEventListener('keydown', keyDownHandler, false);
	document.addEventListener('keyup', keyUpHandler, false);

	function keyDownHandler(event) {
		pressed = event.keyCode;
		if (pressed in KeyboardHelper && !keys.includes(pressed)) {
			l = 0;
			r = 0;
			keys.map(function (num){if (KeyboardHelper[num].includes("l")) {l++;} else {r++;}});
			if ((KeyboardHelper[pressed].includes("l") && l == 0) || (KeyboardHelper[pressed].includes("r") && r == 0)){
				keys.push(pressed);
				combine();
			}
		}
	}

	function keyUpHandler(event) {
		index = keys.indexOf(event.keyCode);
		if (index > -1) {
			keys.splice(index, 1);
			combine();
		}
	}

	function combine() {
		combined = keys.map(function (num){if (num in KeyboardHelper) return (KeyboardHelper[num]);}).join('');
		sendKey(combined);
	}
$(document).ready(function() {
	const leftCamTurnButton = document.getElementById('leftCamTurn');
	const rightCamTurnButton = document.getElementById('rightCamTurn');
	const upCamTurnButton = document.getElementById('upCamTurn');
	const downCamTurnButton = document.getElementById('downCamTurn');

	const leftSteerButton = document.getElementById('leftSteer');
	const rightSteerButton = document.getElementById('rightSteer');
	const upSteerButton = document.getElementById('upSteer');
	const downSteerButton = document.getElementById('downSteer');

	function sendDirections(direction) {
		$.get("cameraSaver.php", {direction:direction},function success(e){});
	}

	leftCamTurnButton.addEventListener('mousedown', () => {
		sendDirections('h+');
	});
	rightCamTurnButton.addEventListener('mousedown', () => {
		sendDirections('h-');
	})
	upCamTurnButton.addEventListener('mousedown', () => {
		sendDirections('v-');
	});
	downCamTurnButton.addEventListener('mousedown', () => {
		sendDirections('v+');
	})

	leftSteerButton.addEventListener('mousedown', () => {
		sendKey(KeyboardHelper[69].concat(KeyboardHelper[65]));
	});
	rightSteerButton.addEventListener('mousedown', () => {
		sendKey(KeyboardHelper[81].concat(KeyboardHelper[68]));
	})
	upSteerButton.addEventListener('mousedown', () => {
		sendKey(KeyboardHelper[81].concat(KeyboardHelper[69]));
	});
	downSteerButton.addEventListener('mousedown', () => {
		sendKey(KeyboardHelper[65].concat(KeyboardHelper[68]));
	})

	document.addEventListener('mouseup', () => {
		sendDirections(' ');
		if (keys.length == 0) sendKey('');
	});

});
