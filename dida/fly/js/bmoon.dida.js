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
        }
    },
    
    init: function() {
        var o = bmoon.dida;

        if (o.inited) return o;
        o.inited = true;

        o.e_content = $('#bd-content');

        return o;
    },

    onready: function() {
        var o = bmoon.dida.init();

        o.bindClick();
    },
    
    bindClick: function() {
        var o = bmoon.dida.init();
        
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
