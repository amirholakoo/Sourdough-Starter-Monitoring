<?php
/*
  Sourdough Starter Monitoring Station
  Display sensor data in a chart
*/

$servername = "localhost";
$dbname = "Starter";
$username = "admin";
$password = "XXXXXXXX";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
}

$sql = "SELECT id, value1, value2, value3, value4, value5, reading_time FROM Sensor order by reading_time desc limit 40";

$result = $conn->query($sql);

while ($data = $result->fetch_assoc()){
    $sensor_data[] = $data;
}

$readings_time = array_column($sensor_data, 'reading_time');

$value1 = json_encode(array_reverse(array_column($sensor_data, 'value1')), JSON_NUMERIC_CHECK);
$value2 = json_encode(array_reverse(array_column($sensor_data, 'value2')), JSON_NUMERIC_CHECK);
$value3 = json_encode(array_reverse(array_column($sensor_data, 'value3')), JSON_NUMERIC_CHECK);
$value4 = json_encode(array_reverse(array_column($sensor_data, 'value4')), JSON_NUMERIC_CHECK);
$value5 = json_encode(array_reverse(array_column($sensor_data, 'value5')), JSON_NUMERIC_CHECK);

$reading_time = json_encode(array_reverse($readings_time), JSON_NUMERIC_CHECK);

$result->free();
$conn->close();
?>

<!DOCTYPE html>
<html>
<meta name="viewport" content="width=device-width, initial-scale=1">
    <script src="https://code.highcharts.com/highcharts.js"></script>
  <style>
    body {
      min-width: 310px;
      max-width: 1280px;
      height: 500px;
      margin: 0 auto;
    }
    h2 {
      font-family: Arial;
      font-size: 2.5rem;
      text-align: center;
    }
  </style>
  <body>
    <h2>Sourdough Starter Monitoring Station</h2>
    <div id="chart-temperature" class="container"></div>
    <div id="chart-humidity" class="container"></div>
    <div id="chart-CO2" class="container"></div>
    <div id="chart-Al" class="container"></div>
    <div id="chart-RF" class="container"></div>
<script>
    var value1 = <?php echo $value1; ?>;
    var value2 = <?php echo $value2; ?>;
    var value3 = <?php echo $value3; ?>;
    var value4 = <?php echo $value4; ?>;
    var value5 = <?php echo $value5; ?>;

    var reading_time = <?php echo $reading_time; ?>;

    // Temperature chart
    var chartT = new Highcharts.Chart({
      chart:{ renderTo : 'chart-temperature' },
      title: { text: 'DHT11 Temperature' },
      series: [{
        showInLegend: false,
        data: value1
      }],
      plotOptions: {
        line: { animation: false,
          dataLabels: { enabled: true }
        },
        series: { color: '#059e8a' }
      },
      xAxis: {
        type: 'datetime',
        categories: reading_time
      },
      yAxis: {
        title: { text: 'Temperature (Celsius)' }
      },
      credits: { enabled: false }
    });

    // Humidity chart
    var chartH = new Highcharts.Chart({
      chart:{ renderTo:'chart-humidity' },
      title: { text: 'DHT11 Humidity' },
      series: [{
        showInLegend: false,
        data: value2
      }],
      plotOptions: {
        line: { animation: false,
          dataLabels: { enabled: true }
        }
      },
      xAxis: {
        type: 'datetime',
        categories: reading_time
      },
      yAxis: {
        title: { text: 'Humidity (%)' }
      },
      credits: { enabled: false }
    });

    // CO2 chart
    var chartCO = new Highcharts.Chart({
      chart:{ renderTo:'chart-CO2' },
      title: { text: 'CO2 Level' },
      series: [{
        showInLegend: false,
        data: value3
      }],
      plotOptions: {
        line: { animation: false,
          dataLabels: { enabled: true }
        },
        series: { color: '#18009c' }
      },
      xAxis: {
        type: 'datetime',
        categories: reading_time
      },
      yAxis: {
        title: { text: 'ppm' }
      },
      credits: { enabled: false }
    });

    // Alcohol Level chart
    var chartAL = new Highcharts.Chart({
      chart:{ renderTo:'chart-Al' },
      title: { text: 'Alcohol Level' },
      series: [{
        showInLegend: false,
        data: value4
      }],
            plotOptions: {
        line: { animation: false,
          dataLabels: { enabled: true }
        },
        series: { color: '#18009c' }
      },
      xAxis: {
        type: 'datetime',
        categories: reading_time
      },
      yAxis: {
        title: { text: '%' }
      },
      credits: { enabled: false }
    });

    // Raise or Fall % chart
    var chartRF = new Highcharts.Chart({
      chart:{ renderTo:'chart-RF' },
      title: { text: 'Raise or Fall %' },
      series: [{
        showInLegend: false,
        data: value5
      }],
      plotOptions: {
        line: { animation: false,
          dataLabels: { enabled: true }
        },
        series: { color: '#18009c' }
      },
      xAxis: {
        type: 'datetime',
        categories: reading_time
      },
      yAxis: {
        title: { text: '%' }
      },
      credits: { enabled: false }
    });

</script>
</body>
</html>


