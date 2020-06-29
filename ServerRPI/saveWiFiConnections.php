<?php
	if(isset($_POST)){
		echo (String)file_put_contents('WiFIConnections.json', json_encode(array_values($_POST)));
		file_put_contents('serial/WiFIConnections.out', "");
	}
?>
