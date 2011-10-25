; var bmoon = bmoon || {};
bmoon.map = {
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
        var o = bmoon.map;
        if (o.inited) return o;

        o.addr = $('#addr');
        o.map = $('#map');

        o.slatlng = {};
        o.elatlng = {};

        o.glat = new google.maps.LatLng(28.188, 113.033);
        o.gmap = new google.maps.Map($('#map')[0], {
            zoom: 10,
            center: o.glat,
            mapTypeId: google.maps.MapTypeId.ROADMAP,
            region: 'zh-CN'
        });

        o.gauto = new google.maps.places.Autocomplete($('#addr')[0]);
        o.ggeocode = new google.maps.Geocoder();
        o.ginfow = new google.maps.InfoWindow();
        o.gmarker = new google.maps.Marker({
            map: o.gmap,
            draggable: true,
            title: '设置起始位置'
        });
        

        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.map.init();

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.map.init();

        o.gauto.bindTo('bounds', o.gmap);

        o.bindAddrChange();
    },

    bindAddrChange: function() {
        var o = bmoon.map.init();

        google.maps.event.addListener(o.gmarker, 'dragend', function() {
            var pos = o.gmarker.getPosition();
            o.ggeocode.geocode({latLng: pos}, function(results, status) {
                if (status == google.maps.GeocoderStatus.OK) {
                    o.slatlng = {
                        lat: results[0].geometry.location.lat(),
                        lng: results[0].geometry.location.lng()
                    };
                    o.addr.val(results[0].formatted_address);
                    o.ginfow.setContent(o._formAddr(
                        results[0].address_components[0].short_name,
                        results[0].address_components,
                        results[0].geometry.location));
                    o.ginfow.open(o.gmap, o.gmarker);
                }
            });
        });
        
        google.maps.event.addListener(o.gauto, 'place_changed', function() {
            o.ginfow.close();
            var place = o.gauto.getPlace();
            if (place.geometry.viewport) {
                o.gmap.fitBounds(place.geometry.viewport);
            } else {
                o.gmap.setCenter(place.geometry.location);
                o.gmap.setZoom(15);
            }
            
            o.gmarker.setPosition(place.geometry.location);
            
            o.ginfow.setContent(o._formAddr(
                place.name,
                place.address_components,
                place.geometry.location));
            o.ginfow.open(o.gmap, o.gmarker);
        });
    }
};

$(document).ready(bmoon.map.onready);
