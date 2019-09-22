var express = require('express');
var router = express.Router();

const dgram = require('dgram');
const server = dgram.createSocket('udp4');
var json_obj = null

server.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on('message', (msg, rinfo) => {
  console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  json_obj = JSON.parse(msg);
});

server.on('listening', () => {
  const address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
});

server.bind(5000,"");
// server listening 0.0.0.0:41234

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('index', {data:json_obj});
});

router.get('/api', function(req, res) {
  //console.log("Get data: " + json_obj);
  return res.send(JSON.stringify(json_obj));
});
module.exports = router;
