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

    // "(28.228209,114.057868)" => [28.228209, 114.057868]
    _dbpoint2ll: function(s) {
        var a = s.split(','),
        lat = a[0].match(/[0-9\.]+/),
        lng = a[1].match(/[0-9\.]+/),
        ret = [parseFloat(lat), parseFloat(lng)];
        return ret;
    },

    // "(28.228209,114.057868),(22.543099,112.938814)"
    // => [[22.543099,112.938814], [28.228209,114.057868]]
    _dbbox2ll: function(s) {
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
    
    init: function() {
        var o = bmoon.icar;
        if (o.inited) return o;

        o.e_hour = $('#hour');
        o.e_min = $('#min');
        o.e_dad = $('#dad');
        o.e_dateinput = $('#date-input');
        o.e_datehint = $('#date-hint');

        o.e_saddr = $('#saddr');
        o.e_eaddr = $('#eaddr');

        o.e_submit = $('#submit');

        o.e_mc_result = $('#mc-result');
        o.e_mc_prev = $('#mc-prev');
        o.e_mc_next = $('#mc-next');
        o.e_mc_nick = $('#mc-nick');
        o.e_mc_saddr = $('#mc-saddr');
        o.e_mc_eaddr = $('#mc-eaddr');
        o.e_mc_sdate = $('#mc-sdate');
        o.e_mc_stime = $('#mc-stime');
        o.e_mc_phone = $('#mc-phone');
        o.e_mc_attach = $('#mc-attach');
        o.e_mc_num_total = $('#mc-num-total');
        o.e_mc_num_cur = $('#mc-num-cur');

        o.plan = {};

        o.inited = true;
        return o;
    },
    
    initMap: function(geo) {
        var o = bmoon.icar.init();

        geo = geo || [28.188,113.033];
        o.g_lat = new google.maps.LatLng(geo[0], geo[1]);
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
        o.g_prect = new google.maps.Rectangle(); // matched plan's rect
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
        
        //$.getJSON('/json/city/ip', {ip: '118.145.22.78'}, function(data) {
        $.getJSON('/json/city/ip', null, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                o.initMap(o._dbpoint2ll(data.city.geopos));
            } else o.initMap();
        });

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.icar.init();

        o.calendar.bind("onShow onHide", function()  {
	        $(this).parent().toggleClass("active"); 
        });

        o.e_submit.click(o.matchPlan);
        o.e_mc_prev.click(function() {o.rendPlan(o._pcur-1);});
        o.e_mc_next.click(function() {o.rendPlan(o._pcur+1);});
    },

    matchPlan: function() {
        var o = bmoon.icar.init();

        var p = $(this).parent(),
        plan = o.plan;

        o.e_mc_result.fadeOut();
        o.g_prect.setMap(null);
        
        if (!plan.sll) {
            o.e_saddr.focus();
            return;
        }
        if (!plan.ell) {
            o.e_eaddr.focus();
            return;
        }

        plan.dad  = o.e_dad.val();
        plan.date = o.e_dateinput.val();
        //o.dayow = xxx;
        plan.time = o.e_hour.val() + ':' + o.e_min.val();
        plan.rect = '((' + plan.sll.join(',') + '),(' + plan.ell.join(',') + '))';
        //plan.sll = [212.12, 232.33];
        //plan.ell = [212.12, 232.33];
        //plan.saddr = ... plan.scity = ..
        //plan.eaddr = ... plan.ecity = ..
        //plan.scityid = x plan.ecityid = x

		$('.vres', p).remove();
		p.removeClass('success').removeClass('error').addClass('loading');
        
        $.getJSON('/json/plan/match', plan, function(data) {
            p.removeClass('loading');
            if (data.success == '1' && bmoon.utl.type(data.plans) == 'Array') {
                p.addClass('success');

                o._pnum = data._ntt;
                o.mplans = data.plans;
                o.e_mc_num_total.html(data._ntt);
                o.rendPlan(0);
            } else {
                p.addClass('error');
                $('<span class="vres">'+ data.errmsg + '</span>').appendTo(p);
            }
        });
    },

    rendPlan: function(ncur) {
        var o = bmoon.icar.init();
        
        var plan = o.mplans[ncur],
        geo = o._dbbox2ll(plan.rect),
        bounds = new google.maps.LatLngBounds(
          new google.maps.LatLng(geo[0][0], geo[0][1]),
          new google.maps.LatLng(geo[1][0], geo[1][1])
        );

        o.g_prect.setOptions({
            strokeColor: "#008888",
            strokeOpacity: 0.8,
            strokeWeight: 2,
            fillColor: "#aaaaaa",
            fillOpacity: 0.35,
            bounds: bounds,
            map: o.g_map
        });

        o.rendMatch(plan, ncur);
    },

    rendMatch: function(plan, ncur) {
        var o = bmoon.icar.init();

        o._pcur = ncur;
        if (ncur > 0) o.e_mc_prev.parent().show();
        else o.e_mc_prev.parent().hide();
        if (ncur < o._pnum-1) o.e_mc_next.parent().show();
        else o.e_mc_next.parent().hide();
        
        o.e_mc_nick.html(plan.nick);
        o.e_mc_attach.html(plan.attach);
        o.e_mc_num_cur.html(ncur+1);
        o.e_mc_saddr.html(plan.saddr);
        o.e_mc_eaddr.html(plan.eaddr);
        if (plan.repeat == 1) {
            o.e_mc_sdate.html('每日');
        } else if (plan.repeat == 2) {
            o.e_mc_sdate.html('每周 ' + plan.sdate);
        } else o.e_mc_sdate.html(plan.sdate);
        o.e_mc_stime.html(plan.stime);
        
        o.e_mc_result.fadeIn('slow');

        o.e_mc_phone.attr('src', '');
        $.getJSON('/json/member/info', {mid: plan.mid}, function(data) {
            if (data.success == '1' && data.phone.match(/http:.*/)) {
                o.e_mc_phone.attr('src', bmoon.utl.clotheHTML(data.phone));
            }
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
        } else {
            p.ell = [data.geometry.location.lat(), data.geometry.location.lng()];
            p.eaddr = data.formatted_address;
            p.ecity = city;
        }
        $.getJSON('/json/city/s', {c: city}, function(data) {
            if (data.success == 1 && bmoon.utl.type(data.city) == 'Object') {
                if (x != 'e') p.scityid = data.city.id;
                else p.ecityid = data.city.id;

                if (p.sll && p.ell) o.e_submit.removeAttr('disabled');
            }
        });
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
                setTimeout(function() {o.g_infow.close();}, 2000);

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
        setTimeout(function() {o.g_infow.close();}, 2000);

        o.upPlan(x, place);
        o.rendDirect();
    }
};

$(document).ready(bmoon.icar.onready);
