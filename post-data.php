<?php
/*
  Sourdough Starter Monitoring Station
  Post data from ESP8266 to a MySQL database
*/

// Database settings
$servername = "localhost";
$dbname = "Starter";
$username = "admin";
$password = "XXX";

$value1 = $value2 = $value3 = $value4 = $value5 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
    $value1 = test_input($_POST["value1"]);
    $value2 = test_input($_POST["value2"]);
    $value3 = test_input($_POST["value3"]);
    $value4 = test_input($_POST["value4"]);
    $value5 = test_input($_POST["value5"]);

    // Create connection
    $conn = new mysqli($servername, $username, $password, $dbname);
    // Check connection
    if ($conn->connect_error) {
        die("Connection failed: " . $conn->connect_error);
    }
    // Check if the table exists, and if not, create it
	$table_check = "SHOW TABLES LIKE 'Sensor'";
	$result = $conn->query($table_check);
	if (!$result) {
	    die("Query failed: " . $conn->error);
	}

	if ($result->num_rows == 0) {
	  $create_table = "CREATE TABLE Sensor (
        id INT(6) UNSIGNED AUTO_INCREMENT PRIMARY KEY,
        value1 VARCHAR(30) NOT NULL,
        value2 VARCHAR(30) NOT NULL,
        value3 VARCHAR(30) NOT NULL,
        value4 VARCHAR(30) NOT NULL,
        value5 VARCHAR(30) NOT NULL,
        reading_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP ON UPDATE CURRENT_TIMESTAMP
	)";


	  if ($conn->query($create_table) === TRUE) {
	    echo "Table 'weather_data' created successfully";
	  } else {
	    echo "Error creating table: " . $conn->error;
	  }
	}
  
   
  
    // Insert data into the 'Sensor' table
    $sql = "INSERT INTO Sensor (value1, value2, value3, value4, value5) VALUES ('" . $value1 . "', '" . $value2 . "', '" . $value3 . "', '" . $value4 . "', '" . $value5 . "')";

    if ($conn->query($sql) === TRUE) {
        echo "New record created successfully";
    }
    else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }

    $conn->close();
}
else {
    echo "No data posted with HTTP POST.";
}

function test_input($data) {
    $data = trim($data);
    $data = stripslashes($data);
    $data = htmlspecialchars($data);
    return $data;
}
?>
