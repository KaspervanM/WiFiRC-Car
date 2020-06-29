$( document ).ready(function() {
	const liveStream = document.getElementById('liveStream');
	var noimg = false;
	var fps = 10;

	function displayFrame(timestamp){
		setTimeout(function() {
			//$.get('img.png')
		    //.done(function() {
		        liveStream.src = 'img.png'//?' + Math.floor(Math.random() * 9999 + 1000);
						if (!noimg) noimg = false;
						if (fps != 10) fps = 10;
		   // }).fail(function() {
						//if (!noimg){
						//	liveStream.src = 'noimg.png';
						//	noimg = true;
					//	}
		       // if (fps == 10) fps = 1/5;
		    //})
			window.requestAnimationFrame(displayFrame);
		}, 1000 / fps); // fps
	}
	window.requestAnimationFrame(displayFrame);
});
