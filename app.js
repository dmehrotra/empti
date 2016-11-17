'use strict';
var express = require('express');
var request = require('request');
var app = express();
app.get('/test', function(req,res){
	res.send("hello world")
});
app.listen(8080,function(){
	console.log('listening on port 8080');
});