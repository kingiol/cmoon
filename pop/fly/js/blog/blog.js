; var bmoon = bmoon || {};
bmoon.blog = {
	version: '1.0',
	
	init: function() {
		var o = bmoon.blog;
		if (o.inited) return o;

		o.inited = true;
		return o;
	},
	
	onready: function() {
		var o = bmoon.blog.init();

		o.bindClick();
	},

	bindClick: function() {
		var o = bmoon.blog.init();

	}
};

$(document).ready(bmoon.blog.onready);
