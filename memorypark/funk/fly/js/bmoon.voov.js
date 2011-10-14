; var g_site_domain = "voov.com";
var bmoon = bmoon || {};
bmoon.voov = {
    version: '1.0',

    init: function() {
        var o = bmoon.voov;

        if (o.inited) return o;
        o.inited = true;

        o.content = $('#bd-content');

        return o;
    },

    onready: function() {
        var o = bmoon.voov.init();

        o.bindClick();
    },
    
    bindClick: function() {
        var o = bmoon.voov.init();
        
    }
};

$(document).ready(bmoon.voov.onready);
