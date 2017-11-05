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
    var hrDataFull = [], ocDataFull = [], statDataFull = [];
    db.scan(params, function(err, data) {
        if(err) {
            console.log("Error", err);
        } else {
            data.Items.forEach(function(element, index, array) {
                hrDataFull.push(JSON.stringify(element));
            });

            params.TableName = 'occupancy';
            db.scan(params, function(err, data) {
                if(err) {
                    console.log("Error occupancy", err);
                } else {
                    data.Items.forEach(function(element, index, array) {
                        ocDataFull.push(JSON.stringify(element));
                    });

                    params.TableName = 'predictions';
                    db.scan(params, function(err, data) {
                        if(err) {
                            console.log("Error predictions", err);
                        } else {
                            data.Items.forEach(function(element, index, array) {
                                statDataFull.push(JSON.stringify(element));
                            });
                            res.render('dashboard', { hr: hrDataFull, oc: ocDataFull, stat: statDataFull });
                        }
                    });
                }
            });
        }
    });

});

module.exports = router;
