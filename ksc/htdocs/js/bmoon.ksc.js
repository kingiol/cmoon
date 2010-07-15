; var g_site_domain = "kossenchem.com";
var bmoon = bmoon || {};
bmoon.ksc = {
	version: "1.0",

	init: function() {
		var o = bmoon.ksc;

		if (o.inited) return o;
		o.inited = true;

		o.uname = $('#unameLayout');
		o.nav_guest = $('#nav-guest');
		o.nav_member = $('#nav-member');
		
		return o;
	},

	onready: function() {
		var o = bmoon.ksc.init();

		o.bindClick();
		o.loginCheck();
	},
	
	bindClick: function() {
		var o = bmoon.ksc.init();
		
		o.loginoverlay = $("a[rel=#loginoverlay]").overlay({
			api: true,
			closeOnClick: false,
			onBeforeLoad: function() {
				//var wrap = this.getContent().find("div.wrap");
				var wrap = this.getOverlay().find(".wrap");
				if (wrap.is(":empty")) {
					wrap.load(this.getTrigger().attr("href"));
				}
			}
		});
		
		$("#userlogout").click(function(){
			$.cookie("uname", null, {path: '/', domain: g_site_domain});
			$.cookie("usn", null, {path: '/', domain: g_site_domain});
			o.loginCheck();
		});
		
		$(".prditem img").tooltip();
	},

	loginCheck: function() {
		var o = bmoon.ksc.init();
		
		var uname = $.cookie("uname");
		if (uname != null) {
			o.uname.text(uname);

			o.nav_guest.hide();
			o.nav_member.show();
		} else {
			o.nav_member.hide();
			o.nav_guest.show();
		}
	}
};

$(document).ready(bmoon.ksc.onready);
