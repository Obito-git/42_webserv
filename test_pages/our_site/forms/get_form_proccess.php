<?php
include('../dependencies/header.html');
echo '<h1> GET FORM </h1><br><br>';
echo "Hello ";
if (isset($_GET['fname']))
    echo $_GET['fname'];
else
    echo "(name is not set)";
echo " ";
if (isset($_GET['lname']))
    echo $_GET['lname'];
else
    echo "(surname is not set)";
include('../dependencies/footer.html');
