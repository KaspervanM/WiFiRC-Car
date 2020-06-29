<?php
	if (isset($_POST['dist']))
		$fp = fopen('dist.html', 'w');
		fwrite($fp, $_POST['dist']);
		fclose($fp);
?>
