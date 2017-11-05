var express = require('express');
var router = express.Router();

var AWS = require('aws-sdk');

AWS.config.apiVersions = { dynamodb: 'latest' }
AWS.config.update({region: 'us-east-1'});

var db = new AWS.DynamoDB.DocumentClient();
var params = {
    TableName: 'heartRate',
};

/* GET home page. */
router.get('/', function(req, res, next) {
    var hrDataFull = [];
    var hrDataLabels = [], hrData = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                hrDataLabels.push(element.time);
                hrData.push(element.hr);
                hrDataFull.push(element);
            });
            res.render('dashboard', { hr: hrDataFull });
        }
    });

});

module.exports = router;
