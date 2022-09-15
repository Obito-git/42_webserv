<?php
include('../dependencies/header.html');
echo '<h1> POST FORM </h1><br><br>';
echo "Hello ";
if (isset($_POST['fname']))
    echo $_POST['fname'];
else
    echo "(name is not set)";
echo " ";
if (isset($_POST['lname']))
    echo $_POST['lname'];
else
    echo "(surname is not set)";
include('../dependencies/footer.html');
