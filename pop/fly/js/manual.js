; var bmoon = bmoon || {};
bmoon.manual = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.manual;
		if (o.inited) return o;

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.manual.init();

		o.bindClick();
		
		bmoon.kol.onready({
	  		aname: 'kol',
			css: 'http://css.kaiwuonline.com/b/client/mic.css',
			maxevent: 'mouseenter',
			pos: {
				top: '35%',
				right: '0'
			}
		});
	},

	bindClick: function() {
		var o = bmoon.manual.init();

	}
};

$(document).ready(bmoon.manual.onready);
