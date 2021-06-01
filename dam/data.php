<?php
include ('connection.php');
$sql_insert = "INSERT INTO data (distance,temperature,humidity,rain,flowlevel) VALUES ('".$_POST["distance"]."','".$_POST["temperature"]."', '".$_POST["humidity"]."', '".$_POST["rain"]."','".$_POST["flowlevel"]."')";
if(mysqli_query($con,$sql_insert))
{
echo "Done";
mysqli_close($con);
}
else
{
echo "error is ".mysqli_error($con );
}
?>