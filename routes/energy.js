var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
  res.render('energy', { title: 'Energy Breakdown - HomeAI', page_id: 1, page_link: '/energy' });
});

module.exports = router;
