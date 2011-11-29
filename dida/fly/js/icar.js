; var bmoon = bmoon || {};
bmoon.icar = {
    version: '1.0',

    init: function() {
        var o = bmoon.icar;
        if (o.inited) return o;

        o.sdate = $('#s-date').datePicker({inline: true}).dpDisplay();
        o.tabs = $('ul.css-tabs').tabs('div.css-panes > div', {api: true});
        
        o.inited = true;
        return o;
    },
    
    onready: function() {
        var o = bmoon.icar.init();

        o.bindClick();
    },

    bindClick: function() {
        var o = bmoon.icar.init();

    }
};

$(document).ready(bmoon.icar.onready);
