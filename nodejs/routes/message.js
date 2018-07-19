var express = require('express');
var router = express.Router();

// var data = "아직 불러오기 전 입니다."
// var url = 'http://api.openweathermap.org/data/2.5/weather?q=Seoul,KR&id=524901&APPID=f2a0cf48bc0853e78596b08c22c43bb4&units=metric';

router.post('/', function(req, res, next) {
    const object = {
        user_key: req.body.user_key,
        type: req.body.type,
        content: req.body.content
    };
    const menu = {
        type: 'buttons',
        buttons: ["M I S A E", "C L U G", "S W  C A M P"]
    };
    var res_object;
    if(object.type == "text")
    {
        if(object.content == "M I S A E") {
            res_object = {
                "message": {
                    "text": '서울특별시 동작구 흑석동 흑석로 84\n--------------------------------------------------------\n미세먼지량 : ? ㎍/㎥\n온도 : ? ℃\n습도 : ? %'
                },
                "keyboard": menu
            };
        }
        else if(object.content == "C L U G") {
            res_object = {
                "message": {
                    "photo": {
                        "url": "https://clug.kr/wp-content/uploads/2015/03/Logo1.png",
                        "width": 400,
                        "height": 300
                    },
                },
                "keyboard": menu
            };
        }
        else if(object.content == "S W  C A M P") {
            res_object = {
                "message": {
                    "photo": {
                        "url": "http://upload2.inven.co.kr/upload/2017/10/14/bbs/i14369649756.jpg",
                        "width": 800,
                        "height": 600
                    }
                },
                "keyboard": menu
            };
        }
        else {
            res_object = {
                "message": {
                    "text": object.content
                },
                "keyboard": menu
            };
        }
    }
    res.set({
        'content-type': 'application/json'
    }).send(JSON.stringify(res_object));
});

module.exports = router;
