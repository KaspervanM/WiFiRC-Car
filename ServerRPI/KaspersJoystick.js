var joystick_box;
var joystick;
var yPos = 0;
var xPos = 0;
var mousedown = -1;

$(document).ready(setTimeout(function() {
	joystick_box = document.getElementById("joyDiv");
	joystick = document.createElement("div");
	joystick.style.width = (33*joystick_box.clientWidth/100).toString().concat("px");
	joystick.style.height = (33*joystick_box.clientHeight/100).toString().concat("px");
	joystick.style.backgroundColor = "black";
	joystick_box.style.backgroundColor = "white";
	joystick.style.position = "absolute";
	joystick.style.borderRadius = "50%";
	joystick_box.appendChild(joystick);
	joystick.style.top = (getPos(1).y - joystick.clientHeight/2) + "px";
	joystick.style.left = (getPos(1).x - joystick.clientWidth/2) + "px";

	// Make the DIV element draggable:
	dragElement(joystick);
}, '2000'));

function dragElement(elmnt) {
	var pos1 = 0, pos2 = 0, pos3 = 0, pos4 = 0;
	elmnt.onpointerdown = dragMouseDown;

	function dragMouseDown(e) {
		e = e || window.event;
		e.preventDefault();
		// get the mouse cursor position at startup:
		pos3 = e.clientX;
		pos4 = e.clientY;
		document.onpointerup = closeDragElement;
		// call a function whenever the cursor moves:
		document.onpointermove = elementDrag;
	}

	function elementDrag(e) {
		e = e || window.event;
		e.preventDefault();
		// calculate the new cursor position:
		pos1 = pos3 - e.clientX;
		pos2 = pos4 - e.clientY;
		pos3 = e.clientX;
		pos4 = e.clientY;
		// set the element's new position:
		elmnt.style.top = (elmnt.offsetTop - pos2) + "px";
		elmnt.style.left = (elmnt.offsetLeft - pos1) + "px";

		// check if it doesnt leave container
		if (Math.hypot(getPos().x, getPos().y) > joystick_box.clientHeight/2){
			elmnt.style.top = (elmnt.offsetTop + pos2) + "px";
			elmnt.style.left = (elmnt.offsetLeft + pos1) + "px";
		}
	}

	function closeDragElement() {
		// stop moving when mouse button is released:
		document.onpointerup = null;
		document.onpointermove = null;

		elmnt.style.top = (getPos(1).y - elmnt.clientHeight/2) + "px";
		elmnt.style.left = (getPos(1).x - elmnt.clientWidth/2) + "px";
	}
}

function getPos(parentonly = -1){
	var joystick_box_pos = joystick_box.getBoundingClientRect();
	var joystick_box_pos_middle = {};
	joystick_box_pos_middle.y = joystick_box_pos.top + (joystick_box_pos.bottom - joystick_box_pos.top)/2;
	joystick_box_pos_middle.x = joystick_box_pos.left + (joystick_box_pos.right - joystick_box_pos.left)/2;

	if(parentonly == 1) return joystick_box_pos_middle;

  var joystick_pos = joystick.getBoundingClientRect();
  var joystick_pos_middle = {};
	joystick_pos_middle.y = joystick_pos.top + (joystick_pos.bottom - joystick_pos.top)/2;
	joystick_pos_middle.x = joystick_pos.left + (joystick_pos.right - joystick_pos.left)/2;

	var joystick_pos_middle_relative = relativate(joystick_box_pos_middle, joystick_pos_middle);

	if(parentonly == 100){
		var ret = {};
		ret.x = joystick_pos_middle_relative.x*joystick_box.clientHeight/100;
		ret.y = joystick_pos_middle_relative.y*joystick_box.clientHeight/100;
		return ret;
	}

	return joystick_pos_middle_relative;
}

function relativate(parentPos, childPos){
	var relativePos = {};
	relativePos.x = childPos.x - parentPos.x;
	relativePos.y = parentPos.y - childPos.y;
	return relativePos;
}
