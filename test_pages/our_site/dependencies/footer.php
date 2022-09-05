<?php
echo "<br>";
echo "<br>";
echo "<h2>Environment variables transferred in execve:</h2>";
    echo getenv('SHELL');
    foreach (getenv() as $key => $value) {
        echo "<b>" . $key . "</b>" . ' - ' . $value;
        echo "<br>";
    }
?>

<script src="https://cdn.jsdelivr.net/npm/bootstrap@5.2.0/dist/js/bootstrap.bundle.min.js" integrity="sha384-A3rJD856KowSb7dwlZdYEkO39Gagi7vIsF0jrRAoQmDKKtQBHUuLZ9AsSv4jD4Xa" crossorigin="anonymous"></script>
</body>
</html>
