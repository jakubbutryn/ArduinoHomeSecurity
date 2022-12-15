<?php
include_once('post-mysql-functions.php');
error_reporting(E_ERROR | E_PARSE);

if ($_GET["TurnOnAlarm"]) {
    $TurnOnAlarm = $_GET["TurnOnAlarm"];
    $json_array = array(
        "TurnOnAlarm" => $_GET["TurnOnAlarm"]
    );
    file_put_contents('TurnOnAlarm.json', json_encode($json_array));
}

if ($_GET["TurnOnHeating"]) {
    $TurnOnHeating = $_GET["TurnOnHeating"];
    $json_array = array(
        "TurnOnHeating" => $_GET["TurnOnHeating"]
    );
    file_put_contents('TurnOnHeating.json', json_encode($json_array));
}

if ($_GET["readingsCount"]) {
    $readings_count = $_GET["readingsCount"];
} else {
    $readings_count = 10;
}



$last_data = getLastData();
$last_data_temp = $last_data["value1"];
$last_data_humi = $last_data["value2"];
$last_data_time = $last_data["reading_time"];

$min_temp = minimum($readings_count, 'value1');
$max_temp = maximum($readings_count, 'value1');
$avg_temp = average($readings_count, 'value1');
$min_humi = minimum($readings_count, 'value2');
$max_humi = maximum($readings_count, 'value2');
$avg_humi = average($readings_count, 'value2');

?>

<!DOCTYPE html>
<html>

<head>
    <meta http-equiv="Content-Type" content="text/html; charset=utf-8">
    <link rel="stylesheet" type="text/css" href="style.css">
    <meta name="viewport" content="width=device-width, initial-scale=1">
</head>

<header class="header">
    <h1>Home Security</h1>
</header>

<body>

    <section class="content">
        <div>
            <h3>Actual temperature</h3>
            <p>
                <?php echo $last_data_temp; ?> °C
            </p>
            <table cellspacing="6" cellpadding="6">
                <tr>
                    <th colspan="3">Temperature from last
                        <?php echo $readings_count; ?> readings
                    </th>
                </tr>
                <tr>
                    <td>Min</td>
                    <td>Max</td>
                    <td>Average</td>
                </tr>
                <tr>
                    <td>
                        <?php echo $min_temp['min_amount']; ?> &deg;C
                    </td>
                    <td>
                        <?php echo $max_temp['max_amount']; ?> &deg;C
                    </td>
                    <td>
                        <?php echo round($avg_temp['avg_amount'], 2); ?> &deg;C
                    </td>
                </tr>
            </table>
        </div>
        <div>
            <h3>Remote Control</h3>
            <table cellspacing="6" cellpadding="6">

                <tr>
                    <td>Heating Control</td>
                </tr>
                <tr>
                    <td>
                        <form method="get">
                            <input type="text" name="TurnOnHeating"
                                placeholder="Type ON/OFF (<?php echo $TurnOnHeating; ?>)">
                            <input type="submit" value="Change">
                        </form>
                    </td>
                    </td>
                    <td>
                </tr>
                <tr>
                    <td>Alarm Control</td>
                </tr>

                <tr>
                    <td>
                        <form method="get">
                            <input type="text" name="TurnOnAlarm"
                                placeholder="Type ON/OFF (<?php echo $TurnOnAlarm; ?>)">
                            <input type="submit" value="Change">
                        </form>

                    <td>
                </tr>
                <tr>
                    <td>
                    </td>
                </tr>
            </table>
        </div>
        <div>
            <h3>Actual humidity</h3>
            <p>
                <?php echo $last_data_humi; ?> %
            </p>
            <table cellspacing="5" cellpadding="5">
                <tr>
                    <th colspan="3">Humidity from last
                        <?php echo $readings_count; ?> readings
                    </th>
                </tr>
                <tr>
                    <td>Min</td>
                    <td>Max</td>
                    <td>Average</td>
                </tr>
                <tr>
                    <td>
                        <?php echo $min_humi['min_amount']; ?> %
                    </td>
                    <td>
                        <?php echo $max_humi['max_amount']; ?> %
                    </td>
                    <td>
                        <?php echo round($avg_humi['avg_amount'], 2); ?> %
                    </td>
                </tr>
            </table>
        </div>
    </section>
    <br></br>
    <h2> Last
        <?php echo $readings_count; ?> Readings
    </h2>
    <form method="get">
        <input type="number" name="readingsCount" min="1"
            placeholder="Enter number of readings (<?php echo $readings_count; ?>)">
        <input type="submit" value="Change">
    </form>
    <table cellspacing="5" cellpadding="5" id="data">
        <tr>
            <th>ID</th>

            <th>Temperature [°C]</th>
            <th>Humidity [%]</th>
            <th>Water level</th>
            <th>Alarm status</th>
            <th>Heating status</th>
            <th>Set Temperature</th>
            <th>Time</th>
        </tr>;
        <?php
        $result = getAllData($readings_count);
        if ($result) {
            while ($row = $result->fetch_assoc()) {
                $row_id = $row["id"];
                $row_value1 = $row["value1"];
                $row_value2 = $row["value2"];
                $row_value3 = $row["value3"];
                $row_value4 = $row["value4"];
                $row_value5 = $row["value5"];
                $row_value6 = $row["value6"];
                $row_reading_time = $row["reading_time"];
                $row_reading_time = date("Y-m-d H:i:s", strtotime("$row_reading_time"));

                echo '<tr>
                    <td>' . $row_id . '</td>
                    <td>' . $row_value1 . '</td>
                    <td>' . $row_value2 . '</td>
                    <td>' . $row_value3 . '</td>
                    <td>' . $row_value4 . '</td>
                    <td>' . $row_value5 . '</td>
                    <td>' . $row_value6 . '</td>
                    <td>' . $row_reading_time . '</td>
                  </tr>';
            }
            echo '</table>';
            $result->free();
        }
        ?>
</body>

</html>