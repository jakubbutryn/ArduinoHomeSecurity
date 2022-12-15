<?php
$servername = "localhost";
$databasename = "s403062";
$username = "root";
$password = "";

$value1 = $value2 = $value3 = $value4 = $value5 = $value6 = "";

if ($_SERVER["REQUEST_METHOD"] == "POST") {
  $value1 = ($_POST["value1"]);
  $value2 = ($_POST["value2"]);
  $value3 = ($_POST["value3"]);
  $value4 = ($_POST["value4"]);
  $value5 = ($_POST["value5"]);
  $value6 = ($_POST["value6"]);




  $result = insertData($value1, $value2, $value3, $value4, $value5, $value6);
  echo $result;

  if ($value4 == "Disarming") {

    $json_array = array(
      "TurnOnAlarm" => "OFF"
    );

    file_put_contents('TurnOnAlarm.json', json_encode($json_array));


  }
  if ($value4 == "ManuallyActivatingAlarm") {

    $json_array = array(
      "TurnOnAlarm" => "ON"
    );

    file_put_contents('TurnOnAlarm.json', json_encode($json_array));


  }


}

function insertData($value1, $value2, $value3, $value4, $value5, $value6)
{
  global $servername, $username, $password, $databasename;

  $connect = new mysqli($servername, $username, $password, $databasename);
  if ($connect->connect_error) {
    exit("Connection failed: " . $connect->connect_error);
  }

  $sql = "INSERT INTO sensordata (value1, value2, value3, value4, value5, value6)
    VALUES ('" . $value1 . "', '" . $value2 . "', '" . $value3 . "', '" . $value4 . "','" . $value5 . "','" . $value6 . "')";

  if ($connect->query($sql) === TRUE) {
    $connect->close();
    return "New record created successfully";
  } else {
    $connect->close();
    return "Error: " . $sql . "<br>" . $connect->error;
  }

}


function getAllData($limit)
{
  global $servername, $username, $password, $databasename;


  $connection = new mysqli($servername, $username, $password, $databasename);

  if ($connection->connect_error) {
    exit("Connection failed: " . $connection->connect_error);
  }

  $sql = "SELECT id, value1, value2, value3, value4, value5, value6, reading_time FROM sensordata order by reading_time desc limit " . $limit;
  if ($result = $connection->query($sql)) {
    $connection->close();
    return $result;
  } else {
    $connection->close();
    return false;
  }
}
function getLastData()
{
  global $servername, $username, $password, $databasename;


  $connection = new mysqli($servername, $username, $password, $databasename);

  if ($connection->connect_error) {
    $connection->close();
    exit("Connection failed with error: " . $connection->connect_error);
  }

  $sql = "SELECT id, value1, value2, value3, value4, value5, value6, reading_time FROM sensordata order by reading_time desc limit 1";
  if ($result = $connection->query($sql)) {
    $connection->close();
    return $result->fetch_assoc();
  } else {
    $connection->close();
    return false;
  }

}

function minimum($limit, $value)
{
  global $servername, $username, $password, $databasename;
  $connection = new mysqli($servername, $username, $password, $databasename);
  if ($connection->connect_error) {
    $connection->close();
    exit("Connection failed with error: " . $connection->connect_error);
  }
  $sql = "SELECT MIN(" . $value . ") AS min_amount FROM (SELECT " . $value . " FROM sensordata order by reading_time desc limit " . $limit . ") AS min";
  if ($result = $connection->query($sql)) {
    $connection->close();
    return $result->fetch_assoc();
  } else {
    $connection->close();
    return false;
  }
}

function maximum($limit, $value)
{
  global $servername, $username, $password, $databasename;


  $connection = new mysqli($servername, $username, $password, $databasename);

  if ($connection->connect_error) {
    exit("Connection failed with error: " . $connection->connect_error);
  }

  $sql = "SELECT MAX(" . $value . ") AS max_amount FROM (SELECT " . $value . " FROM sensordata order by reading_time desc limit " . $limit . ") AS max";
  if ($result = $connection->query($sql)) {
    $connection->close();
    return $result->fetch_assoc();
  } else {
    $connection->close();
    return false;

  }

}

function average($limit, $value)
{
  global $servername, $username, $password, $databasename;


  $connection = new mysqli($servername, $username, $password, $databasename);

  if ($connection->connect_error) {
    $connection->close();
    exit("Connection failed with error: " . $connection->connect_error);

  }

  $sql = "SELECT AVG(" . $value . ") AS avg_amount FROM (SELECT " . $value . " FROM sensordata order by reading_time desc limit " . $limit . ") AS avg";
  if ($result = $connection->query($sql)) {
    $connection->close();
    return $result->fetch_assoc();

  } else {
    $connection->close();
    return false;

  }

}




?>