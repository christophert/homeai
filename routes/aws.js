var express = require('express');
var router = express.Router();
var AWS = require('aws-sdk');

AWS.config.apiVersions = { dynamodb: 'latest' }
AWS.config.update({region: 'us-east-1'});

var db = new AWS.DynamoDB.DocumentClient();


router.get('/hr-data', function(req, res, next) {
    var params = {
        TableName: 'heartRate',
    };
    var DataArray = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                DataArray.push(element);
            });
            res.send(DataArray);
        }
    });
});

router.get('/oc-data', function(req, res, next) {
    var params = {
        TableName: 'occupancy',
    };
    var DataArray = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                DataArray.push(element);
            });
            res.send(DataArray);
        }
    });
});

router.get('/st-data', function(req, res, next) {
    var params = {
        TableName: 'stats',
    };
    var DataArray = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                DataArray.push(element);
            });
            res.send(DataArray);
        }
    });
});

router.get('/pr-data', function(req, res, next) {
    var params = {
        TableName: 'predictions',
    };
    var DataArray = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                DataArray.push(element);
            });
            res.send(DataArray);
        }
    });
});
module.exports = router;
