var express = require('express');
var router = express.Router();
var AWS = require('aws-sdk');

AWS.config.apiVersions = { dynamodb: 'latest' }
AWS.config.update({region: 'us-east-1'});

var db = new AWS.DynamoDB.DocumentClient();
var params = {
    TableName: 'heartRate',
};

//router.get('/table-list', function(req, res, next) {
    //db.listTables(function(err, data) {
        //console.log(data.TableNames);
    //});
    //res.send('good');
//});
router.get('/hr-data', function(req, res, next) {
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
