; var g_site_domain = "dididada.org",
g_site_admin = "http://admin.dididada.org/",
g_site_www = "http://www.dididada.org/";

var bmoon = bmoon || {};
bmoon.dida = {
    version: '1.0',

    odomain: {
        1: {
            url: 'http://www.58.com',
            img: '/img/logo/58.gif',
            title: '58同城'
        },
        2: {
            url: 'http://www.ganji.com',
            img: '/img/logo/ganji.png',
            title: '赶集网'
        },
        3: {
            url: 'http://www.edeng.cn',
            img: '/img/logo/edeng.jpg',
            title: '易登网'
        },
        4: {
            url: 'http://www.liebiao.com',
            img: '/img/logo/liebiao.png',
            title: '列表网'
        }
    },
    
    init: function() {
        var o = bmoon.dida;

        if (o.inited) return o;
        o.inited = true;

        o.e_content = $('#bd-content');

        o.mnick = $('#bd-mnick');
        o.member = $('#bd-member');
        o.guest = $('#bd-guest');
        o.loginmname = $('#login-mname');
        o.loginmsn = $('#login-msn');
        o.loginoverlay = $('a[rel="#bd-login"]').overlay({
            mask: '#666', api: true,
            onLoad: function() {
                if (o.loginmname.val().length <= 0)
                    o.loginmname.focus();
                else
                    o.loginmsn.focus();
            }
        });
        
        return o;
    },

    onready: function() {
        var o = bmoon.dida.init();

        o.bindClick();
        o.loginCheck();

        o.loginref = bmoon.utl.getQueryString("loginref");
        if (o.loginref) {
            o.loginoverlay.load();
        }
        o.vikierr = bmoon.utl.getQueryString("vikierr");
        if (o.vikierr) {
            $('#content').empty().append('<div class="text-error">'+o.vikierr+'</div>')
        }
    },
    
    bindClick: function() {
        var o = bmoon.dida.init();
        
        $('#login-submit').click(o.login);
        $('#userlogout').click(o.logout);
        o.loginmsn.bind('keydown', 'return', o.login);
    },

    login: function() {
        var o = bmoon.dida.init();
        
        if (!$(".VAL_LOGIN").inputval()) return;

        var mname = o.loginmname.val(),
        msn = $.md5($.md5(o.loginmsn.val()));

        $.getJSON("/json/member/login", {mname: mname, msn: msn}, function(data) {
            if (data.success != 1 || !data.mname) {
                alert(data.errmsg || "操作失败， 请稍后再试");
                return;
            }
            o.loginoverlay.close();
            setTimeout(function() {location.href = o.loginref || location.href;}, 1000);
        });
    },
    
    logout: function() {
        var o = bmoon.dida.init();
        
        $.getJSON('/json/member/logout');
        
        $.cookie('mname', null, {path: '/', domain: g_site_domain});
        $.cookie('mnick', null, {path: '/', domain: g_site_domain});
        $.cookie('mnick_esc', null, {path: '/', domain: g_site_domain});
        $.cookie('msn', null, {path: '/', domain: g_site_domain});
        //$.cookie('mmsn', null, {path: '/', domain: g_site_domain});
        o.loginmname.val("");
        o.loginCheck();
    },
    
    loginCheck: function() {
        var o = bmoon.dida.init();
        
        var mnick = $.cookie('mnick_esc'),
        mname = $.cookie('mname');
        
        if (mnick != null) {
            o.mnick.text(mnick);
            o.guest.hide();
            o.member.show();
            o.loginmname.val(mname);
        } else {
            o.member.hide();
            o.guest.show();
        }
    },

    // "(28.228209,114.057868)" => [28.228209, 114.057868]
    dbpoint2ll: function(s) {
        var a = s.split(','),
        lat = a[0].match(/[0-9\.]+/),
        lng = a[1].match(/[0-9\.]+/),
        ret = [parseFloat(lat), parseFloat(lng)];
        return ret;
    },

    // "(28.228209,114.057868),(22.543099,112.938814)"
    // => [[22.543099,112.938814], [28.228209,114.057868]]
    dbbox2ll: function(s) {
        var a = s.split(','),
        lat1 = parseFloat(a[0].match(/[0-9\.]+/)),
        lng1 = parseFloat(a[1].match(/[0-9\.]+/)),
        lat2 = parseFloat(a[2].match(/[0-9\.]+/)),
        lng2 = parseFloat(a[3].match(/[0-9\.]+/));

        if (lat1 < lat2) {
            return [[lat1,lng1], [lat2,lng2]];
        } else {
            return [[lat2,lng2], [lat1,lng1]];
        }
    },
    
    // {address_components: [], formatted_address: "", geometry: {}...}
    getCityByGeoresult: function(res, callback) {
        var o = bmoon.dida.init();

        var arrs = res.address_components;

        for (var i = 0; i < arrs.length; i++) {
            if (arrs[i].types[1] != 'political')
                arrs.splice(i, 1);
        }

        var get = function (pos) {
            if (arrs.length == 0 || pos == arrs.length) return callback(null);
            
            var city = arrs[pos].short_name;

            $.getJSON('/json/city/s', {c: city}, function(data) {
                if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                    return callback(data.city);
                } else {
                    return get(pos+1);
                }
            });
        }

        get(0);
    }
};

$(document).ready(bmoon.dida.onready);
