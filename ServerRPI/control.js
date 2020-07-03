// Declare some vars
var keys = [];
var KeyboardHelper = {65 : "600l-", 81 : "600l+", 68 : "600r-", 69 : "600r+"};
document.addEventListener('keydown', keyDownHandler, false);
document.addEventListener('keyup', keyUpHandler, false);

// Update slider amount(and add it to KeyboardHelper var)
function updateSlider(slideAmount) {
  var sliderDiv = document.getElementById("sliderAmount");
  sliderDiv.innerHTML = slideAmount;
  var n = slideAmount.toString();
  KeyboardHelper = {65 : n + "l-", 81 : n + "l+", 68 : n + "r-", 69 : n + "r+"};
}

// Handle key presses
function keyDownHandler(event) {
  pressed = event.keyCode;
  if (pressed in KeyboardHelper && !keys.includes(pressed)) {
    l = 0;
    r = 0;
    keys.map(function(num) {
      if (KeyboardHelper[num].includes("l")) {
        l++;
      } else {
        r++;
      }
    });
    if ((KeyboardHelper[pressed].includes("l") && l == 0) ||
        (KeyboardHelper[pressed].includes("r") && r == 0)) {
      keys.push(pressed);
      combine();
    }
  }
}

// Handle key releases
function keyUpHandler(event) {
  index = keys.indexOf(event.keyCode);
  if (index > -1) {
    keys.splice(index, 1);
    combine();
  }
}

// Send motor instructions
function sendKey(key) {
  console.log(key);
  $.get("saver.php", {
    key : key
  }); //,function success(e){/*$(".controls").append(key).append("  -  ");*/});
}
// Send keys that where used
function combine() {
  combined = keys.map(function(num) {
                   if (num in KeyboardHelper)
                     return (KeyboardHelper[num]);
                 })
                 .join('');
  sendKey(combined);
}
$(document).ready(function() {
  // Camera button elements
  const leftCamTurnButton = document.getElementById('leftCamTurn');
  const rightCamTurnButton = document.getElementById('rightCamTurn');
  const upCamTurnButton = document.getElementById('upCamTurn');
  const downCamTurnButton = document.getElementById('downCamTurn');

  // Create JoyStick object into the DIV 'joyDiv'
  // const Joy = new JoyStick('joyDiv');

  // Send camera instructions
  function sendDirections(direction) {
    $.get("cameraSaver.php", {direction : direction}, function success(e) {});
  }

  // Camera buttons
  leftCamTurnButton.addEventListener('mousedown',
                                     () => { sendDirections('h+'); });
  rightCamTurnButton.addEventListener('mousedown',
                                      () => { sendDirections('h-'); });
  upCamTurnButton.addEventListener('mousedown',
                                   () => { sendDirections('v-'); });
  downCamTurnButton.addEventListener('mousedown',
                                     () => { sendDirections('v+'); });

  var prevSpeed = " ";

  // Joystick
  setInterval(function() {
    // get joystick coordinates
    var x = getPos().x; // Joy.GetX()/100;
    var y = getPos().y; // Joy.GetY()/100;
    var minSpeed = document.getElementById('slide').value;

    // convert to polar
    var r = Math.hypot(x, y);
    if (r > 1) {
      r = 1
    } // because it's a square joystick
    var t = Math.atan2(y, x);

    // rotate by 45 degrees
    t -= Math.PI / 4;

    // back to cartesian
    var VL = r * Math.cos(t);
    var VR = r * Math.sin(t);

    // rescale the new coords
    VL = VL * Math.sqrt(2);
    VR = VR * Math.sqrt(2);

    // clamp to -1/+1
    VL = Math.max(-1, Math.min(VL, 1)) * 1000;
    VR = Math.max(-1, Math.min(VR, 1)) * 1000;

    // Apply minimum at which motors start moving
    // VL
    if (VL != 0)
      VL = Math.round(VL / Math.abs(VL) * minSpeed +
                      VL / 1000 * (1000 - minSpeed));
    // VR
    if (VR != 0)
      VR = Math.round(VR / Math.abs(VR) * minSpeed +
                      VR / 1000 * (1000 - minSpeed));

    // Convert to arduino readable values
    // lSend
    var lSend = Math.abs(VL).toString().concat("l");
    if (VL < 0)
      lSend = lSend.concat("-");
    else
      lSend = lSend.concat("+");
    // rSend
    var rSend = Math.abs(VR).toString().concat("r");
    if (VR < 0)
      rSend = rSend.concat("-");
    else
      rSend = rSend.concat("+");

    var msg = lSend.concat(rSend);
    if (msg !== prevSpeed) {
      prevSpeed = msg;
      sendKey(msg);
    }
  }, 25);

  // Handle mouse releases
  document.addEventListener('mouseup', () => {
    sendDirections(' ');
    if (keys.length == 0)
      sendKey('');
  });
});
