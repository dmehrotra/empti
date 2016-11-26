'use strict';
var express = require('express');
var request = require('request');
var app = express();
var data;
app.use('/static',express.static(__dirname + '/public'));

app.get('/', function(req,res){
	res.sendFile(__dirname + "/views/index.html");

});
app.get('/getData',function(req,res){
	request("http://128.122.151.169/arduino/analog/0",function(e,r,b){
		if (e == null){
			data = b.split('analog')[1]
		}
	})
	res.json(data)
})
app.listen(8080,function(){
	console.log('listening on port 8080');
});
