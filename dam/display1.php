<?php
$url=$_SERVER['REQUEST_URI'];
header("Refresh: 5; URL=$url"); // Refresh the webpage every 5 seconds
?>
<html>
<head>

    
</style>

<title>Dam Automation Data</title>
<link rel="stylesheet" href="//cdn.datatables.net/1.10.19/css/jquery.dataTables.min.css">
<link rel="stylesheet" type="text/css" href="css/link.css"/>
<script
  src="https://code.jquery.com/jquery-3.3.1.min.js"></script>
<script  src="//cdn.datatables.net/1.10.19/js/jquery.dataTables.min.js"></script>


</head>
<body background="images/dam.jpg" >
<h1  style="
    text-align: center;
">DAM AUTOMATION AND FLOOD MONITORING SYSTEM</h1> 
<table border="0" cellspacing="0" cellpadding="4" id="myTable" style="
    background-color: #fff4;font-weight: bold;font-size: 25px;text-shadow: 0 0 5px dcdcdc, 0 0 5px dcdcdc;
    color: #0c0c0c;
"   >
    <tbody style="
    background-color: #fff4;
">
<tr style="
    background-color: #fff4;text-align: center;
">
<td class="table_titles">ID</td>
<td class="table_titles">DATE AND TIME</td>
<td class="table_titles">WATER LEVEL</td>
<td class="table_titles">TEMPERATURE</td>
<td class="table_titles">HUMIDITY</td>
<td class="table_titles">RAIN</td>
<td class="table_titles">FLOW LEVEL</td>
</tr>
<tr style="
    background-color: #fff4;text-align: center;
">
<td class="table_titles"><img src="images/id1.png" alt="id1.png" style="width:60px;height:60px;"></td>
<td class="table_titles"><img src="images/date.png" alt="date.png" style="width:60px;height:60px;"></td>
<td class="table_titles"><img src="images/waterlevel.png" alt="waterlevel.png" style="width:120px;height:100px;"></td>
<td class="table_titles"><img src="images/temperature.png" alt="temperature.png" style="width:80px;height:60px;"></td>
<td class="table_titles"><img src="images/humidity.png" alt="humidity.png" style="width:90px;height:70px;"></td>
<td class="table_titles"><img src="images/rain1.png" alt="rain1.png" style="width:80px;height:80px;"></td>
<td class="table_titles"><img src="images/flowlevel.png" alt="flowlevel.png" style="width:120px;height:120px;"></td>
</tr>
<?php
include('connection.php');
$result = mysqli_query($con,'SELECT * FROM data ORDER BY id DESC');
// Process every record

while($row = mysqli_fetch_array($result))
{
 
echo "<tr style='
    background-color: #fff4;text-align: center;
'>";
echo "<td >" . $row['id'] . "</td>";
echo "<td >" . $row['event'] . "</td>";
echo "<td >" . $row['distance'] . "</td>";
echo "<td >" . $row['temperature'] . "</td>";
echo "<td >" . $row['humidity'] . "</td>";
$rain =$row['rain'];
if($rain<300){
    $v = 'Heavy rain';
}
elseif ($rain<500) {
    $v = 'Moderate rain';
}
else  {
    $v= 'No rain';
}
echo "<td >" . $v . "</td>";
echo "<td >" . $row['flowlevel'] . "</td>";
echo "</tr>"; 
}
 
// Close the connection
mysqli_close($con);
?>
</tbody>
</table>

<script>
$(document).ready( function () {
    $('#myTable').DataTable();
} );
</script>
</body>
</html>