// Declare some vars
var keys = [];
var KeyboardHelperTank = {
	65: "600l-",
	81: "600l+",
	68: "600r-",
	69: "600r+"
};
var KeyboardHelperCar = {
	83: "600l-",
	87: "600l+",
	65: "600r-",
	68: "600r+",
}; // Right now I'm using the left motor as if it was thrust.
document.addEventListener("keydown", keyDownHandler, false);
document.addEventListener("keyup", keyUpHandler, false);

// Update slider amount(and add it to KeyboardHelper var)
function updateSlider(slideAmount) {
	var sliderDiv = document.getElementById("sliderAmount");
	sliderDiv.innerHTML = slideAmount;
	var n = slideAmount.toString();
	KeyboardHelperTank = {
		65: n + KeyboardHelperTank[65].slice(-2),
		81: n + KeyboardHelperTank[81].slice(-2),
		68: n + KeyboardHelperTank[68].slice(-2),
		69: n + KeyboardHelperTank[69].slice(-2),
	};
	KeyboardHelperCar = {
		83: n + KeyboardHelperCar[83].slice(-2),
		87: n + KeyboardHelperCar[87].slice(-2),
		65: n + KeyboardHelperCar[65].slice(-2),
		68: n + KeyboardHelperCar[68].slice(-2),
	}; // Right now I'm using the left motor as if it was thrust.
}

// Handle key presses
function keyDownHandler(event) {
	var l = 0,
		r = 0;
	var pressed = event.keyCode;
	if (pressed in KeyboardHelperTank && !keys.includes(pressed) && document.getElementById("tdm").checked) {
		l = r = 0;
		keys.map(function(num) {
			if (KeyboardHelperTank[num].includes("l")) {
				l++;
			} else {
				r++;
			}
		});
		if ((KeyboardHelperTank[pressed].includes("l") && l === 0) || (KeyboardHelperTank[pressed].includes("r") && r === 0)) {
			keys.push(pressed);
			combine();
		}
	} else if (pressed in KeyboardHelperCar && !keys.includes(pressed) && document.getElementById("cdm").checked) {
		l = r = 0;
		keys.map(function(num) {
			if (KeyboardHelperCar[num].includes("l")) {
				l++;
			} else {
				r++;
			}
		});
		if ((KeyboardHelperCar[pressed].includes("l") && l === 0) || (KeyboardHelperCar[pressed].includes("r") && r === 0)) {
			keys.push(pressed);
			combine();
		}
	}
}

// Handle key releases
function keyUpHandler(event) {
	var index = keys.indexOf(event.keyCode);
	if (index > -1) {
		keys.splice(index, 1);
		combine();
	}
}

// Send motor instructions
function sendKey(key) {
	console.log(key);
	$.get("saver.php", {
		key: key
	});
}
// Send keys that where used
function combine() {
	var combined;
	// Check whether we should use car or tank drive
	if (document.getElementById("tdm").checked) {
		// Tank drive mode radio button is checked
		combined = keys.map(function(num) {
			if (num in KeyboardHelperTank) return KeyboardHelperTank[num];
		}).join("");
	} else if (document.getElementById("cdm").checked) {
		// Car drive mode radio button is checked
		combined = keys.map(function(num) {
			if (num in KeyboardHelperCar) return KeyboardHelperCar[num];
		}).join("");
	}
	sendKey(combined);
}
$(document).ready(function() {
	// Camera button elements
	const leftCamTurnButton = document.getElementById("leftCamTurn");
	const rightCamTurnButton = document.getElementById("rightCamTurn");
	const upCamTurnButton = document.getElementById("upCamTurn");
	const downCamTurnButton = document.getElementById("downCamTurn");
	const reversePolarityL = document.getElementById('reversePolarityL');
	const reversePolarityR = document.getElementById('reversePolarityR');
	const remapSpeed = document.getElementById('remapSpeed');

	// Send camera instructions
	function sendDirections(direction) {
		$.get("cameraSaver.php", {
			direction: direction
		});
	}

	// Camera buttons
	leftCamTurnButton.addEventListener("mousedown", () => {
		sendDirections("h+");
	});
	rightCamTurnButton.addEventListener("mousedown", () => {
		sendDirections("h-");
	});
	upCamTurnButton.addEventListener("mousedown", () => {
		sendDirections("v-");
	});
	downCamTurnButton.addEventListener("mousedown", () => {
		sendDirections("v+");
	});

	//Reverses left motor polarity
	reversePolarityL.addEventListener("change", (event) => {
		if (event.target.checked){
			KeyboardHelperTank[65] = KeyboardHelperTank[65].slice(0, -1).concat("+");
			KeyboardHelperTank[81] = KeyboardHelperTank[81].slice(0, -1).concat("-");
			KeyboardHelperCar[83] = KeyboardHelperCar[83].slice(0, -1).concat("+");
			KeyboardHelperCar[87] = KeyboardHelperCar[87].slice(0, -1).concat("-");
		} else {
			KeyboardHelperTank[65] = KeyboardHelperTank[65].slice(0, -1).concat("-");
			KeyboardHelperTank[81] = KeyboardHelperTank[81].slice(0, -1).concat("+");
			KeyboardHelperCar[83] = KeyboardHelperCar[83].slice(0, -1).concat("-");
			KeyboardHelperCar[87] = KeyboardHelperCar[87].slice(0, -1).concat("+");
		}
	});
	// Reverses right motor polarity
	reversePolarityR.addEventListener("change", (event) => {
		if (event.target.checked){
			KeyboardHelperTank[68] = KeyboardHelperTank[68].slice(0, -1).concat("+");
			KeyboardHelperTank[69] = KeyboardHelperTank[69].slice(0, -1).concat("-");
			KeyboardHelperCar[65] = KeyboardHelperCar[65].slice(0, -1).concat("+");
			KeyboardHelperCar[68] = KeyboardHelperCar[68].slice(0, -1).concat("-");
		} else {
			KeyboardHelperTank[68] = KeyboardHelperTank[68].slice(0, -1).concat("-");
			KeyboardHelperTank[69] = KeyboardHelperTank[69].slice(0, -1).concat("+");
			KeyboardHelperCar[65] = KeyboardHelperCar[65].slice(0, -1).concat("-");
			KeyboardHelperCar[68] = KeyboardHelperCar[68].slice(0, -1).concat("+");
		}
	});
	// Switches l and r values
	remapSpeed.addEventListener("change", () => {
		var oldKHT65 = KeyboardHelperTank[65];
		KeyboardHelperTank[65] = KeyboardHelperTank[68];
		KeyboardHelperTank[68] = oldKHT65;
		var oldKHT81 = KeyboardHelperTank[81];
		KeyboardHelperTank[81] = KeyboardHelperTank[69];
		KeyboardHelperTank[69] = oldKHT81;
		var oldKHC83 = KeyboardHelperCar[83];
		KeyboardHelperCar[83] = KeyboardHelperCar[65];
		KeyboardHelperCar[65] = oldKHC83;
		var oldKHC87 = KeyboardHelperCar[87];
		KeyboardHelperCar[87] = KeyboardHelperCar[68];
		KeyboardHelperCar[68] = oldKHC87;
	});

	var prevSpeed = " ";

	// Joystick
	setInterval(function() {
		var VL, VR;
		// get joystick coordinates
		var x = getPos(100).x;
		var y = getPos(100).y;
		var minSpeed = document.getElementById("slide").value;

		// Check whether we should use car or tank drive
		if (document.getElementById("tdm").checked) {
			// Tank drive mode radio button is checked

			// convert to polar
			var r = Math.hypot(x, y);
			var t = Math.atan2(y, x) - Math.PI / 4; // theta with 45 degree rotation

			// algorithm to go from polar to VL and VR
			VL = Math.max(-1, Math.min(r * Math.cos(t) * Math.sqrt(2), 1)) * 1000;
			VR = Math.max(-1, Math.min(r * Math.sin(t) * Math.sqrt(2), 1)) * 1000;
		} else if (document.getElementById("cdm").checked) {
			// Car drive mode radio button is checked
			VL = 1000 * y; // Thrust motor
			VR = 1000 * x; // Steer motor
		} else alert("Please select one of the options.");
		
		//Fine tuning if user plugged the motors wrongly
		if (reversePolarityL.checked) {
		  VL *= -1;
		 }
		 if (reversePolarityR.checked) {
		  VR *= -1;
		 }
		 if (remapSpeed.checked) {
		  var oldVL = VL;
		  VL = VR;
		  VR = oldVL;		  
		 }

		// Apply minimum at which motors start moving
		// VL
		if (VL !== 0)
			VL = Math.round((VL / Math.abs(VL)) * minSpeed + (VL / 1000) * (1000 - minSpeed));
		// VR
		if (VR !== 0)
			VR = Math.round((VR / Math.abs(VR)) * minSpeed + (VR / 1000) * (1000 - minSpeed));

		// Convert to arduino readable values
		// lSend
		var lSend = Math.abs(VL).toString().concat("l");
		if (VL < 0) {
			lSend = lSend.concat("-");
		} else lSend = lSend.concat("+");
		// rSend
		var rSend = Math.abs(VR).toString().concat("r");
		if (VR < 0) {
			rSend = rSend.concat("-");
		} else rSend = rSend.concat("+");

		var msg = lSend.concat(rSend);
		if (msg !== prevSpeed) {
			prevSpeed = msg;
			if (VL === 0 && VR === 0) {
				sendKey("");
				return;
			}
			sendKey(msg);
		}
	}, 300);

	// Handle mouse releases
	document.addEventListener("mouseup", () => {
		sendDirections(" ");
		if (keys.length === 0) sendKey("");
	});
});
