'use strict';
var express = require('express');
var request = require('request');
var app = express();

app.use('/static',express.static(__dirname + '/public'));

app.get('/', function(req,res){
	res.sendFile(__dirname + "/views/index.html");
});

app.listen(8080,function(){
	console.log('listening on port 8080');
});