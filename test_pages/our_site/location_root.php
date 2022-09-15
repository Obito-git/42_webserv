<?php include('dependencies/header.html'); ?>
    <p>  In this test browser will send a request to the server with <br>
    not existing image (42.png) in root folder. But in config root folder for this url will be /images folder. </p>
    <img src="/42.png" class="rounded float-center img-thumbnail img-fluid" alt="42" style="width:50%">
<?php include('dependencies/footer.html'); ?>