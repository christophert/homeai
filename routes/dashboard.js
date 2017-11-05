var express = require('express');
var router = express.Router();

/* GET home page. */
router.get('/', function(req, res, next) {
    res.render('dashboard', { title: 'Dashboard - HomeAI', page_id: 1, page_link: '/dashboard' });
});

module.exports = router;
