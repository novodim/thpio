<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
<head>
<title></title>
<meta http-equiv="refresh" content="60" >
</head>
<body>

<table>
<tr><td valign="top">
<?php

if (!isset($_GET['id'])) $_GET['id']="1d";
$mode=$_GET['id'];
?>

<a href="index.php?id=1d"><?php if($mode=="1d")echo"<b>";?>day<?php if($mode=="1d")echo"</b>";?></a><br>
<a href="index.php?id=2d"><?php if($mode=="2d")echo"<b>";?>2day<?php if($mode=="2d")echo"</b>";?></a><br>
<a href="index.php?id=3d"><?php if($mode=="3d")echo"<b>";?>3day<?php if($mode=="3d")echo"</b>";?></a><br>
<a href="index.php?id=1w"><?php if($mode=="1w")echo"<b>";?>week<?php if($mode=="1w")echo"</b>";?></a><br>
<a href="index.php?id=1m"><?php if($mode=="1m")echo"<b>";?>month<?php if($mode=="1m")echo"</b>";?></a><br>
<a href="index.php?id=3m"><?php if($mode=="3m")echo"<b>";?>3months<?php if($mode=="3m")echo"</b>";?></a><br>
<a href="index.php?id=1year"><?php if($mode=="1year")echo"<b>";?>year<?php if($mode=="1year")echo"</b>";?></a><br>

</td>
<td>
<?php

$ids=array("28aa21e903000086","pressure","28ff13026d14047e","dht-0-hum");
foreach ($ids as $sensor)
{
echo "<img src=\"".$sensor."_".$mode.".png\"><br>\n";
}
?>
</td>
</tr>
</table>
</body>
</html>

