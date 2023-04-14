<?php
/*
  Sourdough Starter Monitoring Station
  Post data from ESP8266 to a MySQL database
*/

// Database settings
$servername = "localhost";
$dbname = "Starter";
$username = "admin";
$password = "XXXXXXXXX";

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
