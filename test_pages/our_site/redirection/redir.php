<?php
include('../dependencies/header.html');
echo "<br><br> <h2>Hello, it's redirection test page </h2>";
echo "<br> <p> Followed link will try to connect to not existing page, but it will be redirected to autoindex_ok page </p>";
echo "<a href=\"/not_exist\"> not existing page </a>";
include('../dependencies/footer.html');
