; var bmoon = bmoon || {};
bmoon.spdrobot = {
    version: '1.0',

    init: function() {
        var o = bmoon.spdrobot;
        if (o.inited) return o;

        o.e_count = $('#count');
        o.e_res = $('#res');
        
        o.ggeocode = new google.maps.Geocoder();

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.spdrobot.init();

        o.bindClick();
        o.getPlans();
    },

    bindClick: function() {
        var o = bmoon.spdrobot.init();

    },

    outClear: function() {
        var o = bmoon.spdrobot.init();

        o.e_res.empty();
    },

    outPut: function(msg) {
        var o = bmoon.spdrobot.init();

        $('<pre>' + msg + '</pre>').appendTo(o.e_res);
    },

    getPlans: function() {
        var o = bmoon.spdrobot.init();

        o.outClear();
        $.getJSON('/json/spd/post/robot', null, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.plans) == 'Array') {
                o.plans = data.plans;
                o.cur_plan = 0;
                o.e_count.html(o.plans.length)
                o.parsePlan();
            } else {
                o.outPut(data.errmsg);
                o.outPut(data.errtrace);
                setTimeout(o.getPlans, 60*1000);
            }
        });
    },

    // produce saddr, eaddr, scityid, ecityid, rect, km accord saddr, eaddr
    parsePlan: function() {
        var o = bmoon.spdrobot.init();

        if (o.cur_plan == o.plans.length) {
            o.outPut('完了，休息会儿！');
            setTimeout(o.getPlans, 60*1000);
            return;
        }

        var plan = o.plans[o.cur_plan++];
        setTimeout(o.parsePlan, 5*1000);

        o.outPut('解析 ' + plan.id + ':' + plan.saddr + ' - ' + plan.eaddr + ' ...');
        
        o.ggeocode.geocode({address: plan.saddr}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                var res = results[0];

                plan.saddr = res.formatted_address;
                plan.sll = [res.geometry.location.lat(), res.geometry.location.lng()];

                bmoon.dida.getCityByGeoresult(res, function(city) {
                    if (bmoon.utl.type(city) == 'Object') {
                        plan.scityid = city.id;
                    } else {
                        o.outPut('解析起点有错 ' + city);
                        o.parsePlanErr(plan);
                    }
                });
            } else {
                o.outPut(status);
                o.parsePlanErr(plan);
            }
            
        });

        o.ggeocode.geocode({address: plan.eaddr}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                var res = results[0];

                plan.eaddr = res.formatted_address;
                plan.ell = [res.geometry.location.lat(), res.geometry.location.lng()];

                bmoon.dida.getCityByGeoresult(res, function(city) {
                    if (bmoon.utl.type(city) == 'Object') {
                        plan.ecityid = city.id;

                        if (plan.scityid && plan.ecityid) {
                            plan.rect = '((' + plan.sll.join(',') + '),(' +
                                plan.ell.join(',') + '))';
                            plan.km = bmoon.utl.earthDis(plan.sll, plan.ell);
                            var pdata = {
                                _op: 'mod',
                                plan: JSON.stringify(plan),
                                _type_plan: 'object'
                            };
                            $.post('/json/spd/post/robot', pdata, function(data) {
                                if (data.success == 1) o.outPut('完成');
                                else o.outPut(data.errmsg);
                            }, 'json');
                        }
                    } else {
                        o.outPut('解析终点有错' + city);
                        o.parsePlanErr(plan);
                    }
                });
            } else {
                o.outPut(status);
                o.outPut('处理有错，置位...');
            }
        });
    },

    parsePlanErr: function(plan) {
        var o = bmoon.spdrobot.init();

        plan.statu = 11;
        var pdata = {
            _op: 'mod',
            plan: JSON.stringify(plan),
            _type_plan: 'object'
        };
        $.post('/json/spd/post/robot', pdata, function(data) {
            if (data.success == 1) o.outPut('处理有错，完成置位');
            else o.outPut(data.errmsg);
        }, 'json');
    }
};

$(document).ready(bmoon.spdrobot.onready);
