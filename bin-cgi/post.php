#!/usr/bin/php
<?php

// Set Content-Type header
echo "Content-Type: text/html\r\n\r\n";

// Read input from stdin
$stdin = fopen("php://stdin", "r");
$data = fgets($stdin);
fclose($stdin);

// Parse input data (assuming it's in the format "name=Brian&email=Brian%40call.com")
parse_str($data, $params);

// Extract name and email from parameters
$name = isset($params['name']) ? $params['name'] : '';
$email = isset($params['email']) ? urldecode($params['email']) : '';


// HTML output
$html = <<<HTML
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title>Space Exploration</title>
</head>
<body>
    <h1>Hello, $name!</h1>
    <p>We are thrilled to have you interested in space exploration. Here are some fascinating facts about space:</p>
    <ul>
        <li>The universe is vast and ever-expanding.</li>
        <li>There are billions of galaxies, each containing billions of stars.</li>
        <li>Space exploration has led to many technological advancements.</li>
    </ul>
    <p>Thank you for your interest, $name! We'll keep you updated on the latest discoveries.</p>
    <p>Feel free to reach out to us at $email if you have any questions.</p>
</body>
</html>
HTML;

// Output HTML
echo $html;
?>

