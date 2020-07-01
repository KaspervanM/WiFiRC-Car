<?php
	function encrypt($pass, $key){
		$Lpass = "";
	  $j = $currKeyDigit = 0;
	  while ($j < strlen($pass)) {
	    $c = $pass[$j];
	    $p = ord($c) + ord($key[$currKeyDigit]);
	    while ($p > 126) $p -= 93;
	    $_p = chr($p);
	    $Lpass[$j] = $_p;
	    $j++;
		  $currKeyDigit = ($currKeyDigit == strlen($key) - 1 ? 0 : $currKeyDigit + 1);
	  }
	  return $Lpass;
	}

	$JSONFile = file_get_contents('WiFiConnections.json');
	$JSONArray = json_decode($JSONFile, true);

	if (isset($_POST['rem'])){
		file_put_contents('serial/WiFIConnections.out', "r " . $_POST['rem']);
		sleep(5);
		header("Refresh:0");
	}else if (isset($_POST['add'])){
		file_put_contents('serial/WiFIConnections.out', "a " . $_POST['n'] . " " . encrypt($_POST['p'], "ThisIsTheKey"));
		sleep(5);
		header("Refresh:0");
	}else if (isset($_POST['con'])){
		file_put_contents('serial/WiFIConnections.out', "c");
		file_put_contents('WiFiConnections.json', "[\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\",\"\"]");
		sleep(5);
		header("Refresh:0");
	}
	file_put_contents('serial/WiFIConnections.out', "");
?>

<html>
	<head></head>
	<body>
  	<h1>Wifi connections</h1>
		<?php
		if ($JSONArray[0] != "") {
	    echo "<p>You are connected through the soft AP: " . $JSONArray[0] . "</p><br>";
	    echo "<table><tr><th align='left'>Wifi list</th></tr>";
		    for ($i = 1; $i < sizeof($JSONArray); $i++) {
					if ($JSONArray[$i] != "") echo "<tr><td><form method='POST' action=''><input type='hidden' name='rem' value='". $JSONArray[$i] ."'/><input type='submit' value='X'/>SSID " . $JSONArray[$i] . "</form></td></tr>";
		    }
		?>
    </table>
		<button onclick="window.location.href=window.location.href; return false;">Refresh list</button>
		<form method='POST' onSubmit="return confirm('Are you sure you wish to continue? The device <?php if ($JSONArray[1] == "") echo "will"; else echo "might"; ?> be unable to reconnect.');" action=''>
			<br>To reconnect to WiFi<br><input type='submit' name='con' value='Restart Arduino'/>
		</form>
    <br>
		<form method='POST' action=''>
			<h4>Add network:</h4>
	    <input type='text' placeholder='network' name='n' required/>
	    <br><input type='password' placeholder='password' name='p'/>
	    <br><input type='submit' name="add" value='Add'/>
		</form>
		<?php
			} else {
				echo "<pre>No connection has been made, please check the captive portal for more instructions</pre>";
				if (isset($_POST['rem'])){
					echo "<p>Your change is being processed, come back later to see if it has been updated.</p>";
				}
			}
		?>
    <p>You may want to <a href='/'>return to the home page</a>.</p>
	</body>
</html>
