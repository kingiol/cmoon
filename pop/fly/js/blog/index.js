; var bmoon = bmoon || {};
bmoon.blogindex = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.blogindex;
		if (o.inited) return o;

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.blogindex.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.blogindex.init();

	}
};

$(document).ready(bmoon.blogindex.onready);
