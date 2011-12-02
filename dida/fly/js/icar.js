; var bmoon = bmoon || {};
bmoon.icar = {
    version: '1.0',

    _formAddr: function(name, addrs, loc) {
        if (addrs) {
            return '<div><strong>' + name + '</strong><br />' +
                [(addrs[0] &&
                  addrs[0].short_name || ''),
                 (addrs[1] &&
                  addrs[1].short_name || ''),
                 (addrs[2] &&
                  addrs[2].short_name || '')
                ].join(' ') +
                '（' + loc.lat().toFixed(4) + ', ' + loc.lng().toFixed(4) + '）';
        } else return ' ';
    },

    init: function() {
        var o = bmoon.icar;
        if (o.inited) return o;

        o.e_num = $('#num');
        o.e_hour = $('#hour');
        o.e_min = $('#min');
        o.e_day = $('#day');
        o.e_dateinput = $('#date-input');
        o.e_datehint = $('#date-hint');

        o.e_saddr = $('#saddr');
        o.e_eaddr = $('#eaddr');

        o.e_submit = $('#submit');

        o.plan = {};

        o.inited = true;
        return o;
    },
    
    initMap: function(lat, lng) {
        var o = bmoon.icar.init();

        lat = parseFloat(lat) || 28.188;
        lng = parseFloat(lng) || 113.033;
        o.g_lat = new google.maps.LatLng(lat, lng);
        o.g_map = new google.maps.Map($('#map')[0], {
            zoom: 10,
            center: o.g_lat,
            mapTypeId: google.maps.MapTypeId.ROADMAP,
            region: 'zh-CN'
        });

        o.g_sauto = new google.maps.places.Autocomplete($('#saddr')[0]);
        o.g_eauto = new google.maps.places.Autocomplete($('#eaddr')[0]);
        o.g_geocode = new google.maps.Geocoder();
        o.g_dirservice = new google.maps.DirectionsService();
        o.g_dirrender = new google.maps.DirectionsRenderer();
        o.g_infow = new google.maps.InfoWindow();
        o.g_smarker = new google.maps.Marker({
            map: o.g_map,
            draggable: true,
            title: '设置起始位置'
        });
        o.g_emarker = new google.maps.Marker({
            map: o.g_map,
            draggable: true,
            title: '设置终点位置'
        });

        o.g_sauto.bindTo('bounds', o.g_map);
        o.g_eauto.bindTo('bounds', o.g_map);
        o.g_dirrender.setMap(o.g_map);
        o.bindMapChange();
    },

    onready: function() {
        var o = bmoon.icar.init();

        $.tools.dateinput.localize("zh",  {
            months:        '一月,二月,三月,四月,五月,六月,七月,八月,九月,十月,十一月,十二月',
            shortMonths:   '一,二,三,四,五,六,七,八,九,十,十一,十二',
            days:          '星期日,星期一,星期二,星期三,星期四,星期五,星期六',
            shortDays:     '日,一,二,三,四,五,六'
        });
        
        o.calendar = o.e_dateinput.dateinput({
            format: 'yyyy-mm-dd',
            lang: 'zh',
            trigger: true,
            firstDay: 1,
            change: function(e, date) {
                o.e_datehint.html(this.getValue('dddd'))
            }
        });
        
        $('.bot', o.e_timestamp).html(o.e_hour.val() + ':' + o.e_min.val());
        
        //$.getJSON('/json/city/ip', {ip: '118.145.22.78'}, function(data) {
        $.getJSON('/json/city/ip', null, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                var ts = data.city.geopos.split(','),
                geopos = [ts[0].match(/[0-9\.]+/), ts[1].match(/[0-9\.]+/)];
                o.initMap(geopos[0], geopos[1]);
            } else o.initMap();
        });

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.icar.init();

        o.e_hour.change(o.hourChanged);
        o.e_min.change(o.minChanged);
        o.calendar.bind("onShow onHide", function()  {
	        $(this).parent().toggleClass("active"); 
        });

        o.e_submit.click(o.matchPlan);
    },

    hourChanged: function() {
        var o = bmoon.icar.init();

        $('.bot', o.e_timestamp).html(o.e_hour.val() + ':' + o.e_min.val());
    },

    minChanged: function() {
        var o = bmoon.icar.init();

        $('.bot', o.e_timestamp).html(o.e_hour.val() + ':' + o.e_min.val());
    },

    matchPlan: function() {
        var o = bmoon.icar.init();

        var p = o.plan;
        
        if (!p.sll) {
            o.e_saddr.focus();
            return;
        }
        if (!p.ell) {
            o.e_eaddr.focus();
            return;
        }

        p.dad = 1;
        
        p.num  = o.e_num.val();
        p.date = o.e_dateinput.val();
        //o.dayow = xxx;
        p.time = o.e_hour.val() + ':' + o.e_min.val();
        p.day  = o.e_day.val();
        p.rect = '((' + p.sll.join(',') + '),(' + p.ell.join(',') + '))';
        //p.sll = [212.12, 232.33];
        //p.ell = [212.12, 232.33];
        //p.saddr = ... p.scity = ..
        //p.eaddr = ... p.ecity = ..
        //p.scityid = x p.ecityid = x

        $.getJSON('/json/plan/match', p, function(data) {
        });
    },

    // {address_components: [], formatted_address: "", geometry: {}...} gdata.js
    upPlan: function(x, data) {
        var o = bmoon.icar.init();

        var p = o.plan,
        addrs = data.address_components,
        city = '';

        for (var i = 0; i < addrs.length; i++) {
            if (addrs[i].types[1] == 'political') {
                city = addrs[i].short_name;
                break;
            }
        }

        if (x != 'e') {
            p.sll = [data.geometry.location.lat(), data.geometry.location.lng()];
            p.saddr = data.formatted_address;
            p.scity = city;
            $.getJSON('/json/city/s', {c: city}, function(data) {
                if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                    p.scityid = data.city.id;
                }
            });
        } else {
            p.ell = [data.geometry.location.lat(), data.geometry.location.lng()];
            p.eaddr = data.formatted_address;
            p.ecity = city;
            $.getJSON('/json/city/s', {c: city}, function(data) {
                if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                    p.ecityid = data.city.id;
                }
            });
        }

        if (p.sll && p.ell) o.e_submit.removeAttr('disabled');
    },

    rendDirect: function() {
        var o = bmoon.icar.init();

        if (!o.plan.sll || !o.plan.ell) return;

        var opts = {
            origin: new google.maps.LatLng(o.plan.sll[0], o.plan.sll[1]),
            destination: new google.maps.LatLng(o.plan.ell[0], o.plan.ell[1]),
            travelMode: google.maps.TravelMode.DRIVING
        };
        
        o.g_dirservice.route(opts, function(result, status) {
            if (status == google.maps.DirectionsStatus.OK) {
                o.g_dirrender.setDirections(result);
            }
        });
    },

    bindMapChange: function() {
        var o = bmoon.icar.init();

        google.maps.event.addListener(o.g_smarker, 'dragend', function() {
            o.markDraged('s');
        });
        google.maps.event.addListener(o.g_emarker, 'dragend', function() {
            o.markDraged('e');
        });

        google.maps.event.addListener(o.g_sauto, 'place_changed', function() {
            o.autoChanged('s');
        });
        google.maps.event.addListener(o.g_eauto, 'place_changed', function() {
            o.autoChanged('e');
        });
    },

    markDraged: function(x) {
        var o = bmoon.icar.init();

        var marker = o.g_smarker,
        addr = o.e_saddr;
        if (x == 'e') {
            marker = o.g_emarker;
            addr = o.e_eaddr;
        }

        var pos = marker.getPosition();
        o.g_geocode.geocode({latLng: pos}, function(results, status) {
            if (status == google.maps.GeocoderStatus.OK) {
                addr.val(results[0].formatted_address);
                o.g_infow.setContent(o._formAddr(
                    results[0].address_components[0].short_name,
                    results[0].address_components,
                    results[0].geometry.location));
                o.g_infow.open(o.g_map, marker);

                o.upPlan(x, results[0]);
                o.rendDirect();
            }
        });
    },

    autoChanged: function(x) {
        var o = bmoon.icar.init();
        
        var marker = o.g_smarker,
        auto = o.g_sauto;
        
        if (x == 'e') {
            marker = o.g_emarker;
            auto = o.g_eauto;
        }

        o.g_infow.close();
        var place = auto.getPlace();
        if (place.geometry.viewport) {
            o.g_map.fitBounds(place.geometry.viewport);
        } else {
            o.g_map.setCenter(place.geometry.location);
        }
        
        marker.setPosition(place.geometry.location);
        
        o.g_infow.setContent(o._formAddr(
            place.name,
            place.address_components,
            place.geometry.location));
        o.g_infow.open(o.g_map, marker);

        o.upPlan(x, place);
        o.rendDirect();
    }
};

$(document).ready(bmoon.icar.onready);
