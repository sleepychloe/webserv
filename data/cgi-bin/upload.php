<html>
	<body>
	<?php
			if (getenv("REQUEST_METHOD") == "POST") 
			{
				if (getenv('UPLOAD_ERROR')=='0')
				{
					echo (getenv("filename"));
					echo(" Uploaded Successfully");
				}
				else
					echo("Error uploading file");
			}
		?>
	</body>
</html>
