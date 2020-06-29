<?php
	if (is_writable('serial/Serial.out')){
		//die((String)file_put_contents('serial/Serial.out', $_REQUEST['key']));
		file_put_contents('serial/Serial.out', $_REQUEST['key']);
	} else {
		usleep(25000);
		file_put_contents('serial/Serial.out', "");
	}
?>
