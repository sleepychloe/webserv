<!DOCTYPE html>
<html>
    <head>
        <meta http-equiv="content-type" content="text/html; charset=UTF-8">
        <title>Hello!</title>
    </head>
    <body>
                <h1>This is Ratatouille Server!<br></h1>

            </form>
            <form action="/test/" method="get">
            <input type="submit" value="autoindex">
            </form>

            <h3>[TEST GET METHOD]</h3>
            <a href="/fruits">Test GET images</a><br>
            <a href="/a">Test GET non existing directory</a><br>
            <a href="service.html">Test GET service.html</a><br><br>

            <form action="query.php">
            <label for="fname">Enter Your Name:</label>
            <input type="text" id="fname" name="fname"><br>
            <!-- <label for="lname">Last name:</label>
            <input type="text" id="lname" name="lname"><br><br> -->
            <input type="submit" value="submit">

			<?php
				echo "Welcome, ";
				echo $_GET["fname"];
				echo "!";
				?>
			</form>

            <h3><br>[TEST POST METHOD]</h3>
            </form>
            Test Post File <br>
            <form method="post" enctype="multipart/form-data" action="upload.php">
            <input type="file" id="myid" name="Yofile"/>
            <button>submit</button>
            </form><br>
            <div>Redirection to intra</div>
            <form action="/redirection_intra/" method="get">
              <input type="submit" value="redirection to intra">
            </form><br>
            </form>
            <div>Redirection to google</div>
            <form action="/redirection_google/" method="get">
            <input type="submit" value="redirection to google">
            </form><br>

    </body>
</html>
