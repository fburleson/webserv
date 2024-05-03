#!/usr/bin/env python3

print("Content-Type: text/html\n\n")

# HTML content of the page
print("""
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>Beautiful CGI Page</title>
<style>
  body {
    font-family: Arial, sans-serif;
    background-color: #f4f4f4;
    margin: 0;
    padding: 0;
  }
  .container {
    max-width: 800px;
    margin: 50px auto;
    padding: 20px;
    background-color: #fff;
    border-radius: 8px;
    box-shadow: 0 0 10px rgba(0, 0, 0, 0.1);
  }
  h1 {
    color: #333;
    text-align: center;
  }
  p {
    color: #666;
    line-height: 1.6;
  }
</style>
</head>
<body>
  <div class="container">
    <h1>Welcome to My Beautiful CGI Page</h1>
    <p>This is a simple CGI script written in Python, generating a beautiful HTML page.</p>
    <p>It's running on a web server and generating this HTML dynamically.</p>
  </div>
</body>
</html>
""")

