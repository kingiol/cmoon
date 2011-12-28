; var g_site_domain = "dida.com";
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

        o.content = $('#bd-content');

        return o;
    },

    onready: function() {
        var o = bmoon.dida.init();

        o.bindClick();
    },
    
    bindClick: function() {
        var o = bmoon.dida.init();
        
    }
};

$(document).ready(bmoon.dida.onready);
