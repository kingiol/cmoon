; var bmoon = bmoon || {};
bmoon.omsstat = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.omsstat;
		if (o.inited) return o;

		o.ht_exist = $('#hit-appexist');
		o.ht_nexist = $('#hit-appnexist'),

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.omsstat.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.omsstat.init();
		
	}
};

$(document).ready(bmoon.omsstat.onready);
