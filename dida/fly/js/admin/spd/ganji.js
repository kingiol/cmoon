; var bmoon = bmoon || {};
bmoon.spdganji = {
    version: '1.0',

    init: function() {
        var o = bmoon.spdganji;
        if (o.inited) return o;

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.spdganji.init();

        var href = location.href;

        o.area = href.match(/http:\/\/(.*).ganji.*/)[1];
        
        if (href.match(/.*\/([0-9|_]+)\.htm$/)) {
            o.parseNode(o.area + href.match(/.*\/([0-9|_]+)\.htm$/)[1]);
        } else if (href.match(/.*ganji.com\/pincheshangxiaban\//)){
            o.parseList();
        }

        setTimeout(function() {window.location.reload();}, 10*60*1000);
    },

    bindClick: function() {
        var o = bmoon.spdganji.init();
    },

    parseNode: function(id) {
        var o = bmoon.spdganji.init();

        var
        sizes = ['小轿车', '商务车', '越野车', '面包车', '客车', '货车'], model = '',
        dad = 0, saddr = '', eaddr = '', marks = '', time = '', size = 0, fee = 0,
        repeat = 0, sdate = '', stime = '', attach = '', uname = id;

        var x = $.trim($('h1', '.body_left').html()).split('→');
        if (x) {
            saddr = $.trim(x[0]);
            eaddr = $.trim(x[1]);
            if (eaddr.match(/[0-9]+元/)) {
                fee = parseInt(eaddr.match(/[0-9]+元/));
            }

            saddr.replace('-', '');
            eaddr = $.trim(eaddr.match(/[^0-9]+/)[0].replace('面议', '').replace('-', ''));
        }

        dad = $('.body_left').html().match('我是车主') ? 1: 0;

        x = $('.body_left').html().match('联系人：([^<]+)');
        if (x) uname = $.trim(x[1]);

        x = $('.body_left').html().match(/途径地点:([^\<]+)/);
        marks = x && $.trim(x[1]) || '';
        
        x = $('.body_left').html().match(/[0-9]{4}-[0-9]{2}-[0-9]{2}/);
        time = x && x[0] || '2011-12-31 08:00:00';

        sdate = repeat == 2 ? '1,2,3,4,5': time.split(' ')[0];
        stime = time.split(' ')[1] ? time.split(' ')[1]: '';

        attach = $($('.mt20', '.body_left')[0]).html().replace(/\<[^\>]+\>/g, "");

        var phone = $('img', '.body_left')[0] && $($('img', '.body_left')[0]).attr('src') || '',
        contact = $('img', '.body_left')[1] && $($('img', '.body_left')[1]).attr('src') || '',
        city = $($('.city a', '#head')[0]).html();

        if (phone) phone = 'http://' + o.area + '.ganji.com' + phone;
        if (contact) contact = 'http://' + o.area + '.ganji.com' + contact;
        
        console.log('dad ' + dad);
        console.log('uname ' + uname);
        console.log('saddr ' + saddr);
        console.log('eaddr ' + eaddr);
        console.log('fee' + fee);
        console.log('marks ' + marks);
        console.log('size ' + sizes[size]);
        console.log('repeat ' + repeat);
        console.log('sdate ' + sdate);
        console.log('stime ' + stime);
        console.log('attach ' + attach);

        console.log('phone' + phone);
        console.log('contact' + contact);
        console.log('city' + city);

        var ori = 'ganji',
        mname = id + '@ganji.com';
        
        $.getJSON(g_site_admin + 'json/spd/do?JsonCallback=?', {
            _op: 'add',
            
            member: JSON.stringify({
                mname: mname,
                ori: ori,
                city: city, // need convert to cityid
                phone: bmoon.utl.clotheHTML(phone),
                contact: bmoon.utl.clotheHTML(contact),

                size: size
            }),

            _type_member: 'object',

            plan: JSON.stringify({
                mname: mname,
                ori: ori,
                oid: id,
                ourl: location.href,
                dad: dad,
                nick: uname,
                fee: fee,
                saddr: saddr,
                eaddr: eaddr,
                marks: marks, // convert
                repeat: repeat,
                sdate: sdate,
                stime: stime,
                attach: attach
            }),
            
            _type_plan: 'object',
            
        }, function(mydata) {
            if (mydata.success == 1) {
                window.opener = null;
                window.close();
            } else {
                console.log(mydata.errmsg);
                console.log(mydata.errtrace);
            }
        });
    },
    
    parseList: function() {
        var o = bmoon.spdganji.init();

        var
        ids = [], urls = {},
        objs = $('a', '#con_one_1'),
        reg = /.*\/pincheshangxiaban\/(.*)\.htm$/;

        $.each(objs, function(i, obj) {
            if ($(obj).attr('href').match(reg)) {
                var id = o.area + $(obj).attr('href').match(reg)[1];
                ids.push(id);
                urls[id] = 'http://' + o.area + '.ganji.com' + $(obj).attr('href');
            }
        });

        $.getJSON(g_site_admin + 'json/spd/pre?JsonCallback=?',
                 {
                     ori: 'ganji',
                     oids: ids
                 }, function(data) {
                     if (data.success == 1) {
                         if (bmoon.utl.type(data.oids) == 'Object') {
                             $.each(data.oids, function(key, val) {
                                 console.log(val);
                                 setTimeout(function() {window.open(urls[val]);},
                                            Math.random()*50*1000);
                             });
                         } else {
                             console.log('dida ALL DONE');
                         }
                     } else {
                         alert(data.errmsg || '操作失败');
                     }
                 })
    }
};

$(document).ready(bmoon.spdganji.onready);
