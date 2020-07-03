$(document).ready(setTimeout(function() {
	$("#joystick_box").append("<div id='joystick'></div>");
	$("#joystick").width("33%").height("33%");//33*$("#joystick_box").height()/100);//33*$("#joystick_box").width()/100);
	$("#joystick").css("background-color", "black").css("position", "relative");;
	$("#joystick_box").css("background-color", "white");
	$("#joystick").css("border-radius", "50%");
	updateJoystickPos();

	// Make the DIV element draggable:
	var radius = 100+0*$("#joystick_box").width()/2;
	var zoom = $('#joystick').css('zoom');
	$('#joystick').draggable({
    drag: function( event, ui ) {
			var x = ui.position.left - radius,
			y = radius - ui.position.top,
			h = Math.hypot(x, y);

			var factor = (1 / zoom) -1;
			ui.position.top += Math.round((ui.position.top - ui.originalPosition.top) * factor);
			ui.position.left += Math.round((ui.position.left- ui.originalPosition.left) * factor);

			if (Math.floor(h) > radius) {
			  ui.position.top =  radius - Math.round(radius * y / h);
			  ui.position.left = Math.round(radius * x / h) + radius;
			}
    },
	revert: true,
	revertDuration: 300,
	scroll: false
});
}, '2000'));

function updateJoystickPos() {
	console.log(1);
	$("#joystick").offset({top: Math.round(getPos(1).y - $("#joystick").height()/2), left: Math.round(getPos(1).x - $("#joystick").width()/2)});
}

function getPos(parentonly = -1){
	var joystick_box_pos_middle = {};
	joystick_box_pos_middle.y = $("#joystick_box").offset().top + $("#joystick_box").height()/2;
	joystick_box_pos_middle.x = $("#joystick_box").offset().left + $("#joystick_box").width()/2;

	if(parentonly == 1) return joystick_box_pos_middle;

  var joystick_pos_middle = {};
	joystick_pos_middle.y = $("#joystick").offset().top + $("#joystick").height()/2;
	joystick_pos_middle.x = $("#joystick").offset().left + $("#joystick").width()/2;

	var joystick_pos_middle_relative = relativate(joystick_box_pos_middle, joystick_pos_middle);

	if(parentonly == 100){
		var ret = {};
		ret.x = joystick_pos_middle_relative.x/($("#joystick_box").width()/2);
		ret.y = joystick_pos_middle_relative.y/($("#joystick_box").height()/2);
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
window.addEventListener('resize', updateJoystickPos);
