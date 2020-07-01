<?php
	if (isset($_POST['frame_base64'])) {
		$split = explode(",", $_POST['frame_base64']);
		$unencoded = base64_decode($split[1]);
		//Create the image
		file_put_contents('img.png', $unencoded);
		file_put_contents('base.txt', $_POST['frame_base64']);
	}
?>
