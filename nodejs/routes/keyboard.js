var express = require('express');
var router = express.Router();

/* GET users listing. */
router.get('/', function(req, res, next) {
  const menu = {
    type: 'buttons',
    buttons: ["M I S A E", "C L U G", "S W  C A M P"]
  };
  res.set({
    'content-type': 'application/json'
  }).send(JSON.stringify(menu));
});

module.exports = router;
