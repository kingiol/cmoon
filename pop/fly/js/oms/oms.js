; var bmoon = bmoon || {};
bmoon.oms = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.oms;
		if (o.inited) return o;

		o.scroll = $(".scrollable").scrollable({api: true});
		o.navi = $(".scrollable").navigator({api: true});

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.oms.init();

	}
};

$(document).ready(bmoon.oms.onready);
